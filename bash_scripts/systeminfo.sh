#!/bin/bash
#the script gives information about system
#author : Gautham

echo --------------------------------
echo "User Information"
echo --------------------------------
(whoami ; echo is the user)|tr '\n' '\t'
echo 

echo --------------------------------
echo "Operating System Type"
echo --------------------------------
uname

echo --------------------------------
echo "OS Distribution and Version"
echo --------------------------------
cat /etc/*release

echo --------------------------------
echo "Kernel Version"
echo --------------------------------
uname -r

echo --------------------------------
echo "Kernel gcc version build"
echo --------------------------------
cat /proc/version

echo --------------------------------
echo "kernel build time"
echo --------------------------------
uname -v
#uname -m

echo --------------------------------
echo "System Architecture Information"
echo --------------------------------
lscpu

echo --------------------------------
echo "Information on File system Memory"
echo --------------------------------
free -m
cat /proc/meminfo
