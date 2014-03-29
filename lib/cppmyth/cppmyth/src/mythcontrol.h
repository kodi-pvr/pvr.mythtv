/*
 * File:   mythcontrol.h
 * Author: jlb
 *
 * Created on 6 février 2014, 14:50
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
