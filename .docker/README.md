
# Using Docker


## Image Update Instructions

If need to add/remove tools from the dev-environment docker image then follow these steps:

1) Edit the Dockerfile to add or remove commands needed for installing the tools.

2) Login to docker. This might be possible in the UI in 'docker desktop' or on the command line:

```
docker login -u jryland
```

3) Run these commands:

```
docker build -t dev-env .
docker tag dev-env:latest jryland/dev-env:latest
docker push jryland/dev-env:latest
```

  or:

```
./update.sh
```

4) Verify, the image should have been uploaded to here:

     https://hub.docker.com/r/jryland/dev-env


## Using the Image

The image can be used inside a github workflow to run commands. See the documentation on
github for running directly from the Dockerfile without building it first as in the steps
above.

https://docs.github.com/en/actions/creating-actions/creating-a-docker-container-action

```
# action.yml
name: 'Hello World'
runs:
  using: 'docker'
  image: 'Dockerfile'
  args:
    - hello
```

The docker image can be used in a workflow like this:

```
jobs:
  testbox:
    runs-on: ubuntu-latest
    container:
      image: jryland/dev-env:latest
    steps:  
    - uses: actions/checkout@v2
    - name: What OS is running
      run: uname -a
    - name: What gcc version do we have
      run: gcc -v
```


## Using the Image in VSCode with Docker Desktop Deve Environments

This is a way to use the image in a way other than through a CLI which can be done easily
by pulling the image and running it in docker.

To use it instead inside of vscode, in 'Docker Desktop' open the dashboard and go to the
'Dev Environments' tab. Click the 'Create' button, then choose 'Existing Git repo' and enter
the URL to this git repository (https://github.com/JohnRyland/Maths3D.git).

Then wait for it to download the docker image and open it in VSCode. (This assumes VSCode is
installed which possibly Docker Desktop might do for you).



