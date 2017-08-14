HUSH 1.0.9
=============

What is HUSH?
--------------

![Logo](doc/hush/hush.png "Logo")

HUSH (formerly Zdash) is a private implementation of the "Zerocash" protocol forked from [ZCash](https://z.cash/).
Based on Bitcoin's code, it intends to offer a far higher standard of privacy
through a sophisticated zero-knowledge proving scheme that preserves
confidentiality of transaction metadata. 

This software is the HUSH node and command-line client. It downloads and stores the entire history
of HUSH transactions; depending on the speed of your computer and network
connection, the synchronization process could take a day or more once the
blockchain has reached a significant size.


**HUSH is unfinished and highly experimental.** Use at your own risk!

TEAM
----

* Axel Karlsson: [@sparkit] Community manager, general communications.
* Iolar Demartini Junior: [@demartini] Web Developer (Front End).
* David Mercer: [@radix42] Lead Developer
* Matthew Reichardt: [@mreichardt] Core Developer, Tooling
* Duke Leto: [@leto] Core Developer, Yak Shaver
* Larry Ludlow [@madbuda] Core Developer, Infrastructure 

* You can join our team too and make contributions

Slack
-----

Please feel free to join our Slack at https://slack.myhush.org/ .
There are many channels, some you might enjoy are #general, #exchanges and #mining.

Forum
-----
https://forum.myhush.org/

Installing
----------

See INSTALL.md.

Deprecation Policy
------------------

This release is considered deprecated 16 weeks after the release day. There
is an automatic deprecation shutdown feature which will halt the node some
time after this 16 week time period. The automatic feature is based on block
height and can be explicitly disabled.

Where do I begin?
-----------------
As a reference a guide for joining the main Zcash network may be used:
https://github.com/zcash/zcash/wiki/1.0-User-Guide#using-zcash
Users should *not* follow this guide blindly since it applies to ZCash instead of HUSH!
The section of using the command line is relevant to HUSH.

### Need Help?

* See the documentation at the [Zcash Wiki](https://github.com/zcash/zcash/wiki)
  for help and more general information.

### Not content with command line use of HUSH?

A [HUSH GUI Wallet](https://github.com/vaklinov/hush-swing-wallet-ui) exists and is maintained by an independent developer as a separate project. Please read about the known issues and limitations before using the wallet.

License
-------

For license information see the file [COPYING](COPYING).
