# Installing Hush

## Linux Install with Windows via VirtualBox
VirtualBox 5.2.8 (released February 27 2018)
https://www.virtualbox.org/wiki/Download_Old_Builds_5_2

Or click below for direct download
https://download.virtualbox.org/virtualbox/5.2.8/VirtualBox-5.2.8-121009-Win.exe

Ubuntu Install
Download Ubuntu 16.04.4 LTS (Xenial Xerus) from your favorite mirror, or find it below.
http://www.gtlib.gatech.edu/pub/ubuntu-releases/xenial/

Or click below for direct download
http://www.gtlib.gatech.edu/pub/ubuntu-releases/xenial/ubuntu-16.04.4-desktop-amd64.iso

Set up VirtualBox to install from Ubuntu ISO, 4 Gigs of RAM and 20 GB of storage will work.  

---

## Linux Install with VPS (<$3/month)
Ubuntu 16.04 VPS with CPU: 1 vCore, RAM: 512 MB, Storage: and 20 GB SSD will work.

Create a new username, just replace "CREATE_NEW_USERNAME" with a new name.
```sh
adduser CREATE_NEW_USERNAME && adduser CREATE_NEW_USERNAME sudo
```
Reboot and log in as new user

---

## Update Ubuntu

After installation is complete open terminal and do an update.
```sh
sudo apt-get update && sudo apt-get upgrade -y
```

## Swap Space (Optional)
You will need at least 4GB of RAM to build hush from git source, OR you can
enable a swap file. To enable a 4GB swap file on modern Linux distributions:

```sh
sudo fallocate -l 4G /swapfile
sudo chmod 600 /swapfile
sudo mkswap /swapfile
sudo swapon /swapfile
```

Now make the swap work better. Add a line to sysctl.conf
```sh
sudo nano /etc/sysctl.conf
```
add to last line of file:
```
vm.swappiness=10
```

Then make it so the swap gets mounted when the server reboots. Edit the fstab file
```sh
sudo nano /etc/fstab
```
add to last line of file:
```
/swapfile   none    swap    sw    0   0
```

## Build HUSH dependencies
The following build process generally applies to Ubuntu (and similar) Linux
distributions. For best results it is recommended to use Ubuntu Linux 16.04
or later.

```sh
sudo apt-get install build-essential pkg-config libc6-dev m4 g++-multilib \
      autoconf libtool ncurses-dev unzip git python zlib1g-dev wget \
      bsdmainutils automake curl unzip nano
```

## Download and Install Hush v1.0.13 Stable Release
```sh
cd ~
sudo wget https://github.com/MyHush/hush/releases/download/v1.0.13/hush-1.0.13-afad8af-amd64.deb
sudo dpkg -i hush-1.0.13-afad8af-amd64.deb
```

## Download proving key
```sh
./zcutil/fetch-params.sh
```

## Create a HUSH configuration file (*important*):
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
```ssh
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
Get dependencies:

```sh
# Install xcode
xcode-select --install

# Install brew
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

brew install cmake autoconf libtool automake coreutils pkgconfig gmp wget
brew install gcc5 --without-multilib
```
Downloading Git source repo, building and running Hush:

```sh
# pull
git clone https://github.com/MyHush/hush.git
cd hush
# fetch key
./zcutil/fetch-params.sh
# Build
./zcutil/build-mac.sh
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
