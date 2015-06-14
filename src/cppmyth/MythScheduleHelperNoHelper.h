#pragma once
/*
 *      Copyright (C) 2005-2015 Team XBMC
 *      http://www.xbmc.org
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
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "MythScheduleManager.h"

#define RECGROUP_DFLT_ID            0
#define RECGROUP_DFLT_NAME          "Default"
#define EXPIRATION_NEVER_EXPIRE_ID  0
#define EXPIRATION_ALLOW_EXPIRE_ID  1

// No helper

class MythScheduleHelperNoHelper : public MythScheduleManager::VersionHelper
{
public:

  virtual const std::vector<MythScheduleManager::TimerType>& GetTimerTypes() const;
  virtual const MythScheduleManager::RulePriorityList& GetRulePriorityList() const;
  virtual int GetRulePriorityDefaultId() const { return 0; }
  virtual const MythScheduleManager::RuleDupMethodList& GetRuleDupMethodList() const;
  virtual int GetRuleDupMethodDefaultId() const { return static_cast<int>(Myth::DM_CheckNone); }
  virtual const MythScheduleManager::RuleExpirationList& GetRuleExpirationList() const;
  virtual int GetRuleExpirationId(const MythScheduleManager::RuleExpiration& expiration) const;
  virtual const MythScheduleManager::RuleExpiration& GetRuleExpiration(int id) const;
  virtual int GetRuleExpirationDefaultId() const { return EXPIRATION_NEVER_EXPIRE_ID; }
  virtual const MythScheduleManager::RuleRecordingGroupList& GetRuleRecordingGroupList() const;
  virtual int GetRuleRecordingGroupId(const std::string& name) const;
  virtual const std::string& GetRuleRecordingGroupName(int id) const;
  virtual int GetRuleRecordingGroupDefaultId() const { return RECGROUP_DFLT_ID; }

  virtual bool SameTimeslot(const MythRecordingRule& first, const MythRecordingRule& second) const;
  virtual bool FillTimerEntryWithRule(MythTimerEntry& entry, const MythRecordingRuleNode& node) const;
  virtual bool FillTimerEntryWithUpcoming(MythTimerEntry& entry, const MythProgramInfo& recording) const;
  virtual MythRecordingRule NewFromTemplate(const MythEPGInfo& epgInfo);
  virtual MythRecordingRule NewFromTimer(const MythTimerEntry& entry, bool withTemplate);
  virtual MythRecordingRule MakeDontRecord(const MythRecordingRule& rule, const MythProgramInfo& recording);
  virtual MythRecordingRule MakeOverride(const MythRecordingRule& rule, const MythProgramInfo& recording);
};
