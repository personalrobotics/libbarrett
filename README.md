# Libbarrett 3.0.0
> Libbarrett is a real-time controls library written in C++ that runs Barrett
Technology products (WAM and BarrettHand)

` This version of Libbarrett works with a non-real-time kernel (a low-latency Ubuntu 22.04 kernel) and should only be used when a hard real-time guarantee is not critical for your application.`


### General

Disable unattended upgrades

```
apt remove unattended-upgrades
```

### Download package
```
cd && git clone https://github.com/personalrobotics/libbarrett
```

### Install dependencies
```
cd ~/libbarrett/scripts && ~/libbarrett/scripts/install_dependencies.sh
```

### Reboot into the new kernel (after reboot, "uname -r" should show "lowlatency"): 
```
sudo reboot
```

### Build and install the Peak pcan driver
We recommend the following CAN hardware:

 - PCAN-PCI
 - PCAN-PCIe
 - PCAN-USB
 - PCAN-ISA 

NOTE: These cards are supported by the SocketCAN driver built into Linux, however, we have found that the SocketCAN txbuffer can occasionally fail to send CAN frames without reporting an error, and this causes the WAM to E-Stop itself unexpectedly. This can happen a few seconds, a few minutes, or even several hours after starting a WAM application. **Fortunately, Peak offers an alternative driver (pcan) which is still socket-based but avoids this SocketCAN bug.**

You can install that PCAN driver here:
```
sh ~/libbarrett/scripts/install_pcan.sh
```

### For PCAN-ISA only, manually configure the driver (not plug-and-play): 
```
sudo tee /etc/modprobe.d/pcan.conf <<EOF
options pcan type=isa,isa io=0x300,0x320 irq=7,5
install pcan modprobe --ignore-install pcan
EOF
echo 'pcan' |sudo tee -a /etc/modules-load.d/modules.conf
```

### Reboot to use the new CAN driver (after reboot, both "cat /proc/pcan" and "ifconfig" should list can0): 
```
sudo reboot
```
You may need to run `reset_can.sh` before `can0` appears in `ifconfig`.

### Build libbarrett
```
cd ~/libbarrett && cmake .
make -j$(nproc)
```

### Install libbarrett
```
sudo make install
```

### Build the libbarrett example programs
```
cd ~/libbarrett/examples && cmake .
make -j$(nproc)
```

### Additional Makefile targets
Optional: Update or install configuration files only - not necessary if you have already done a full make (above)
```
make install_config
```

Optional: Package the library as a tar-ball (not common)
```
make package
```

### Configuration Files for the WAM
Upon installation of libbarrett, the configuration files of the robot are installed to the `/etc/barrett` directory. However, to give an additional flexibility of each user maintaining their own configurations for the same robot, by default, the configuration files are read from `~/.barrett` directory if it exists. If not, then libbarrett reads the necessary configuration files from `/etc/barrett/` directory. It is up to the user to maintain and populate the `~/.barrett` directory.

### Eclipse IDE setup
It is possible to use CMake to generate several output formats, including Eclipse CDT4 project files. For details, see:
    http://www.paraview.org/Wiki/Eclipse_CDT4_Generator
To generate Eclipse project files, run:
```
cmake . -G"Eclipse CDT4 - Unix Makefiles"
```
Then import the generated project into your Eclipse workspace using:
File -> Import -> General -> Existing Projects into Workspace

## BURT Util

`burt-util` is a BURT firmware management Utility with commands
that allow you to easily upgrade firmware, flash a bootloader,
connect to the firmware via CAN and CoAP, and much more.

### Installing burl-util

Download the utils from https://git.barrett.com/burt/software/burt-util/ and install them. 

```sh
curl 'https://git.barrett.com/burt/software/burt-util/-/jobs/40381/artifacts/download'  --output  burt-util-dev-3.7.3-deb.zip 
unzip burt-util-dev-3.7.3-deb.zip
sudo dpkg -i burt-util_3.7.3_amd64.deb 
```


## Enumerating the pucks on the CAN bus

`burt-util` offers a variety of functions. E.g. enumerating the devices on the
bus. For that to work, you need to specify the ID of the CAN bus in
`.config/barrett/burt_util.toml`.  For example `burt-util enumerate`, should
output

```
Searching for nodes
  [####################################]  100%          
Found: [1, 2, 3, 4, 5, 6, 7]
```


