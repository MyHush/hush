### Qos ###

This is a Linux bash script that will set up tc to limit the outgoing bandwidth for connections to the Hush network. It limits outbound TCP traffic with a source or destination port of 8888, but not if the destination IP is within a LAN (defined as 192.168.x.x).

This means one can have an always-on hushd instance running, and another local wallet which connects to this node and receives blocks from it.

###Usage###

Open the script in a text editor. Find the line that says IF="eth0" and change eth0 to reflect the network interface that your internet connection runs through. To get a list of your computer's network interfaces, use ifconfig on the command line. My computer is connected wireless through wlan1, so the IF line of my tc.sh looks like this:

    IF="wlan1"

LINKCEIL should reflect the limit of the network interface, and most likely does not need to be changed.

    LINKCEIL="1gbit"

Change LIMIT to be the maximum bandwidth you want **hushd** to use (I chose 1mbit).

    LIMIT="1mbit"

Exit your editor and make the script executable with the following command:

    chmod +x ./tc.sh

Run the script as root:

    sudo ./tc.sh

Your **hushd** bandwidth will be throttled until you reboot your computer!




