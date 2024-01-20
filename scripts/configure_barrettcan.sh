#/bin/bash
# configure_barrettcan.sh <id>

# Wake puck (Get STATUS)
cansend can0 00$1#05
sleep 0.1

# Set ROLE to 256
cansend can0 00$1#81.00.00.01
sleep 0.1

# Set GRPA to 0
cansend can0 00$1#9A.00.00.00
sleep 0.1

# Set GRPC to 4
cansend can0 00$1#9C.00.04.00
sleep 0.1

# Set JIDX to ID
cansend can0 00$1#D5.00.0$1.00
sleep 0.1

if [ $1 -lt 5 ]
then # 4DOF
  # Set GRPB to 1
  cansend can0 00$1#9B.00.01.00
  sleep 0.1

  # Set PIDX to ID
  cansend can0 00$1#C6.00.0$1.00
  sleep 0.1

  # Set IPNM to 2700
  cansend can0 00$1#D6.00.8C.0A
  sleep 0.1
else # Wrist
  # Set GRPB to  2
  cansend can0 00$1#9B.00.02.00
  sleep 0.1

  # set PIDX to ID-4
  cansend can0 00$1#C6.00.0`expr $1 - 4`.00
  sleep 0.1
  if [ $1 -lt 7 ]
  then # Wrist diff
    # Set IPNM to 6500
    cansend can0 00$1#D6.00.64.19
  else # Wrist roll
    # Set IPNM to 17474
    cansend can0 00$1#D6.00.42.44
  fi
  sleep 0.1
fi

# Save all
cansend can0 00$1#9E.00.FF.FF
