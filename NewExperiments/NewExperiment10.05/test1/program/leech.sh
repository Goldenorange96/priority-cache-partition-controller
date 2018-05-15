#! /bin/bash

sudo echo $1 > /sys/fs/cgroup/cpuset/part2/cgroup.procs

sudo echo $1 > /sys/fs/cgroup/palloc/part2/cgroup.procs
