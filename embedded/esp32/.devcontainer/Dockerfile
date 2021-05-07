# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.145.1/containers/cpp/.devcontainer/base.Dockerfile

# [Choice] Debian / Ubuntu version: debian-10, debian-9
ARG VARIANT="buster"
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

# [Optional] Uncomment this section to install additional packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends git wget flex git-lfs \
    bison gperf python python-pip python-setuptools cmake ninja-build \
    ccache libffi-dev libssl-dev dfu-util python3 python3-pip python3-setuptools

# Configure Python 3 as the default version.
RUN sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 10

# Install ESP-IDF.
RUN mkdir -p ~/esp \
    && cd ~/esp \
    && git clone --recursive https://github.com/espressif/esp-idf.git \
    && cd ~/esp/esp-idf \
    && ./install.sh

# Make ESP-IDF tool available in PATH.
RUN echo "source $HOME/esp/esp-idf/export.sh > /dev/null" >> ~/.bashrc
