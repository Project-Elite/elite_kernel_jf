#!/sbin/busybox sh
#
#Set Max Mhz for GPU
echo 400000000 > /sys/devices/platform/kgsl-3d0.0/kgsl/kgsl-3d0/max_gpuclk

#Set Max Mhz speed and booted flag to set Super Max
echo 1890000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq;
echo 1 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_booted;

echo $(date) END of post-init.sh
