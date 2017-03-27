# zdash-gui
gui-wallet for zdash

Installing and running the Wallet GUI

Before running the Desktop GUI Wallet you need to have Zdash installed up and running in linux. 

1 - download the binary zdash.jar

Operating system and tools:


The Linux tools you need to run the Wallet GUI, Java (JDK7 or later) and Ant. If using Ubuntu Linux, they may be installed via command:

>> sudo apt-get install git default-jdk ant

For RedHat/CentOS/Fedora-type Linux systems the command is (like):

>> sudo yum install java-1.8.0-openjdk git ant 

The name of the JDK package (java-1.8.0-openjdk) may vary depending on the Linux system, so you need to check it, if name java-1.8.0-openjdk is not accepted. If you have some other Linux distribution, please check your relevant documentation on installing Git, JDK and Ant. The commands git, java, javac and ant need to be startable from command line before proceeding with build.

2- Place the file zdash.jar in zdash/src and make it executable:

>> chmod u+x ./zdash/src/zdash.jar

* zdash.jar must be in the same folder with the daemon and cli-wallet.
* the daemon must be running before gui wallet is started ( zcashd --daemon )


Running the Zdash GUI wallet:

Before running the GUI you need to start zcashd (e.g. zcashd --daemon). The wallet GUI is a Java program packaged as an executable JAR file. Run from command line. Assuming you have already installed Zdash and the GUI Wallet zdash.jar in directory /home/user/zdash/src run it from command line:

>> java -jar /home/user/zdash/src/dash.jar

The GUI wallet is simple to use and navigate. Test with small transctions at first.
=======================================================================================

zdash GUI wallet is based on:

https://github.com/ondrejsika/zcash-swing-wallet-ui


License

This program is distributed under an MIT License.

Disclaimer

This program is not officially endorsed by or associated with the ZCash project and the ZCash company.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Known issues and limitations

1- Limitation: Wallet encryption has been temporarily disabled in ZCash due to stability problems. A corresponding issue #1552 has been opened by the ZCash developers. Correspondingly wallet encryption has been temporarily disabled in the ZCash Desktop GUI Wallet.

2- Issue: the GUI wallet does not work correctly if zcashd is started with a custom data directory, like: zcashd -datadir=/home/data/whatever This will be fixed in later versions.

3- Issue: GUI data tables (transactions/addresses etc.) allow copying of data via double click but also allow editing. The latter needs to be disabled.

4- Limitation: The list of transactions does not show all outgoing ones (specifically outgoing Z address transactions). A corresponding issue #1438 has been opened for the ZCash developers - soon to be fixed. A fix for the GUI wallet may be expected within 1-2 weeks.

5- Limitation: The CPU percentage shown to be taken by zcashd is the average for the entire lifetime of the process. This is not very useful. This will be improved in future versions.
