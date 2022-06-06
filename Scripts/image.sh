#!/bin/sh

SCRIPT_DIR=$(dirname $0)

DISK_SIZE=128 # In MiB
GRUB_PARTITION_SIZE=7 # In MiB

OUTPUT_FILE=disk.img
GRUB_PARTITION_START=1
GRUB_PARTITION_END=$(($GRUB_PARTITION_START+$GRUB_PARTITION_SIZE))
MOUNT_DIRECTORY=mnt

# Create empty disk image
dd if=/dev/zero of=$OUTPUT_FILE count=$DISK_SIZE bs=1048576

# Set up loops for created partitions
LOOPBACK=$(sudo losetup --find --partscan --show $OUTPUT_FILE)

# Create partitions
sudo parted --script $LOOPBACK mklabel gpt mkpart BIOSBOOT ext2 ${GRUB_PARTITION_START}MiB ${GRUB_PARTITION_END}MiB mkpart OS ext2 ${GRUB_PARTITION_END}MiB $(($DISK_SIZE - 1))MiB set 1 bios_grub

# Clear OS partition
sudo dd if=/dev/zero of="${LOOPBACK}p2" bs=1M count=1

# Set up ext2 filesystem on root partiton loop device
sudo mke2fs -q "${LOOPBACK}p2"

# Mount root filesystem
mkdir -p $MOUNT_DIRECTORY
sudo mount "${LOOPBACK}p2" $MOUNT_DIRECTORY

# Install grub
# sudo mkdir -p $MOUNT_DIRECTORY/boot/grub/i386-pc
sudo grub-install --boot-directory=$MOUNT_DIRECTORY/boot --target=i386-pc --modules="part_gpt ext2" $LOOPBACK
sudo cp $SCRIPT_DIR/grub.cfg $MOUNT_DIRECTORY/boot/grub/grub.cfg
sudo cp Kernel/Kernel.bin $MOUNT_DIRECTORY/kernel.bin

# Flush
sync

# Clear after yourself
sudo umount $MOUNT_DIRECTORY
sudo rmdir $MOUNT_DIRECTORY
sudo losetup -d $LOOPBACK

