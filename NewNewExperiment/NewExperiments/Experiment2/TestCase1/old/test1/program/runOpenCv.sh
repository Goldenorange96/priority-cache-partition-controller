#! /bin/bash

cd /home/jj/Desktop/testobj1/
sudo nohup gnome-terminal -e "nice --5 ./4cores.sh SURF"

cd /home/jj/Desktop/testobj2/
sudo nohup gnome-terminal -e "nice --5 ./4cores.sh SURF"


