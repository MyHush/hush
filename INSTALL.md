# Building Hush

See the Zcash github wiki (https://github.com/zcash/zcash/wiki) for
instructions on building zcashd, the intended-for-services,
no-graphical-interface, reference implementation of Zcash.

## In progress

```
git clone https://github.com/MyHush/hush.git
cd hush
./zcutil/build.sh
```

## Supported Platforms

We aim to support all platforms, but there is only so much time in the day.

Currently, any ARMv7 machine will not be able to build this repo, because the underlying tech (zcash and the zksnark library) do not support that instruction set.

This also means that RaspberryPi devices will not work, unless they have a newer ARMv8
