#!/bin/bash

FILE=/home/herb_admin/.barrett
if [ -e "$FILE" ]; then
    rm ~/.barrett 
fi

ln -s /etc/barrett/config_herblaunch_rightarm/ ~/.barrett &&

# Run the script
./bt-wam-zerocal