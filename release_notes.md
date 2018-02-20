Release notes for Hush 1.0.13

- Includes all bug fixes from zcash 1.0.13 (upstream)
- Fixes network sync issue.
- Adds new DNS seed nodes help nodes connect and find other nodes faster.
- Implements TLS options - allows options for self-signed or CA-signed TLS. Options can also be set to require TLS and/or CA-signed certs.
- TLS options have been tested and work on Windows, Mac, and Linux.
- TLS setup instructions are at: https://github.com/MyHush/hush/blob/master/SECURE_SETUP.md 
- Large memory reduction when processing shielded transactions.
- Enabled new RPC methods from zcash upstream: z-shieldcoinbase z_getpaymentdisclosure z_validatepaymentdisclosure.
- Adds ability to compile on Mac (experimental).
- Hush continues to be supported by BarterDEX and the Agama Wallet.
