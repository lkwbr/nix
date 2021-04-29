## SEE INITIAL FILESYSTEM
#echo "========================================================"
#echo "<< FILESYSTEM BEFORE >>"
#echo "========================================================"
# Diskimage refresh
rm diskimage
cp ../Lab\ 6/diskimage .

# Diskimage contents (before)
sudo mount diskimage /mnt
ls -liR /mnt | cat > lsFsBefore
sudo umount /mnt


## RUN PROGRAM
echo "========================================================"
echo "<< RUN PROGRAM >>"
echo "========================================================"

# Compile
gcc -w main.c -o emulator

# Run 
./emulator

# Remove
rm emulator


## FILESYSTEM AFTER PROGRAM RUNS
#echo "========================================================"
#echo "<< FILESYSTEM AFTER >>"
#echo "========================================================"
# Diskimage contents (after)
sudo mount diskimage /mnt
ls -liR /mnt | cat > lsFsAfter
sudo umount /mnt

#mkfs -b 1024 diskimage 1440  


## SEE FILESYSTEM CHANGES
echo "========================================================"
echo "<< FILESYSTEM CHANGES >>"
echo "========================================================"

colordiff -u lsFsBefore lsFsAfter
rm lsFsBefore lsFsAfter


## END
echo "========================================================"
