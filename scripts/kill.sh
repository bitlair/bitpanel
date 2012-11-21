for x in `cat /root/bitpanel.pid`; do kill -9 $x; done
fuser -k /dev/ttyAMA0
