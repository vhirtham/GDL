#Download base image ubuntu 17.10
FROM ubuntu:17.10

# Update Ubuntu Software repository
RUN apt-get update

# Get basic libraries
RUN apt-get install -y cmake git g++ clang-5.0 libboost-test-dev

# Get libraries for codecov
RUN apt-get -y install lcov curl

# Get opengl related libraries
RUN apt-get install -y freeglut3-dev libglew-dev mesa-utils xvfb


# Create directories
RUN mkdir /home/usr && mkdir /home/usr/gdl && mkdir /home/usr/gdl/build && mkdir /home/usr/gdl/code

# Build mesa
RUN apt-get -y install pkg-config llvm-dev python python-mako zlib1g-dev expat libexpat1-dev libelf-dev
RUN mkdir /home/usr/mesa 
WORKDIR /home/usr/mesa
RUN curl https://mesa.freedesktop.org/archive/mesa-17.3.7.tar.gz --output mesa-17.3.7.tar.gz
RUN tar -xvzf mesa-17.3.7.tar.gz
WORKDIR /home/usr/mesa/mesa-17.3.7
RUN ./configure --enable-osmesa --disable-driglx-direct
RUN make -j8
RUN make install

#RUN export DISPLAY=:99 && export LIBGL_ALWAYS_SOFTWARE=1

WORKDIR /home/usr/gdl/build
