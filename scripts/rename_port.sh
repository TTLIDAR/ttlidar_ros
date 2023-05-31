#!/bin/bash

echo "remap the device serial port(ttyUSBX) to  ttlidar"
echo "ttlidar usb connection as /dev/ttlidar , check it using the command : ls -l /dev|grep ttyUSB"
echo "start copy ttlidar.rules to  /etc/udev/rules.d/"
echo "`rospack find ttlidar_node`/scripts/ttlidar.rules"
sudo cp `rospack find ttlidar_node`/scripts/ttlidar.rules  /etc/udev/rules.d
echo " "
echo "Restarting udev"
echo ""
sudo service udev reload
sudo service udev restart
echo "finish "
