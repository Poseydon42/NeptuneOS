#!/bin/sh

qemu-system-x86_64 \
	-drive format=raw,file=disk.img \
	-m 1024 \
	-s -S -d int
