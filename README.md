# Random Kernel Modules
This repo has some random kernel modules I have thrown together.

## Usage
To use the moduels please build using `make` and then install using `insmod`, after install check dmesg for the Major number, or just check `/proc/devices`

### Andrew-As-A-Service
This simple kernel module infinitely prints out a random selection of lower and uppercase As to the console.
After some optimizations it now goes at 18.8GB/s on my machine instead of the original 24MB/s, in comparison /dev/zero does 20.2GB/s on my machine.
The name is based on a friend of mine named Andrew who frequently types "AAaaAaaAaAaAAaaaaAa"