FROM ubuntu:22.04
RUN apt-get update && apt-get install build-essential nano -y
RUN mkdir /home/labOS
CMD cd /home/labOS && bash

# LINUX:
# docker run -ti --rm --name="labOS" --privileged -v /:/host -v "$(pwd):/home/labOS" --hostname "labOS" labos/ubuntu

# WINDOWS:
# docker run -ti --rm --name="labOS" --privileged -v /:/host -v "%cd%:/home/labOS" --hostname "labOS" labos/ubuntu