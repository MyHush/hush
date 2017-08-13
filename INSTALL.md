# Building Hush

Building Hush from source will take some time but your efforts will be rewarded :)

## Requirements

You will need at least 2GB of RAM to build hush from git source, OR
you can enable a swap file. To enable a 4GB swap file on modern Linux
distributions:

    sudo fallocate -l 4G /swapfile
    sudo chmod 600 /swapfile
    sudo mkswap /swapfile
    sudo swapon /swapfile

You will need to have Git and a C++ compiler and libtool and a
a few other libraries, depending on your setup.

## Supported Platforms

We aim to support all platforms, but there is only so much time in the day.

Currently, any ARMv7 machine will not be able to build this repo, because the
underlying tech (zcash and the zksnark library) do not support that instruction
set.

This also means that RaspberryPi devices will not work, unless they have a newer ARMv8

## Compiling from Git source

    git clone https://github.com/MyHush/hush.git
    cd hush
    ./zcutil/build.sh

