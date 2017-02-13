#pragma once
/*
*      Copyright (C) 2017 Team Kodi
*      http://www.kodi.tv
*
*  This Program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2, or (at your option)
*  any later version.
*
*  This Program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with XBMC; see the file COPYING.  If not, write to
*  the Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
*  MA 02110-1301 USA
*  http://www.gnu.org/copyleft/gpl.html
*
*/

#include "pvrclient-mythtv.h"
#include <p8-platform/threads/threads.h>

#define PVRCLIENT_LAUNCHER_RETRY    30  /* Wait for 30 seconds before retry */

class PVRClientLauncher : public P8PLATFORM::CThread 
{
public:
  PVRClientLauncher(PVRClientMythTV* client);
  ~PVRClientLauncher();

protected:
  void *Process();

private:
  PVRClientMythTV* m_client;
  P8PLATFORM::CEvent m_alarm;
};
