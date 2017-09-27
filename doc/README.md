ArtByte Core 0.13.0
=====================

Setup
---------------------
[ArtByte Core](https://www.artbyte.me/get-started/) is the original ArtByte client and it builds the backbone of the network. However, it downloads and stores the entire history of ArtByte transactions (which is currently several GBs); depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more.

Running
---------------------
The following are some helpful notes on how to run ArtByte on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/artbyte-qt` (GUI) or
- `bin/artbyted` (headless)

### Windows

Unpack the files into a directory, and then run artbyte-qt.exe.

### OS X

Drag ArtByte-Core to your applications folder, and then run ArtByte-Core.

### Need Help?

* See the documentation at the [ArtByte Forum](http://forum.artbyte.me)
for help and more information.
* Ask for help on the [BitcoinTalk](https://bitcointalk.org/) forums, in the [Technical Support board](https://bitcointalk.org/index.php?topic=503131.0).

Building
---------------------
The following are developer notes on how to build ArtByte on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [OS X Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- [OpenBSD Build Notes](build-openbsd.md)

License
---------------------
Distributed under the [MIT software license](http://www.opensource.org/licenses/mit-license.php).
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.
