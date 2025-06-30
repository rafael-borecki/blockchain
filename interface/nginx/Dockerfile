# build web app
FROM node:23-slim AS builder
WORKDIR /front
COPY ./front .
RUN npm install
RUN npm run build

# nginx
FROM nginx:alpine
COPY ./nginx/default.conf /etc/nginx/conf.d/default.conf
COPY --from=builder /front/dist /usr/share/nginx/html
