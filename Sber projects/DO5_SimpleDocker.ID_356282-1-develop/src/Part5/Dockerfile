FROM nginx:1.29.0-alpine AS builder
WORKDIR /app  
RUN apk add --no-cache build-base fcgi-dev

COPY server.c /app
RUN gcc -o server server.c -lfcgi && chmod +x server

FROM nginx:1.29.0-alpine

WORKDIR /app
COPY --from=builder /app/server /app/

COPY nginx/nginx.conf /etc/nginx/conf.d/default.conf

RUN apk add --no-cache spawn-fcgi fcgi fcgi-dev bash; \
    chown -R nginx:nginx /etc/nginx/nginx.conf; \
    chown -R nginx:nginx /var/cache/nginx; \
    chown -R nginx:nginx /app; \
    touch /var/run/nginx.pid; \
    chown -R nginx:nginx /var/run/nginx.pid

EXPOSE 81

HEALTHCHECK --interval=5s --timeout=3s --start-period=5s --retries=3 \
    CMD curl -f http://localhost:80/health || exit 1

USER nginx

CMD ["/bin/bash", "-c", "/usr/bin/spawn-fcgi -f /app/server -p 8080; nginx -g 'daemon off;'"]