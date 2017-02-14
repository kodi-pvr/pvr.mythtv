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

#include "pvrclient-launcher.h"
#include "client.h"

using namespace ADDON;
using namespace P8PLATFORM;

PVRClientLauncher::PVRClientLauncher(PVRClientMythTV* client)
: m_client(client)
{
}

PVRClientLauncher::~PVRClientLauncher()
{
  this->StopThread(-1); // Set stopping. don't wait as we need to signal the thread first
  m_alarm.Signal();
  this->StopThread(0); // Wait for thread to stop
}

void* PVRClientLauncher::Process()
{
  bool retry = true;
  while (!IsStopped() && retry)
  {
    if (m_client->Connect())
    {
      PVR->ConnectionStateChange(m_client->GetBackendName(), PVR_CONNECTION_STATE_CONNECTED, m_client->GetBackendVersion());
      /* Read setting "LiveTV Priority" from backend database */
      bool savedLiveTVPriority;
      if (!XBMC->GetSetting("livetv_priority", &savedLiveTVPriority))
        savedLiveTVPriority = DEFAULT_LIVETV_PRIORITY;
      g_bLiveTVPriority = m_client->GetLiveTVPriority();
      if (g_bLiveTVPriority != savedLiveTVPriority)
        m_client->SetLiveTVPriority(savedLiveTVPriority);
      /* End of process */
      break;
    }

    PVRClientMythTV::CONN_ERROR error = m_client->GetConnectionError();
    if (error == PVRClientMythTV::CONN_ERROR_UNKNOWN_VERSION)
    {
      // HEADING: Connection failed
      // Failed to connect the MythTV backend with the known protocol versions.
      // Do you want to retry ?
      std::string msg = XBMC->GetLocalizedString(30300);
      msg.append("\n").append(XBMC->GetLocalizedString(30113));
      bool canceled = false;
      if (!GUI->Dialog_YesNo_ShowAndGetInput(XBMC->GetLocalizedString(30112), msg.c_str(), canceled) && !canceled)
        retry = false;
    }
    else if (error == PVRClientMythTV::CONN_ERROR_API_UNAVAILABLE)
    {
      // HEADING: Connection failed
      // Failed to connect the API services of MythTV backend. Please check your PIN code or backend setup.
      // Do you want to retry ?
      std::string msg = XBMC->GetLocalizedString(30301);
      msg.append("\n").append(XBMC->GetLocalizedString(30113));
      bool canceled = false;
      if (!GUI->Dialog_YesNo_ShowAndGetInput(XBMC->GetLocalizedString(30112), msg.c_str(), canceled) && !canceled)
        retry = false;
    }
    else if (g_bNotifyAddonFailure)
    {
      // HEADING: Connection failed
      // No response from MythTV backend.
      // Do you want to retry ?
      std::string msg = XBMC->GetLocalizedString(30304);
      msg.append("\n").append(XBMC->GetLocalizedString(30113));
      bool canceled = false;
      if (!GUI->Dialog_YesNo_ShowAndGetInput(XBMC->GetLocalizedString(30112), msg.c_str(), canceled) && !canceled)
        retry = false;
    }
    else
    {
      m_alarm.Wait(PVRCLIENT_LAUNCHER_RETRY * 1000);
    }
  }
  XBMC->Log(LOG_NOTICE, "Launcher stopped");
  return 0;
}
