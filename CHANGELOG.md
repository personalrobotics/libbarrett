# Change Log
All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/). 

## [Unreleased]

- Added wamudpd script that makes PCs findable by the wamdiscover script.
- Updated wamudpd script to run using python3

## [dev-3.0.1]

- Increased enumeration timeout from 1 ms to 5 ms

## [dev-3.0.0]

- Python3 support
- Clang support
- Updated dependencies (Boost, Eigen, Freeglut)
- Allowed WAM loop to run while reading BHand sensors
- Converted ex10 haptics visualization from vpython to OpenGL
- Based WAM loop on a (100x) more accurate timer_fd instead of sleep()
- More robust installation script
- Improved mutex handling for non-rt operation
- Offered Top10 tactile values from BHand for higher update rates

[dev-3.0.1]: https://git.barrett.com/software/libbarrett/-/tags/dev-3.0.1
[dev-3.0.0]: https://git.barrett.com/software/libbarrett/-/tags/dev-3.0.0

