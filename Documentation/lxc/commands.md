# LXC #

- lxc list (Показывает список всех контейнеров)

- lxc start <name> (Запускает контейнер с заданным именем)

- lxc stop <name> (Останавливает контейнер)

- lxc rm <name> (Удаляет контейнер)

- lxc launch ubuntu:24.04 <name> --vm -c limits.cpu=4 -c limits.memory=8GiB -c raw.idmap="both 1000 1000" (Создаёт виртуальную машину Ubuntu 24.04 с 4 ядрами, 8 ГБ памяти и пробросом UID/GID 1000)

- lxc exec <name> -- sudo --login --user <user_name> (Входит в VM под указанным пользователем)

- lxc console <name> --type=vga (Запускает консоль LXC в режиме VGA)

- lxc start <name> --console=vga (Запускает контейнер с выводом в консоль VGA)


