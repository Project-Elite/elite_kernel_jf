#!/sbin/busybox sh
#
#Set Max Mhz for GPU
echo 450000000 > /sys/devices/platform/kgsl-3d0.0/kgsl/kgsl-3d0/max_gpuclk

#Set Max Mhz speed and booted flag to set Super Max
echo 1890000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq;
echo 1 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_booted;

# Remount FileSys RW
/sbin/busybox mount -t rootfs -o remount,rw rootfs

## Create the kernel data directory
if [ ! -d /data/.ktoonsez ];
then
  mkdir /data/.ktoonsez
  chmod 777 /data/.ktoonsez
fi

## Enable "post-init" ...
if [ -f /data/.ktoonsez/post-init.log ];
then
  # BackUp old post-init log
  mv /data/.ktoonsez/post-init.log /data/.ktoonsez/post-init.log.BAK
fi

# Start logging
date >/data/.ktoonsez/post-init.log
exec >>/data/.ktoonsez/post-init.log 2>&1

echo "Running Post-Init Script"

## Testing: Check for ExFat SD Card
#
SDTYPE1=`blkid /dev/block/mmcblk1  | awk '{ print $3 }' | sed -e 's|TYPE=||g' -e 's|\"||g'`
SDTYPE2=`blkid /dev/block/mmcblk1p1  | awk '{ print $3 }' | sed -e 's|TYPE=||g' -e 's|\"||g'`

if [ "${SDTYPE1}" == "exfat" ];
then
  echo "ExFat-Debug: SD-Card is type ExFAT"
  echo "ExFat-Debug: trying to mount via fuse"
  mount.exfat-fuse /dev/block/mmcblk1 /extSdCard
else
  echo "ExFat-Debug: SD-Card is type1: ${SDTYPE1}"
fi

if [ "${SDTYPE2}" == "exfat" ];
then
  echo "ExFat-Debug: SD-Card is type ExFAT"
  echo "ExFat-Debug: trying to mount via fuse"
  mount.exfat-fuse /dev/block/mmcblk1p1 /extSdCard
else
  echo "ExFat-Debug: SD-Card is type2: ${SDTYPE2}"
fi
# Remount FileSys RO
/sbin/busybox mount -t rootfs -o remount,ro rootfs

echo $(date) END of post-init.sh
