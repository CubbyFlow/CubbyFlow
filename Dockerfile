FROM ubuntu:18.04
MAINTAINER Chris Ohk <utilForever@gmail.com>

RUN apt-get update -yq && \
    apt-get install -yq build-essential python3-dev python3-pip python3-venv cmake

ADD . /app

WORKDIR /app/build
RUN cmake .. && \
    make -j`nproc` && \
    make install

RUN apt-get install -yq pkg-config libfreetype6-dev libpng-dev

WORKDIR /app/ENV3
RUN pip3 install -r ../requirements.txt && \
    pip3 install .. && \
    python3 ../Tests/PythonTests/main.py

WORKDIR /
