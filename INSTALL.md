# Installing Hush

## Download and Install Hush v2.0.0 Stable Release

This method can be used to install release packages:

```sh
cd ~
sudo wget https://github.com/MyHush/hush/releases/download/v2.0.0/hush-2.0.0-c7d6ba61-amd64.deb
sudo dpkg -i hush-2.0.0-c7d6ba61-amd64.deb
```

To install Hush from source, read on.

## Build HUSH dependencies

The following build process generally applies to Ubuntu (and similar) Linux
distributions. For best results it is recommended to use Ubuntu Linux 16.04
or later.

## Swap Space (Optional)
You will need at least 4GB of RAM to build hush from git source, OR you can
enable a swap file. To enable a 4GB swap file on modern Linux distributions:

```sh
sudo fallocate -l 4G /swapfile
sudo chmod 600 /swapfile
sudo mkswap /swapfile
sudo swapon /swapfile
```

## Build on Linux:


```sh
# install build depedencies
sudo apt-get install build-essential pkg-config libc6-dev m4 g++-multilib \
      autoconf libtool ncurses-dev unzip git python zlib1g-dev wget \
      bsdmainutils automake curl unzip nano
# pull
git clone https://github.com/MyHush/hush.git
cd hush
# Build
./zcutil/build.sh -j$(nproc)
```

## Download proving key:
```sh
./zcutil/fetch-params.sh
```

## Create a HUSH configuration file (*important*):

You can compile Hush without this, but need a config file to run hushd.

```sh
mkdir -p ~/.hush
echo "rpcuser=username" >> ~/.hush/hush.conf
echo "rpcpassword=`head -c 32 /dev/urandom | base64`" >>~/.hush/hush.conf
echo "addnode=explorer.myhush.org" >> ~/.hush/hush.conf
echo "addnode=dnsseed.myhush.org" >> ~/.hush/hush.conf
echo "addnode=dnsseed2.myhush.org" >> ~/.hush/hush.conf
echo "addnode=dnsseed.bleuzero.com" >> ~/.hush/hush.conf
echo "addnode=dnsseed.hush.quebec" >> ~/.hush/hush.conf
```

## Run a HUSH Node

```sh
./hushd
```

## Windows (cross-compiled on Linux)
Get dependencies:
```ssh
sudo apt-get install \
      build-essential pkg-config libc6-dev m4 g++-multilib \
      autoconf libtool ncurses-dev unzip git python \
      zlib1g-dev wget bsdmainutils automake mingw-w64 cmake
```

Downloading Git source repo, building and running Hush:

```sh
# pull
git clone https://github.com/MyHush/hush.git
cd hush
# fetch key
./zcutil/fetch-params.sh
# Build
./zcutil/build-win.sh -j$(nproc)
# Run a HUSH node
./src/hushd
```

## Mac
Install Xcode CLI tools:

```sh
xcode-select --install
```
Downloading Git source repo, building and running Hush:

```sh
# pull
git clone -b v2.0.0 https://github.com/FireMartZ/hush-apple.git
cd hush-apple
source environment
make
# Run a HUSH node
./src/hushd
```

Make sure to create a HUSH configuration file as described above.

## Supported Platforms

Currently Linux,  Windows and Mac OS X are our supported platforms. If you
are interested in porting Hush to a new operating system, we are interested
to talk with you.

We aim to support all platforms, but there is only so much time in the day.

Currently, any ARMv7 machine will not be able to build this repo, because the
underlying tech (zcash and the zksnark library) do not support that instruction
set.

This also means that RaspberryPi devices will not work, unless they have a
newer ARMv8-based Raspberry Pi.
