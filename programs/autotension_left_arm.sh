#!/bin/bash


FILE=/home/herb_admin/.barrett
if [ -e "$FILE" ]; then
    rm ~/.barrett 
fi

ln -s /etc/barrett/config_herblaunch_leftarm/ ~/.barrett &&

# Run the script
./programs/bt-wam-autotension -a left -j -1
