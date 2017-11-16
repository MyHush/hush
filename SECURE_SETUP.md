# Configuring Hush for secure peer-to-peer connections

## Connection settings: 
 
### Default (Mixed TLS + regular connections):
This option is on by default and enables only minimal privacy since a large portion of peer-to-peer connections will likely still be regular connections. No additional actions are required to enable this option.

### Basic Privacy and Security (Forced TLS connections + no certificate verification):
This option enables basic privacy since all peer-to-peer connections are forced over TLS, however, certificates for peers are not validated. 
To enable this option turn on the following flag in your `hush.conf` file:
1. `tlsforce=1`

### Highest Privacy and Security (Forced TLS connections + certificate verification):
This option enables the highest privacy + security by forcing TLS connections and validating peer certificates. 
To enable this option turn on the following two flags in your `hush.conf` file:
1. `tlsforce=1`
2. `tlsvalidate=1`

## Standing up your own "secure" node with a valid certificate:
This guide requires having your own domain. You can still use all of the above connection options without setting up your own secure node, however, peers using the `tlsvalidate=1` flag will not connect to your node.

### Install a free certificate from LetsEncrpt and use it with Hush:
1. Create an A record pointing to the IP address of your node on the DNS control panel for your domain. You can set the `host` entry to anything you like such as `hnode`. This A record should then be reachable as `host.mydomain.com` (we will refer to this as your `FQDN` from now on). 

2. Ensure your domain name as propagated and matches the public IP address of your node by pinging your `FQDN`
    * `ping FQDN`
3. Install the acme script to create a certificate:
    * `sudo apt install socat`
    * `cd ~/`
    * `git clone https://github.com/Neilpang/acme.sh.git`
    * `cd acme.sh`
    * `./acme.sh --install`
4. Create the certificate:
    * `MY_FQDN=FQDN` 
      * Where `FQDN` is the full `host.mydomain.com`
    * `echo $MY_FQDN`
    * `sudo ~/.acme.sh/acme.sh --issue --standalone -d $MY_FQDN`
    * Make a note of where your certificates are:
      * They should be in `~/.acme.sh/FQDN`
5. Add a crontab to check the script expiration date and renew if necessary:
    * `sudo crontab -e`
    * `6 0 * * * "/home/<USER>/.acme.sh"/acme.sh --cron --home "/home/<USER>/.acme.sh" > /dev/null`
      * Where `<USER>` is your username
6. Add the intermediate authority certificate to the certificate store and install it:
    *  `MY_FQDN=FQDN`
    *  `sudo cp /home/$USER/.acme.sh/$MY_FQDN/ca.cer /usr/share/ca-certificates/ca.crt`
    *  `sudo dpkg-reconfigure ca-certificates`
       * Use `space` to select, and `tab \ enter` to navigate.
7. Stop the `hushd` (if running) and add the certificate flags to `hush.conf`
   * `MY_FQDN=FQDN`
   * `cat <<EOF >> ~/.hush/hush.conf tlscertpath=/home/$USER/.acme.sh/$MY_FQDN/$MY_FQDN.cer tlskeypath=/home/$USER/.acme.sh/$MY_FQDN/$MY_FQDN.key EOF`
8. Restart `hushd` and verify you have a valid certificate!
   * `hush-cli getnetworkinfo`
     * You should see a line reading `“tls_cert_verified”: true`


License
-------

For license information see the file [COPYING](COPYING).