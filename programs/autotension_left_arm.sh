#!/bin/bash

rm ~/.barrett &&
ln -s /etc/barrett/config_herblaunch_leftarm/ ~/.barrett &&

# Run the script
./programs/bt-wam-autotension -a left -j -1
