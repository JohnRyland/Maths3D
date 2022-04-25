FROM ubuntu

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update

# Install standard C/C++ development tools
RUN apt install -y build-essential
RUN apt install -y git
RUN apt install -y gcovr
RUN apt install -y clang clang-format

# Install documentation tools
RUN apt install -y doxygen graphviz plantuml
RUN apt install -y texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra
RUN apt install -y pandoc librsvg2-bin

# Configure for use with docker 'Dev Environments'
RUN useradd -s /bin/bash -m vscode
RUN groupadd docker
RUN usermod -aG docker vscode
USER vscode

