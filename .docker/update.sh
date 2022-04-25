#!/bin/bash

docker build -t dev-env .
docker tag dev-env:latest jryland/dev-env:latest
docker push jryland/dev-env:latest
