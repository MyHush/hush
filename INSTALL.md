# Installing Hush

## Install

    echo 'deb https://dl.bintray.com/myhush/hush/ hush main' | sudo tee --append /etc/apt/sources.list.d/hush.list
    apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 379CE192D401AB61
    sudo apt-get update
    sudo apt-get install hush

## Create a HUSH configuration file (*important*):

```
mkdir -p ~/.hush
echo "rpcuser=username" >> ~/.hush/hush.conf
echo "rpcpassword=`head -c 32 /dev/urandom | base64`" >>~/.hush/hush.conf
echo "addnode=explorer.myhush.org" >> ~/.hush/hush.conf
echo "addnode=dnsseed.myhush.org" >> ~/.hush/hush.conf
echo "addnode=dnsseed2.myhush.org" >> ~/.hush/hush.conf
echo "addnode=dnsseed.bleuzero.com" >> ~/.hush/hush.conf
echo "addnode=dnsseed.hush.quebec" >> ~/.hush/hush.conf
```

## Download proving key

```
hush-fetch-params
```

# Building Hush from source

Building Hush from source will take some time but your efforts will be rewarded :)

## Requirements

You will need at least 4GB of RAM to build hush from git source, OR
you can enable a swap file. To enable a 4GB swap file on modern Linux
distributions:

    sudo fallocate -l 4G /swapfile
    sudo chmod 600 /swapfile
    sudo mkswap /swapfile
    sudo swapon /swapfile

You will need to have Git and a C++ compiler and libtool and a
a few other libraries, depending on your setup.

## Building

The following build process generally applies to Ubuntu (and similar) Linux
distributions. For best results it is recommended to use Ubuntu Linux 16.04
or later.

Build HUSH along with most dependencies from source by running:

## Linux
Get dependencies:
```{r, engine='bash'}

sudo apt-get install \
      build-essential pkg-config libc6-dev m4 g++-multilib \
      autoconf libtool ncurses-dev unzip git python \
      zlib1g-dev wget bsdmainutils automake
```

Downloading Git source repo, building and running Hush:

```{r, engine='bash'}
# pull
git clone https://github.com/MyHush/hush.git
cd hush
# fetch key
./zcutil/fetch-params.sh
# Build
./zcutil/build.sh -j$(nproc)
# Run a HUSH node
./src/hushd
```

## Windows (cross-compiled on Linux)
Get dependencies:
```{r, engine='bash'}
sudo apt-get install \
      build-essential pkg-config libc6-dev m4 g++-multilib \
      autoconf libtool ncurses-dev unzip git python \
      zlib1g-dev wget bsdmainutils automake
```

Downloading Git source repo, building and running Hush:

```{r, engine='bash'}
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
Get dependencies:

```{r, engine='bash'}
# Install xcode
xcode-select --install

# Install brew
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

brew install cmake autoconf libtool automake coreutils pkgconfig gmp wget
brew install gcc5 --without-multilib
```
Downloading Git source repo, building and running Hush:

```{r, engine='bash'}
# pull
git clone https://github.com/MyHush/hush.git
cd hush
# fetch key
./zcutil/fetch-params.sh
# Build
./zcutil/build-mac.sh --disable-rust
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

