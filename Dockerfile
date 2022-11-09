# Export display: xhost +"local:docker@"
# Bugs: cmake not found
#
# Ubuntu Dockerfile
#

FROM ubuntu:20.04

#Set time zone
ENV TZ=Europe/Berlin
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

ENV DEBIAN_FRONTEND=noninteractive
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections

# Install.
RUN \
  apt-get update && \
  apt-get -y upgrade && \
  apt install -y build-essential && \
  apt install -y software-properties-common && \
  apt install -y cmake byobu curl git htop man unzip vim wget sudo && \
  apt install -y libcurl4-openssl-dev libssl-dev libuv1-dev qtdeclarative5-dev qml-module-qt-labs-platform qml-module-qtquick-controls qml-module-qtquick-controls2 qml-module-qtquick-shapes qml-module-qtquick-dialogs && \
  apt install -y pkg-config libssl-dev libzmq3-dev libsodium-dev libunwind8-dev liblzma-dev libreadline6-dev libpgm-dev qttools5-dev-tools libhidapi-dev libusb-1.0-0-dev libprotobuf-dev protobuf-compiler libudev-dev libboost-chrono-dev libboost-date-time-dev libboost-filesystem-dev libboost-locale-dev libboost-program-options-dev libboost-regex-dev libboost-serialization-dev libboost-system-dev libboost-thread-dev python3 ccache

WORKDIR /opt/
RUN wget https://github.com/libexpat/libexpat/releases/download/R_2_4_8/expat-2.4.8.tar.bz2
RUN tar -xf expat-2.4.8.tar.bz2
RUN rm expat-2.4.8.tar.bz2
WORKDIR /opt/expat-2.4.8
RUN ./configure --enable-static --disable-shared
RUN make
RUN sudo make install
WORKDIR /opt

RUN wget https://www.nlnetlabs.nl/downloads/unbound/unbound-1.16.1.tar.gz
RUN tar -xzf unbound-1.16.1.tar.gz
RUN rm unbound-1.16.1.tar.gz
WORKDIR /opt/unbound-1.16.1
RUN ./configure --disable-shared --enable-static --without-pyunbound --with-libevent=no --without-pythonmodule --disable-flto --with-pthreads --with-libunbound-only --with-pic
RUN make
RUN sudo make install
WORKDIR /opt

CMD ["bash"]