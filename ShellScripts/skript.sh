#! /bin/bash

#Max no. bins: 256

sudo echo 0x00000000003FC0 > /sys/kernel/debug/palloc/palloc_mask
cd /sys/fs/cgroup/palloc/
echo "Creating partitions for palloc"
sudo mkdir "part1" "part2" "part3" "part4"
cd /sys/fs/cgroup/cpuset
echo "Creating partitions for cpusets"
sudo mkdir "part1" "part2" "part3" "part4"
counter=1
while [ $counter -le $2 ]
do
	sudo echo 0 > /sys/fs/cgroup/cpuset/part1/cpuset.mems
sudo echo 0 > /sys/fs/cgroup/cpuset/part1/cpuset.cpus
sudo echo $$ > /sys/fs/cgroup/cpuset/part1/tasks
sudo echo $1 > /sys/fs/cgroup/palloc/part1/palloc.bins
sudo echo $$ > /sys/fs/cgroup/palloc/part1/tasks
done
echo "part1 on cpusets and palloc"
sudo echo 0 > /sys/fs/cgroup/cpuset/part1/cpuset.mems
sudo echo 0 > /sys/fs/cgroup/cpuset/part1/cpuset.cpus
sudo echo $$ > /sys/fs/cgroup/cpuset/part1/tasks
sudo echo $1 > /sys/fs/cgroup/palloc/part1/palloc.bins
sudo echo $$ > /sys/fs/cgroup/palloc/part1/tasks

cat /sys/kernel/debug/palloc/palloc_mask
echo "Enable palloc"
sudo echo 1 > /sys/kernel/debug/palloc/use_palloc

echo "bins part1"
cat /sys/fs/cgroup/palloc/part1/palloc.bins



