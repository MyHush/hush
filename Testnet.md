HUSH 1.0.9 - Testnet
=============

What is a Testnet?
--------------

![Logo](doc/hush/hush.png "Logo")

This software is the HUSH node and command-line Testnet client. It provides
a safe place - off the main network - for testing of new features,
applications and ideas without compromising the security of the main network.

How is running the Testnet different to running a Mainnet node?
--------------

In essence their is very little difference in running a Testnet to a Mainnet node
it only requires one of many possible changes in how you start the node - we will
look at just a few.

However - it is important that before you choose which method to use, you consider
if you will be ONLY running a Testnet node, or if you will be running one alongside
a Mainnet node on the same computer.

### To run a Testnet node on it's own (the simplest way) once only.
Make sure there are no other instances of Hush running:
```
cd hush
./src/hush-cli stop
```
Start the Testnet:
```
./src/hushd -testnet
```
This will give you a running node on testnet - and your node will verify testnet
transactions

### To run a Testnet node on it's own every time Hush is started.
For this we need to edit the hush.conf configuration file.

Using your favourite text editor add the following line to the hush.conf file:
```
testnet=1
```
Exit your text editor and start the node with:
```
./src/hushd
```
as usual. Your node will start on the Testnet and be validating Testnet transactions

### I want to run the Testnet AND mine some Testnet coins!
Thats great!! and you will be helping to keep the testnet alive and well - and supporting
all the testing that developers are trying out out on the chain.

But please remember - this is not competitive mining, we don't need a rig full of GPU's
running to keep the Testnet generating blocks and coins - just one CPU core will do!
Keeping the hasrate low will ensure that everybody can mine a few blocks and do some testing.

Please also note - many VPS suppliers don't like users pegging a CPU core at 100% permanently
you may want to run this from home in a VM or on a server you own.

This time we will make some more detailed changes to the hush.conf file (these will need
removing before using this datadir for a Mainnet node)

Using your favourite text editor add the following lines to the hush.conf file:
```
testnet=1
gen=1
genproclimit=1
equihashsolver=tromp
mineraddress=
```
Don't forget to add in one of your testnet transparent addresses (an empty one)
on the last line there - it will keep all your mined coins on one address ready for
shielding.

### What if I want to run Mainnet and Testnet at the same time?
Not recommended - but if you must.

The simple way is to use a common hush.conf file and data directory - but then you
can't mine as you can't tell one to mine and one not to mine using the same config
But you will still need to talk to both nodes.

This is slightly more complex - as you will need to tell the client (hush-cli) which
node you want to talk to when you ask it questions.

By default hush-cli will talk to the Mainnet node if one / both are running

By default hush-cli will talk to the Testnet node if ONLY the testnet is running

First start the Mainnet:
```
./src/hushd
```

Then start the Testnet
```
./src/hushd -testnet
```

Now We need a way to tell hush-cli to talk to the Testnet node instead of the Mainnet
node if both are running.
We can do this by calling the correct Testnet RPC port on the command line:
```
hush-cli -rpcport=18822 getinfo
```
this should show the Testnet node details and Testnet should be listed.

### I REALLY want to run Testnet and Mainnet AND mine on the Testnet
OK - Then I assume you know what you are doing on Linux, some basic
commands and how to troubleshoot when things go wrong.

For this, no changes to the Mainnet hush.conf are needed - they can all be removed
and put back to default.

- Copy your ```/.hush``` data directory to ```/.hush-t``` or a suitably named alternative
- Make the changes to the new ```/.hush-t/hush.conf``` file as before:
```
testnet=1
gen=1
genproclimit=1
equihashsolver=tromp
mineraddress=
```
- Run this from the usual hush directory (adjust for your paths):
```
./src/hushd -datadir=/home/username/.hush-t
```
- OR make a nice bash script in ```/usr/bin/```
```
#!/bin/bash
~/hush/src/hushd -datadir=/home/username/.hush-t $1 $2 $3
```
Remember that to talk to this Testnet node - you will still need to use the
```./src/hush-cli -rpcport=18822``` style command, or of course you could also
make other scripts and nice bits to customise the system the way you want it.
Or use any combination of the methods outlined above.

Happy hashing!

### 

Building
--------
Building a Testnet node is no different from building a mainnet node - it
is built into the mainnet node software, which builds as follows:

The following build process generally applies to Ubuntu (and similar) Linux
distributions. For best results it is recommended to use Ubuntu Linux 16.04
or later.
Build HUSH along with most dependencies from source by running
Get dependencies:
```{r, engine='bash'}

sudo apt-get install \
      build-essential pkg-config libc6-dev m4 g++-multilib \
      autoconf libtool ncurses-dev unzip git python \
      zlib1g-dev wget bsdmainutils automake
```

Create a HUSH configuration file (*important*):
```
mkdir -p ~/.hush
echo "rpcuser=username" >> ~/.hush/hush.conf
echo "rpcpassword=`head -c 32 /dev/urandom | base64`" >>~/.hush/hush.conf
```

Install HUSH
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

Currently only Linux is officially supported. Windows/Mac OS X versions are in the works...

Deprecation Policy
------------------

This release is considered deprecated 16 weeks after the release day. There
is an automatic deprecation shutdown feature which will halt the node some
time after this 16 week time period. The automatic feature is based on block
height and can be explicitly disabled.

License
-------

For license information see the file [COPYING](COPYING).
