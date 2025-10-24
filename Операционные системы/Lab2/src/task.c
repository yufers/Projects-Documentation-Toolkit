#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>

#define SHM_NAME "/my_shm"
#define M (1ULL << 30) // 1 ГБ
#define K (128 * 1024) // 128 КБ
#define W (128 * 1024 * 1024) // 128 МБ

// Функция для заполнения объекта случайными числами
void fill_shm(int fd) {
    uint64_t *buffer = malloc(K);
    if (!buffer) {
        perror("malloc failed");
        exit(1);
    }
    srand(time(NULL));
    off_t offset = 0;
    while (offset < M) {
        for (size_t i = 0; i < K / sizeof(uint64_t); i++) {
            buffer[i] = ((uint64_t)rand() << 32) | rand();
        }
        ssize_t written = write(fd, buffer, K);
        if (written < 0) {
            perror("write failed");
            free(buffer);
            exit(1);
        }
        offset += written;
    }
    free(buffer);
}

// Алгоритм A: блочно-буферизированное чтение
uint64_t algorithm_a(int fd) {
    uint64_t sum = 0;
    uint64_t *buffer = malloc(K);
    if (!buffer) {
        perror("malloc failed");
        exit(1);
    }
    off_t offset = 0;
    while (offset < M) {
        ssize_t bytes_read = pread(fd, buffer, K, offset);
        if (bytes_read < 0) {
            perror("pread failed");
            free(buffer);
            exit(1);
        }
        for (size_t i = 0; i < bytes_read / sizeof(uint64_t); i++) {
            sum += buffer[i]; // Сумма по модулю 2^64
        }
        offset += bytes_read;
    }
    free(buffer);
    return sum;
}

// Алгоритм B: скользящее окно с mmap
uint64_t algorithm_b(int fd) {
    uint64_t sum = 0;
    void *addr = NULL;
    off_t offset = 0;
    while (offset < M) {
        size_t len = (offset + W <= M) ? W : M - offset;
        addr = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, offset);
        if (addr == MAP_FAILED) {
            perror("mmap failed");
            exit(1);
        }
        uint64_t *data = (uint64_t *)addr;
        for (size_t i = 0; i < len / sizeof(uint64_t); i++) {
            sum += data[i]; // Сумма по модулю 2^64
        }
        munmap(addr, len);
        offset += len;
    }
    return sum;
}

int main() {
    // Создание объекта разделяемой памяти
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600);
    if (fd < 0) {
        perror("shm_open failed");
        exit(1);
    }
    if (ftruncate(fd, M) < 0) {
        perror("ftruncate failed");
        exit(1);
    }

    // Заполнение объекта случайными числами
    printf("Filling shared memory...\n");
    fill_shm(fd);

    // Алгоритм A
    printf("Running Algorithm A...\n");
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t sum_a = algorithm_a(fd);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_a = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Algorithm A: Sum = %" PRIu64 ", Time = %.6f seconds\n", sum_a, time_a);

    // Алгоритм B
    printf("Running Algorithm B...\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t sum_b = algorithm_b(fd);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_b = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Algorithm B: Sum = %" PRIu64 ", Time = %.6f seconds\n", sum_b, time_b);

    // Очистка
    shm_unlink(SHM_NAME);
    close(fd);
    return 0;
}