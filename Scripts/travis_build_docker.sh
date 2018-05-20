#!/usr/bin/env bash

set -e

docker build -t utilforever/cubbyflow-v0 .
docker run utilforever/cubbyflow-v0