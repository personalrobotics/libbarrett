# libbarrett - Barrett Technology #

Libbarrett is a real-time controls library written in C++ that runs [Barrett
Technology](http://www.barrett.com/robot/index.htm)'s products, including the
[WAM Arm](http://www.barrett.com/robot/products-arm.htm) and the [BH8-280
BarrettHand](http://www.barrett.com/robot/products-hand.htm). For support,
please email: support@barrett.com

## Build Instructions ##

To build and install libbarrett, run:

```bash
cmake .
make
sudo make install
```

Headers and shared libraries will be installed to their typical locations for
your system. Configuration files will be installed to the `/etc/barrett/`
directory. A copy of the `examples/` directory will be placed in your home
folder.

Additional Makefile targets include:

```bash
make install_config      # Update or install configuration files only
make package             # Package the library as a tar-ball
```

## Dependencies ##

For additional dependencies, see [this page](http://web.barrett.com/svn/libbarrett/dependencies/).

### Ubuntu 12.04 ###

Install system dependencies:

```bash
apt-get install libeigen2-dev
```

The version of libconfig++ shipped with Ubuntu 12.04 is too old to be used by
libbarrett. Additionally, libbarrett requires a modified version of libconfig++
that adds a public `getCSettings` method.

We will build this patched version of libconfig from source:

```bash
wget http://web.barrett.com/svn/libbarrett/dependencies/libconfig-1.4.5-PATCHED.tar.gz
tar xzf libconfig-1.4.5-PATCHED.tar.gz
cd libconfig-1.4.5
debuild -i -us -uc -b
```

This will generate a collection of `.deb` files in teh parent directory. You
should install using the following command:

```bash
sudo dpkg -i ../libconfig*.deb
```

### Ubuntu 14.04 ###

Install system dependencies:

```bash
apt-get install libconfig-dev libconfig++-dev libeigen2-dev libgsl0-dev
libboost-dev libboost-thread-dev libboost-system-dev libboost-python-dev # Ubuntu 14.04
```

Libbarrett requires a modified version of libconfig++ that adds a public
`getCSettings` method. You need to modify `/usr/include/libconfig.h++` by
adding this code to the `public:` block:

```c++
// BARRETT(DC): Added this inline method to allow simultaneous libconfig and
//              libconfig++ use.
config_setting_t *getCSetting() const { return(_setting); }
```

## Documentation ##

To get started, look through libbarrett's example code. For additional
documentation, see:

- [Product support site](http://support.barrett.com/)
- [File download area](http://barrett.com/robot/support.htm)
- [API documentation](http://web.barrett.com/libbarrett/)

It is possible to use CMake to generate several output formats, including
Eclipse CDT4 project files. For details, see [this
page](http://www.paraview.org/Wiki/Eclipse_CDT4_Generator)

To generate Eclipse project files, run:

```bash
cmake . -G"Eclipse CDT4 - Unix Makefiles"
```

Then import the generated project into your Eclipse workspace using:

> File -> Import -> General -> Existing Projects into Workspace

In order to use libbarrett as a hard real time library, you must have the
[Xenomai](http://www.xenomai.org/) real time co-kernel and its RTSocketCAN
driver installed on your system. If a hard real time guarantee is not important
for your application, you may use the SocketCAN driver from the standard Linux
kernel. To use the Linux SocketCAN driver, add `-DNON_REALTIME=true` to your
cmake command.

## License ##

This version of libbarrett is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by the
Free Software Foundation.

## Contact Information ##

```
support@barrett.com
http://www.barrett.com/
+1-617-252-9000
```

```
Barrett Technology
625 Mount Auburn Street
Cambridge, MA 02138
USA
```
