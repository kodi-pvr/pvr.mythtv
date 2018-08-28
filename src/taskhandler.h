#pragma once
/*
 *      Copyright (C) 2018 Team XBMC
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
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
 *  MA 02110-1301 USA
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <p8-platform/threads/threads.h>

#include <queue>
#include <vector>

class Task
{
public:
  virtual ~Task() { }
  virtual void Execute() = 0;
};

class TaskHandler : private P8PLATFORM::CThread
{
public:
  TaskHandler();
  ~TaskHandler();

  void ScheduleTask(Task *task, unsigned delayMs = 0);
  void Clear();
  void Suspend();
  bool resume();

protected:
    void *Process();

private:
  typedef std::pair<Task*, P8PLATFORM::CTimeout*> Scheduled;
  std::queue<Scheduled> m_queue;
  std::vector<Scheduled> m_delayed;
  P8PLATFORM::CMutex m_mutex;
  P8PLATFORM::CEvent m_queueContent;
};
