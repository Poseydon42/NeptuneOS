#!/bin/sh

qemu-system-x86_64 \
	-drive format=raw,file=disk.img \
	-serial stdio \
	-m 1024 \
	-d int \

