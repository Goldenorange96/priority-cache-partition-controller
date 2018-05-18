#! /bin/bash

 
sudo echo $1 > /sys/fs/cgroup/palloc/unprio/cgroup.procs
