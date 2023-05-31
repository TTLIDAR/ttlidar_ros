#!/bin/bash

echo "delete remap the device serial port(ttyUSBX) to  ttlidar"
echo "sudo rm   /etc/udev/rules.d/ttlidar.rules"
sudo rm   /etc/udev/rules.d/ttlidar.rules
echo " "
echo "Restarting udev"
echo ""
sudo service udev reload
sudo service udev restart
echo "finish  delete"
