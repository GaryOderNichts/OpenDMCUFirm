# OpenDMCUFirm

A Wii U DMCU firmware reimplementation and replacement.

## Overview

The DMCU (Display Microcontroller Unit) is a M68HC11 compatible microcontroller part of many AMD GPUs. On desktop systems it is involved in eDP features like Adaptive Backlight Modulation and Panel Self Refresh. On the Wii U it is used for backwards compatibility with the Wii's Video Interface.

The Wii U's DMCU firmware reads the VI state from a hardware shim mapped in the graphics controller register space and configures the graphics controller accordingly. It sets up display output and configures things like deinterlacing, scaling and overscan.

## Goals

This project reimplements the Wii U DMCU firmware based on reverse engineering of the original firmware. It is not a byte-for-byte matching decompilation.

This project builds a working DMCU firmware for the Wii U to use in Wii mode. It can be loaded using [evWii](github.com/GaryOderNichts/evwii).

The goal of this project is to gain an insight into what the DMCU firmware does and potentially improve upon it. It is currently still work-in-progess, potential issues are expected.

## Building

OpenDMCUFirm is built using GCC 3.3.6-m68hc1x-20060122, one of the last GCC versions with working M68HC11 support. You can download the GNU 68HC11/12 packages here: http://gnu-m68hc11.vacs.fr/m68hc11_pkg_zip.php  

A `Dockerfile` is provided with the repo to easily setup the toolchain. To build the project using docker, follow the steps below.
```bash
# Build using image from docker hub
docker run -it --rm -v ${PWD}:/project garyodernichts/dmcubuilder make PREFIX="wine /gnu-68hc1x/app/bin/m6811-elf-" SUFFIX=".exe"

# or alternatively build the docker image yourself
docker build . -t dmcubuilder
# and run make 
docker run -it --rm -v ${PWD}:/project dmcubuilder make PREFIX="wine /gnu-68hc1x/app/bin/m6811-elf-" SUFFIX=".exe"
```

The resulting `dmcu.d.hex` file should be placed in the `wiiu` folder on your SD Card and loaded using [evWii](github.com/GaryOderNichts/evwii)'s "Load DMCU Firmware from SD Card" option.

## References

**About the DMCU:**  
- https://lists.freedesktop.org/archives/amd-gfx/2018-September/026550.html
- https://fail0verflow.com/blog/2014/console-hacking-2013-omake/

**Graphics Controller Registers:**
- https://www.x.org/docs/AMD/old/42589_rv630_rrg_1.01o.pdf
- https://github.com/torvalds/linux/blob/51a24b7deaae5c3561965f5b4b27bb9d686add1c/drivers/gpu/drm/radeon/r500_reg.h
- https://github.com/NetBSD/src/blob/ae57d7427dc47611be14e5f7523fca0422093d29/sys/external/bsd/drm2/dist/drm/radeon/r600d.h
