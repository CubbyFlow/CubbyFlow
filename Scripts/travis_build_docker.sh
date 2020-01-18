#!/usr/bin/env bash

set -e

if [ $# -eq 0 ]
  then
    docker build -t utilforever/cubbyflow .
else
    docker build -f $1 -t utilforever/cubbyflow:$2 .
fi
docker run utilforever/cubbyflow