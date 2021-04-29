# Refresh vdisk
cp vdisk_fresh vdisk

# Detect current tty
A=`tty`
SFI=$IFS; IFS=/
set -f
B=($A)
TTY=${B[3]}

# Run operating system 
qemu-system-i386 -hda vdisk -m 520M -smp $TTY -serial /dev/pts/1 -serial /dev/pts/2 -parallel none -localtime 
