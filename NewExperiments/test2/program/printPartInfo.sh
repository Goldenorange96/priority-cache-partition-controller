#! /bin/bash


cd /sys/fs/cgroup/

echo "part1"
echo "procs:"
sudo cat cpuset/part1/cgroup.procs
echo "Cpuset CPU:"
sudo cat cpuset/part1/cpuset.cpus
echo "palloc procs"
sudo cat palloc/part1/cgroup.procs
echo "palloc bins"
sudo cat palloc/part1/palloc.bins

