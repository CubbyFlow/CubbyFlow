#!/usr/bin/env bash

set -e

docker build -t utilforever/CubbyFlow .
docker run utilforever/CubbyFlow