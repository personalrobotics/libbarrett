#!/usr/bin/env bash
# This script installs libbarrett dependencies

# Import the Linux Version Number Variables
. /etc/lsb-release

if [ "$DISTRIB_RELEASE" = "22.04" ]; then
	# Install CAN scripts
	sudo cp reset_can.sh /bin
	sudo cp 60-can.rules /etc/udev/rules.d
	sudo udevadm control --reload-rules
	sudo udevadm trigger
	
	# Install libbarrett dependencies
	sudo apt update
	sudo apt install -y linux-lowlatency
	sudo apt install -y git cmake build-essential net-tools can-utils
	sudo apt install -y libgsl-dev libeigen3-dev libncurses-dev pkg-config libconfig++-dev
	sudo apt install -y libboost-system-dev libboost-thread-dev libboost-python-dev

	# Pin the new kernel (to avoid recompiling custom modules)
	sudo apt-mark hold linux-lowlatency

else
	echo "This script must run under Ubuntu 22.04"
fi

