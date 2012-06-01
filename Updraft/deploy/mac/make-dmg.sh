#!/bin/sh
#
#12/03/02- Modified script, originally posted by Mr. Sharumpe to Mac OS X Hints - JG
#
# Creates a disk image (dmg) on Mac OS X from the command line.
# usage:
# mkdmg   
#
# Where  is the name to use for the mounted image,  is the version
# number of the volume and  is where the contents to put on the dmg are.
#
# The result will be a file called -.dmg
#
if [ $# != 3 ]; then
echo "Usage: mkdmg volname vers srcdir"
exit 0
fi
VOL="$1"
VER="$2"
SRC="$3"
DMG="tmp-$VOL.dmg"

## The newfs command can only be run as root, so we need to prompt for an administrative
## password. By default, the admin group is listed in /etc/sudoers give its memebers the
## right to run commands as root, albeit still having to authenticate.
#
#sudo -v

## Create temporary disk image and format, ejecting when done


## The total size of the disk image is calculated in here to be 5% percent more than the
## total data size. This allows for the partition tables overhead (64 sectors, 512 bytes each)
## plus some spare.
#
SIZE=`du -sk ${SRC} | awk '{printf "%.3gn",($1*10.5)/(10240)}'`

#hdiutil create "$DMG" -megabytes ${SIZE} -ov -type UDIF

## Create /dev entries but do not mount it, else the Finder will prompt to initialize the disk
#
#DISK=`hdid -nomount "$DMG" |awk '/scheme/ {print substr ($1, 6, length)}'`

hdiutil create -srcfolder "${SRC}" -volname "${VOL}" -fs HFS+ -format UDRW -megabytes ${SIZE} ${DMG}

DISK=$(hdiutil attach -readwrite -noverify -noautoopen "$DMG" | egrep '^/dev/' | sed 1q | awk '{print $1}')


## Make a new filesystem and eject the disk
#
#newfs_hfs -v "$VOL" /dev/${DISK}

#hdiutil eject $DISK

## Mount and copy files onto volume, making sure that there are no locked files before running ditto
#
#hdid "$DMG"

#cp -r "${SRC}" "/Volumes/${VOL}" && echo ">"

hdiutil eject $DISK

## Convert to compressed image, delete temp image if it exists
#
if [ -e "${VOL}-${VER}.dmg" ]; then
rm -f "${VOL}-${VER}.dmg"
fi
hdiutil convert "$DMG" -format UDZO -o "${VOL}-${VER}.dmg"
rm -f "$DMG"