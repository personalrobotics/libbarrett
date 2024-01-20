!/usr/bin/env bash
# This script installs libbarrett dependencies

# Import the Linux Version Number Variables
. /etc/lsb-release

if [ "$DISTRIB_RELEASE" = "22.04" ]; then
	# Install PCAN driver
	mydir=$(pwd)
	cd && wget https://www.peak-system.com/fileadmin/media/linux/files/peak-linux-driver-8.17.0.tar.gz
	tar xf peak-linux-driver-8.17.0.tar.gz
	cd peak-linux-driver-8.17.0 && make -j$(nproc) -C driver netdev
	sudo make -C driver install
	cd $mydir
else
	echo "This script must run under Ubuntu 22.04"
fi
