# LXC #

- lxc list # Отображает список всех контейнеров
- lxc list # Отображает список всех vm

- lxc start <name> # Запускает контейнер с заданным именем
- lxc start <name> # Запускает vm с заданным именем

- lxc stop <name> # Останавливает vm


- lxc rm <name> # Удаляет vm

- lxc launch ubuntu:24.04 <name> --vm -c limits.cpu=4 -c limits.memory=8GiB -c raw.idmap="both 1000 1000" # Создаёт виртуальную машину Ubuntu 24.04 с 4 ядрами, 8 ГБ памяти и пробросом UID/GID 1000

- lxc exec <name> -- sudo --login --user <user_name> # Входит в VM под указанным пользователем
