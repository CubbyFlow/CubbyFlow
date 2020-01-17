#!/usr/bin/env bash

set -e

docker build -t utilforever/cubbyflow .
docker run utilforever/cubbyflow