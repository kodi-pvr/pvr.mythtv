/*
 *      Copyright (C) 2014 Jean-Luc Barriere
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
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
 *  MA 02110-1301 USA
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#ifndef MYTHCONTROL_H
#define	MYTHCONTROL_H

#include "proto/mythprotomonitor.h"
#include "mythtypes.h"
#include "mythwsapi.h"

namespace Myth
{

  class Control
  {
  public:
    Control(const std::string& server, unsigned protoPort, unsigned wsapiPort);
    ~Control();

    bool Open();
    void Close();
    bool IsOpen() { return m_monitor.IsOpen(); }

    bool QueryFreeSpaceSummary(int64_t *total, int64_t *used)
    {
      return m_monitor.QueryFreeSpaceSummary(total, used);
    }
    std::string GetSetting(const std::string& hostname, const std::string& setting)
    {
      return m_monitor.GetSetting(hostname, setting);
    }
    bool SetSetting(const std::string& hostname, const std::string& setting, const std::string& value)
    {
      return m_monitor.SetSetting(hostname, setting, value);
    }
    bool QueryGenPixmap(Program& program)
    {
      return m_monitor.QueryGenpixmap(program);
    }
    bool DeleteRecording(Program& program, bool force = false, bool forget = false)
    {
      return m_monitor.DeleteRecording(program, force, forget);
    }
    bool UndeleteRecording(Program& program)
    {
      return m_monitor.UndeleteRecording(program);
    }
    bool StopRecording(Program& program)
    {
      return m_monitor.StopRecording(program);
    }
    bool CancelNextRecording(int rnum, bool cancel)
    {
      return m_monitor.CancelNextRecording(rnum, cancel);
    }
    StorageGroupFilePtr QuerySGFile(const std::string& hostname, const std::string& sgname, const std::string& filename)
    {
      return m_monitor.QuerySGFile(hostname, sgname, filename);
    }
    /**
     * @brief Request a set of cut list marks for a recording
     * @param program
     * @param unit 0 = Frame count, 1 = Position, 2 = Duration ms
     * @return MarkListPtr
     */
    MarkListPtr GetCutList(Program& program, int unit = 0)
    {
      return m_monitor.GetCutList(program, unit);
    }
    /**
     * @brief Request a set of commercial break marks for a recording
     * @param program
     * @param unit 0 = Frame count, 1 = Position, 2 = Duration ms
     * @return MarkListPtr
     */
    MarkListPtr GetCommBreakList(Program& program, int unit = 0)
    {
      return m_monitor.GetCommBreakList(program, unit);
    }

  private:
    ProtoMonitor m_monitor;
    WSAPI m_wsapi;

  };

}

#endif	/* MYTHCONTROL_H */
