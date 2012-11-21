#!/bin/bash

echo 17 > /sys/class/gpio/export 
echo "1" > /sys/class/gpio/gpio17/value
echo "out" > /sys/class/gpio/gpio17/direction 

#stty -F /dev/ttyAMA0 500000
stty -F /dev/ttyAMA0 406:0:18b5:8a30:3:1c:7f:8:4:2:64:0:11:13:1a:0:12:f:17:16:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0
echo $$ > /root/bitpanel.pid
#cat /dev/ttyAMA0 > /dev/null &
echo $! >> /root/bitpanel.pid
sleep 2
echo -ne ":\xaa\xaa\xaa" > /dev/ttyAMA0 && dd if=/dev/zero of=/dev/ttyAMA0 bs=1500 count=1 
sleep 2
echo -ne ":\x00\x4c" >/dev/ttyAMA0
hostname -I >/dev/ttyAMA0;
echo -ne "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" > /dev/ttyAMA0
echo -ne "\x01\x01\x01\x02\x01\x01\x01\x02\x01\x01\x02\x01\x01\x01" > /dev/ttyAMA0
echo -ne "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" > /dev/ttyAMA0
echo -ne "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" > /dev/ttyAMA0
echo -ne "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" > /dev/ttyAMA0
while true; do stty -F /dev/ttyAMA0 500000 ; netcat -l 1337 -d > /dev/ttyAMA0; done
