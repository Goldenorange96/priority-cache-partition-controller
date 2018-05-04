#! /bin/bash

 
sudo echo $1 > /sys/fs/cgroup/palloc/oprio/cgroup.procs
