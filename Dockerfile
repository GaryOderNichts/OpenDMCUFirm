FROM ubuntu:24.04

# install needed packages
RUN dpkg --add-architecture i386 \
    && apt update \
    && apt install -y --no-install-recommends build-essential python3 wget wine-stable wine32 innoextract \
    && rm -rf /var/lib/apt/lists/*

# setup gnu-68hc1x
WORKDIR /gnu-68hc1x
RUN wget http://gnu-m68hc11.vacs.fr/EXE/gnu-68hc1x-3.1.exe \
    && echo "8259c3fd3ab52fd496d73b5cecddda0eecd48abdba3f67cd25c9c7c8b59e1c26  gnu-68hc1x-3.1.exe" | sha256sum -c - \
    && innoextract gnu-68hc1x-3.1.exe \
    && chmod +x app/bin/*.exe \
    && cd app/ && wine ./bin/winstall.exe

# supress wine spam
ENV WINEDEBUG=-all

WORKDIR /project
