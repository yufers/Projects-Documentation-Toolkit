#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#define D 2 // глубина дерева, можно изменить

volatile sig_atomic_t stop_flag = 0;

void create_tree(int depth, int max_depth, pid_t parent_pid);

void gf_handler(int signo)
{
    stop_flag = 1;
}

pid_t create_child(int depth, int max_depth, pid_t parent_pid)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        create_tree(depth + 1, max_depth, getppid());
        exit(0);
    }
    if (pid < 0)
        perror("Ошибка порождения процесса!");
    return pid;
}

void leaf_logic(pid_t parent_pid)
{
    printf("[Leaf %d] Ожидаю SIGINT для завершения\n", getpid());
    while (!stop_flag)
        sleep(1);
    printf("[Leaf %d] Получил SIGINT, отправляю родителю %d\n", getpid(), parent_pid);
    if (parent_pid > 0)
        kill(parent_pid, SIGINT);
    printf("[Leaf %d] Завершаюсь\n", getpid());
    exit(0);
}

void node_logic(pid_t left, pid_t right, pid_t parent_pid, int depth)
{
    while (!stop_flag)
        sleep(1);
    if (left > 0)
        kill(left, SIGINT);
    if (right > 0)
        kill(right, SIGINT);
    if (parent_pid > 0)
        kill(parent_pid, SIGINT);

    if (left > 0)
        waitpid(left, NULL, 0);
    if (right > 0)
        waitpid(right, NULL, 0);

    printf("[PID %d] Завершаюсь на глубине %d\n", getpid(), depth);
    exit(0);
}

void create_tree(int depth, int max_depth, pid_t parent_pid)
{

    pid_t left = -1, right = -1;
    if (depth < max_depth)
    {
        left = create_child(depth, max_depth, parent_pid);
        right = create_child(depth, max_depth, parent_pid);
    }
    printf("[PID %d] Стартовал на глубине %d, parent=%d, left=%d, right=%d\n", getpid(), depth, parent_pid, left, right);
    if (depth == max_depth)
        leaf_logic(parent_pid);
    node_logic(left, right, parent_pid, depth);
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    signal(SIGINT, gf_handler);
    printf("[ROOT %d] Запуск дерева глубины %d\n", getpid(), D);
    create_tree(0, D, 0);
    return 0;
}