#!/usr/bin/env bash

set -e

docker build -f $1 -t utilforever/cubbyflow-v0:$2 .
docker run utilforever/cubbyflow-v0