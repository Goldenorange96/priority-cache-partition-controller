#! /bin/bash

 #set mask, bins, cpus, procs


sudo echo 0x00000000003FC0 > /sys/kernel/debug/palloc/palloc_mask

sudo echo 0 > /sys/fs/cgroup/cpuset/part1/cpuset.mems

sudo echo 0 > /sys/fs/cgroup/cpuset/part1/cpuset.cpus

sudo echo 0-255 > /sys/fs/cgroup/palloc/part1/palloc.bins

sudo echo $1 > /sys/fs/cgroup/cpuset/part1/cgroup.procs

sudo echo $1 > /sys/fs/cgroup/palloc/part1/cgroup.procs



#sudo echo 0 > /sys/fs/cgroup/cpuset/part2/cpuset.mems

#sudo echo 1 > /sys/fs/cgroup/cpuset/part2/cpuset.cpus

#sudo echo 0 > /sys/fs/cgroup/palloc/part2/palloc.bins

#sudo echo $2 > /sys/fs/cgroup/cpuset/part2/cgroup.procs

#sudo echo $2 > /sys/fs/cgroup/palloc/part2/cgroup.procs

sudo echo 1 > /sys/kernel/debug/palloc/use_palloc



