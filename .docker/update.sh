#!/bin/bash

# Setup a minimal docker image with C++ compiler
docker build -t dev-env-min -f Dockerfile.min .
docker tag dev-env-min:latest jryland/dev-env-min:latest
docker push jryland/dev-env-min:latest

# Setup base docker image
docker build -t dev-env -f Dockerfile.devenv .
docker tag dev-env:latest jryland/dev-env:latest
docker push jryland/dev-env:latest

# Setup derived docker image for use in github workflows
docker build -t dev-env-github -f Dockerfile.github .
docker tag dev-env-github:latest jryland/dev-env-github:latest
docker push jryland/dev-env-github:latest

# Setup derived docker image for use in VSCode
docker build -t dev-env-vscode -f Dockerfile.vscode .
docker tag dev-env-vscode:latest jryland/dev-env-vscode:latest
docker push jryland/dev-env-vscode:latest

