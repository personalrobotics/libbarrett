#!/bin/bash

rm ~/.barrett &&
ln -s /etc/barrett/config_herblaunch_rightarm/ ~/.barrett &&

# Run the script
./programs/bt-wam-autotension -a right -j 1 2 3 4
