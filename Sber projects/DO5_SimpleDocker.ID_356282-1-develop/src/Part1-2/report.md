## Part 1. Готовый докер ##

- Взял официальный докер-образ с nginx и выкачал его при помощи `docker pull nginx`

    ![](../screenshots/Screenshot_1_1.png)  
    *Рис. 1. Результат выполнения команды `docker pull`*

- Проверил наличие докер-образа через docker images

    ![](../screenshots/Screenshot_1_2.png)  
    *Рис. 2. Результат выполнения команды `docker image ls`*

- Запустил докер-образ через `docker run -d nginx:latest`

    ![](../screenshots/Screenshot_1_3.png)  
    *Рис. 3. Результат выполнения команды `docker run -d nginx:latest`*

- Провер, что образ запустился через `docker ps`

    ![](../screenshots/Screenshot_1_4.png)  
    *Рис. 4. Результат выполнения команды `docker ps`*

- Посмотрел информацию о контейнере через `docker inspect`

    ![](../screenshots/Screenshot_1_5.png)  
    *Рис. 5. Результат выполнения команды `docker inspect`*  

    ![](../screenshots/Screenshot_1_6.png)  
    *Рис. 6. Результат выполнения команды `docker inspect` (продолжение вывода)*

- По выводу команды определил и поместил в отчёт размер контейнера, список замапленных портов и ip контейнера

    ![](../screenshots/Screenshot_1_7.png)  
    *Рис. 7. Размер контейнера*  

    ![](../screenshots/Screenshot_1_8.png)  
    *Рис. 8. Список замапленных портов*  

    ![](../screenshots/Screenshot_1_9.png)  
    *Рис. 9. ip контейнера*

- Остановил докер контейнер через `docker stop`. Проверил, что контейнер остановился через `docker ps`

    ![](../screenshots/Screenshot_1_10.png)  
    *Рис. 10. Результат выполнения команд `docker stop`, `docker ps`*

- Запусти докер с портами 80 и 443 в контейнере, замапленными на такие же порты на локальной машине, через команду `docker run -p 80:80 -p 443:443 -d nginx:latest`

    ![](../screenshots/Screenshot_1_11.png)  
    *Рис. 11. Результат выполнения команды `docker run -p 80:80 -p 443:443 -d nginx:latest`*

- Проверил, что в браузере по адресу localhost:80 доступна стартовая страница nginx

    ![](../screenshots/Screenshot_1_12.png)  
    *Рис. 12. Стартовая страница nginx*

- Перезапустил докер контейнер через `docker restart`. Проверил через `docker ps`, что контейнер запустился

    ![](../screenshots/Screenshot_1_13.png)  
    *Рис. 13. Результат выполнения команд `docker restart`, `docker ps`*

## Part 2. Операции с контейнером ##

- Прочитал конфигурационный файл nginx.conf внутри докер контейнера через команду exec

    ![](../screenshots/Screenshot_2_1.png)  
    *Рис. 1. Результат выполнения команд `docker exec -it frosty_cohen /bin/bash`, `cat /etc/nginx/nginx.conf`*

- Создал на локальной машине файл nginx.conf

    ![](../screenshots/Screenshot_2_2.png)  
    *Рис. 2. Результат выполнения команды `touch nginx.conf`*

- Настроил в нем по пути /status отдачу страницы статуса сервера nginx

    ![](../screenshots/Screenshot_2_3.png)  
    *Рис. 3. Содержимое файла nginx.conf*

- Скопировал созданный файл nginx.conf внутрь докер-образа через команду `docker cp nginx.conf frosty_cohen:/etc/nginx/`. Перезапустил nginx внутри докер-образа через команду `docker exec -it frosty_cohen nginx -s reload`

    ![](../screenshots/Screenshot_2_4.png)  
    *Рис. 4. Результат выполнения команд `docker cp nginx.conf frosty_cohen:/etc/nginx/`, `docker exec -it frosty_cohen nginx -s reload`*

- Проверил, что по адресу localhost:80/status отдается страничка со статусом сервера nginx

    ![](../screenshots/Screenshot_2_5.png)  
    *Рис. 5. Страничка со статусом сервера nginx*

- Экспортировал контейнер в файл container.tar через команду `docker export -o container.tar frosty_cohen`

    ![](../screenshots/Screenshot_2_6.png)  
    *Рис. 6. Результат выполнения команды `docker export -o container.tar frosty_cohen`*

- Остановил контейнер

    ![](../screenshots/Screenshot_2_7.png)  
    *Рис. 7. Результат выполнения команды `docker stop frosty_cohen`*

- Удалил образ через `docker rmi -f nginx:latest`, не удаляя перед этим контейнеры

    ![](../screenshots/Screenshot_2_8.png)  
    *Рис. 8. Результат выполнения команды `docker rmi -f nginx:latest`*

- Удали остановленный контейнер

    ![](../screenshots/Screenshot_2_9.png)  
    *Рис. 9. Результат выполнения команды `docker rm frosty_cohen`*

- Импортировал контейнер обратно через команду `docker import -c 'CMD ["nginx", "-g", "daemon off;"]' container.tar nginx`. Запустил импортированный контейнер командой `docker run -p 80:80 -p 443:443 -d nginx:latest`

    ![](../screenshots/Screenshot_2_10.png)  
    *Рис. 10. Результат выполнения команд `docker import -c 'CMD ["nginx", "-g", "daemon off;"]' container.tar nginx`, `docker run -p 80:80 -p 443:443 -d nginx:latest`*

- Проверил, что по адресу localhost:80/status отдается страничка со статусом сервера nginx

    ![](../screenshots/Screenshot_2_11.png)  
    *Рис. 11. Страничка со статусом сервера ngin*
