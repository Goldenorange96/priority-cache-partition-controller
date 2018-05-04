#! /bin/bash

sudo echo 0x00000000003FC0 > /sys/kernel/debug/palloc/palloc_mask
cd /sys/fs/cgroup/palloc/
echo "Creating partitions for palloc"
sudo mkdir "part1" 
cd /sys/fs/cgroup/cpuset
echo "Creating partitions for cpusets"
sudo mkdir "part1"
	
echo "part1 on cpusets and palloc"
sudo echo 0 > /sys/fs/cgroup/cpuset/part1/cpuset.mems
sudo echo 0 > /sys/fs/cgroup/cpuset/part1/cpuset.cpus
sudo echo $$ > /sys/fs/cgroup/cpuset/part1/tasks
sudo echo $1 > /sys/fs/cgroup/palloc/part1/palloc.bins
sudo echo $$ > /sys/fs/cgroup/palloc/part1/tasks

sudo echo 1 > /sys/kernel/debug/palloc/use_palloc


./avg_op_per_sec_long_name -c 1 -C 0 -m 10000000 -o 1
