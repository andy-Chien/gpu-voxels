#!/bin/bash

# Copyright (c) 2020, NVIDIA CORPORATION. All rights reserved.
# Full license terms provided in LICENSE.md file.

CONTAINER_NAME=$1
if [[ -z "${CONTAINER_NAME}" ]]; then
    CONTAINER_NAME=moveit_devel
fi

# This specifies a mapping between a host directory and a directory in the
# docker container. This mapping should be changed if you wish to have access to
# a different directory
HOST_DIR=$2
if [[ -z "${HOST_DIR}" ]]; then
    HOST_DIR=`realpath ${PWD}/..`
fi

CONTAINER_DIR=$3
if [[ -z "${CONTAINER_DIR}" ]]; then
    CONTAINER_DIR=/home/solomon/catkin_ws/src
fi

IMAGE_NAME=$4
if [[ -z "${IMAGE_NAME}" ]]; then
    IMAGE_NAME=moveit_devel:-v0
fi

echo "Container name     : ${CONTAINER_NAME}"
echo "Host directory     : ${HOST_DIR}"
echo "Container directory: ${CONTAINER_DIR}"
MOVEIT_DEVEL_ID=`docker ps -aqf "name=^/${CONTAINER_NAME}$"`
if [ -z "${MOVEIT_DEVEL_ID}" ]; then
    echo "Creating new moveit_devel docker container."
    xhost +local:root
    docker run --gpus all  -it --shm-size=4096m --privileged --network=host -v ${HOST_DIR}:${CONTAINER_DIR}:rw -v /tmp/.X11-unix:/tmp/.X11-unix:rw --env="DISPLAY" --name=${CONTAINER_NAME} ${IMAGE_NAME} bash
else
    echo "Found moveit_devel docker container: ${MOVEIT_DEVEL_ID}."
    # Check if the container is already running and start if necessary.
    if [ -z `docker ps -qf "name=^/${CONTAINER_NAME}$"` ]; then
        xhost +local:${MOVEIT_DEVEL_ID}
        echo "Starting and attaching to ${CONTAINER_NAME} container..."
        docker start ${MOVEIT_DEVEL_ID}
        docker attach ${MOVEIT_DEVEL_ID}
    else
        echo "Found running ${CONTAINER_NAME} container, attaching bash..."
        docker exec -it ${MOVEIT_DEVEL_ID} bash
    fi
fi