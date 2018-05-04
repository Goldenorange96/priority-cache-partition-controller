#! /bin/bash

cd /sys/fs/cgroup/cpuset
sudo rmdir "part1" "part2" "part3" "part4"

cd /sys/fs/cgroup/palloc
sudo rmdir "part1" "part2" "part3" "part4"

