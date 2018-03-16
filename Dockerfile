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

#RUN export DISPLAY=:99 && export LIBGL_ALWAYS_SOFTWARE=1

WORKDIR /home/usr/gdl/build