FROM nginx:1.29.0-alpine AS builder
WORKDIR /app  
RUN apk add --no-cache build-base fcgi-dev

COPY server.c /app
RUN gcc -o server server.c -lfcgi && chmod +x server

FROM nginx:1.29.0-alpine
RUN apk add --no-cache spawn-fcgi fcgi fcgi-dev bash

WORKDIR /app
COPY --from=builder /app/server /app/

COPY nginx/nginx.conf /etc/nginx/conf.d/default.conf

EXPOSE 81

CMD ["/bin/bash", "-c", "/usr/bin/spawn-fcgi -f /app/server -p 8080; nginx -g 'daemon off;'"]
