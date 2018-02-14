FROM phusion/baseimage:0.9.22

RUN add-apt-repository ppa:mongoose-os/mos && \
    apt-get update  -qq  && \
    apt-get install -y sudo  && \
    apt-get install -y mos && \
    apt-get install -y wget clang-3.9 make && \
    apt-get install -qqy apt-transport-https ca-certificates

# Update Mongoose OS to last version
RUN mos update

# Install Docker from Docker Inc. repositories.
RUN curl -sSL https://get.docker.com/ | sh

# Install the magic wrapper.
ADD ./wrapdocker /usr/local/bin/wrapdocker
RUN chmod +x /usr/local/bin/wrapdocker

# Define additional metadata for our image.
VOLUME /var/lib/docker

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

CMD ["wrapdocker"]