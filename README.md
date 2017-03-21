[![Build Status](https://travis-ci.org/kodi-pvr/pvr.mythtv.svg?branch=master)](https://travis-ci.org/kodi-pvr/pvr.mythtv)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/3115/badge.svg)](https://scan.coverity.com/projects/3115)

# MythTV PVR
MythTV PVR client addon for [Kodi] (http://kodi.tv)

## Build instructions
When building the addon you have to use the correct branch depending on which version of Kodi you're building against.
For example, if you're building the `Krypton` branch of Kodi you should checkout the `Krypton` branch of this repository.
Also make sure you follow this README from the branch in question.

### Linux

1. `git clone --branch=Krypton --depth=1 https://github.com/xbmc/xbmc.git`
2. `git clone --branch=Krypton https://github.com/kodi-pvr/pvr.mythtv.git`
3. `cd pvr.mythtv && mkdir build && cd build`
4. `cmake -DADDONS_TO_BUILD=pvr.mythtv -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../xbmc/addons -DPACKAGE_ZIP=ON ../../xbmc/cmake/addons`
5. `make`

The addon files will be placed in `../../xbmc/kodi-build/addons` so if you build Kodi from source and run it directly
the addon will be available as a system addon.

##### Useful links

* [Kodi's PVR user support] (http://forum.kodi.tv/forumdisplay.php?fid=170)
* [Kodi's PVR development support] (http://forum.kodi.tv/forumdisplay.php?fid=136)
