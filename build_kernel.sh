#!/bin/bash

#script to build kernel
#echo "Script running to compile the kernel"
#to clone linux kernel source code from the git hub repo

cd linux

#git clone https://github.com/torvalds/linux

#to update dependencies
#sudo apt-get install git fakeroot build-essential ncurses-dev xz-utils libssl-dev bc
make defconfig 
make
sudo make modules_install  
sudo make install

echo " Configuring grub for booting kernel "

echo " comment out GRUB_HIDDEN_TIMEOUT and GRUB_HIDDEN_TIMEOUT_QUIET "

#sudo vim /etc/default/grub

#sudo grub-mkconfig -o /boot/grub/grub.cfg

echo " to check kernels present "

ls -lha /boot

echo "reboot to work with new kernel"
