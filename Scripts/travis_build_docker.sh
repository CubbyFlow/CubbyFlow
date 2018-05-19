#!/usr/bin/env bash

set -e

docker build -f $1 -t utilForever/CubbyFlow-v0:$2 .
docker run utilForever/CubbyFlow-v0