#!/bin/sh
dir_path=$(dirname $(realpath $0))

gdb ${dir_path}/../Build/System/Kernel/Kernel.bin -x ${dir_path}/gdb.cfg -tui

