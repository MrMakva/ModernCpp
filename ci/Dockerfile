# docker build -t registry.csc.sibsutis.ru/cpp/ci .
# docker push registry.csc.sibsutis.ru/cpp/ci

FROM alpine:3.20

RUN apk add --no-cache \
    clang17-extra-tools \
    findutils \
    g++ \
    gcc \
    git \
    gtest-dev \
    make \
    valgrind

COPY etc/clang-tidy /etc/clang-tidy
COPY csc_cpp_lint /usr/bin/csc_cpp_lint
