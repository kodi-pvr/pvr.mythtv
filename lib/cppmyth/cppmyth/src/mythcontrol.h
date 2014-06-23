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

namespace Myth
{

  class Control : public ProtoMonitor
  {
  public:
    Control(const std::string& server, unsigned port);
    ~Control();

    void Close();

    bool QueryFreeSpaceSummary(int64_t *total, int64_t *used)
    {
      return ProtoMonitor::QueryFreeSpaceSummary(total, used);
    }
    std::string GetSetting(const std::string& hostname, const std::string& setting)
    {
      return ProtoMonitor::GetSetting(hostname, setting);
    }
    bool SetSetting(const std::string& hostname, const std::string& setting, const std::string& value)
    {
      return ProtoMonitor::SetSetting(hostname, setting, value);
    }
    bool QueryGenPixmap(Program& program)
    {
      return ProtoMonitor::QueryGenpixmap(program);
    }
    bool DeleteRecording(Program& program)
    {
      return ProtoMonitor::DeleteRecording(program);
    }
    bool UndeleteRecording(Program& program)
    {
      return ProtoMonitor::UndeleteRecording(program);
    }
    bool StopRecording(Program& program)
    {
      return ProtoMonitor::StopRecording(program);
    }
    bool CancelNextRecording(int rnum, bool cancel)
    {
      return ProtoMonitor::CancelNextRecording(rnum, cancel);
    }
    StorageGroupFilePtr QuerySGFile(const std::string& hostname, const std::string& sgname, const std::string& filename)
    {
      return ProtoMonitor::QuerySGFile(hostname, sgname, filename);
    }
  };

}

#endif	/* MYTHCONTROL_H */
