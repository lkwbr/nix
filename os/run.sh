# Put cwd in path (to run KC's stuff)
PATH=$PATH:.

# Refresh vdisk, in case we trashed it
cp vdisk_fresh vdisk

# Place programs in OS: 
# init, login, sh, cat, more, grep, l2u, cp
mku init
if [[ ! -f a.out ]] ; then exit; fi
rm *.o a.out
mku login
if [[ ! -f a.out ]] ; then exit; fi
rm *.o a.out
mku sh
if [[ ! -f a.out ]] ; then exit; fi
rm *.o a.out
mku cat
if [[ ! -f a.out ]] ; then exit; fi
rm *.o a.out
mku cp
if [[ ! -f a.out ]] ; then exit; fi
rm *.o a.out
mku more
if [[ ! -f a.out ]] ; then exit; fi
rm *.o a.out
mku grep
if [[ ! -f a.out ]] ; then exit; fi
rm *.o a.out
mku l2u
if [[ ! -f a.out ]] ; then exit; fi
rm *.o a.out

# Detect current tty
A=`tty`
SFI=$IFS; IFS=/
set -f
B=($A)
TTY=${B[3]}
echo Current terminal tty = $TTY

# Run operating system (via virtualization)
# NOTE: Serial ports are hard-coded
# TODO: See what -parallel means 
qemu-system-i386 -hda vdisk -m 520M -smp $TTY -serial /dev/pts/1 -serial /dev/pts/2 -parallel none -localtime #-cdrom MTXinstallCD.iso #-boot d

# Cleanup
rm vdisk
