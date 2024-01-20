# Libbarrett 3.0.0
> Libbarrett is a real-time controls library written in C++ that runs Barrett
Technology products (WAM and BarrettHand)

` This version of Libbarrett works with a non-real-time kernel (a low-latency Ubuntu 20.04 kernel) and should only be used when a hard real-time guarantee is not critical for your application.`


### Download package
```
cd && git clone https://git.barrett.com/software/libbarrett
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

NOTE: These cards are supported by the SocketCAN driver built into Linux, however, we have found that the SocketCAN txbuffer can occasionally fail to send CAN frames without reporting an error, and this causes the WAM to E-Stop itself unexpectedly. This can happen a few seconds, a few minutes, or even several hours after starting a WAM application. Fortunately, Peak offers an alternative driver (pcan) which is still socket-based but avoids this SocketCAN bug.
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

### Build libbarrett (using clang)
```
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
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
