#! /bin/bash

sudo echo $1 > /sys/fs/cgroup/cpuset/part1/cgroup.procs

sudo echo $1 > /sys/fs/cgroup/palloc/part1/cgroup.procs
