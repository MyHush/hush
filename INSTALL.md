# Building Hush

See the Zcash github wiki (https://github.com/zcash/zcash/wiki) for
instructions on building zcashd, the intended-for-services,
no-graphical-interface, reference implementation of Zcash.

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

## Compiling from Git source

    git clone https://github.com/MyHush/hush.git
    cd hush
    ./zcutil/build.sh
