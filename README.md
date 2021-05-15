# Random Kernel Modules
This repo has some random kernel modules I have thrown together.

## Usage
To use the moduels please build using `make` and then install using `insmod`, after install check dmesg for the Major number, or just check `/proc/devices`

### Andrew-As-A-Service
This simple kernel module infinitely prints out a random selection of lower and uppercase As to the console.
After some optimizations it now goes at 18.8GB/s on my machine instead of the original 24MB/s, in comparison /dev/zero does 20.2GB/s on my machine.
The name is based on a friend of mine named Andrew who frequently types "AAaaAaaAaAaAAaaaaAa"

### Reverse
A very simple kernel module that when written to stores whatever was written to it and then later when read returns the previously stored value backwards.

### Speed
The largest "transfer" rate I could accomplish. It obviously is not actually copying the data, just relying on reporting false numbers for the buffer length returned from fops write.
```
➜  ~ dd if=/dev/speed of=/dev/null bs=1M status=progress
114931125817704448 bytes (115 PB, 102 PiB) copied, 7 s, 16.4 PB/s^C
903019+0 records in
0+903018 records out
124109848886378496 bytes (124 PB, 110 PiB) copied, 7.56131 s, 16.4 PB/s
```