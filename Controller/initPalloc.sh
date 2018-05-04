#! /bin/bash

#Max no. bins: 256

sudo echo 0x00000000003FC0 > /sys/kernel/debug/palloc/palloc_mask
cd /sys/fs/cgroup/palloc/
echo "Creating partitions for palloc"
sudo mkdir "part1" "part2" "part3" "part4" "part5" "part6" "leech" "unprio"
cd /sys/fs/cgroup/cpuset
echo "Creating partitions for cpusets"
sudo mkdir "part1" "part2" "part3" "part4" "part5" "part6" "leech" "unprio"

 for i in `seq 1 6`;
        do
		echo "part"$i"on cpusets and palloc"
		sudo echo 0 > /sys/fs/cgroup/cpuset/part$i/cpuset.mems
		sudo echo 0 > /sys/fs/cgroup/cpuset/part$i/cpuset.cpus

		
        done  

		sudo echo 0 > /sys/fs/cgroup/cpuset/oprio/cpuset.mems
		sudo echo 0 > /sys/fs/cgroup/cpuset/oprio/cpuset.cpus
		sudo echo 192-255 > /sys/fs/cgroup/oprio/palloc.bins

cat /sys/kernel/debug/palloc/palloc_mask
echo "Enable palloc"
sudo echo 1 > /sys/kernel/debug/palloc/use_palloc

echo "bins part1"
cat /sys/fs/cgroup/palloc/part1/palloc.bins



