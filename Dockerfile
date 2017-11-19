FROM phusion/baseimage:0.9.22

RUN add-apt-repository ppa:mongoose-os/mos && \
    apt-get update && \
    apt-get install -y mos && \
    apt-get install -y wget clang-3.9 make

ENV CC /usr/bin/clang-3.9
ENV CXX /usr/bin/clang++-3.9
ENV MAKE /usr/bin/make

ENV version 3.9.6
RUN mkdir -p ~/temp
WORKDIR ~/temp
RUN wget https://cmake.org/files/v3.9/cmake-$version.tar.gz && \
    tar -xzvf cmake-$version.tar.gz
WORKDIR cmake-$version/
RUN ./bootstrap && \
    make -j4 && \
    make install

ENTRYPOINT ["/bin/sh"]