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

#include "taskhandler.h"

TaskHandler::TaskHandler()
: CThread()
{
  CreateThread(false);
}

TaskHandler::~TaskHandler()
{
  Clear();
  Suspend();
  // last chance
  StopThread(1000);
}

void TaskHandler::ScheduleTask(Task *task, unsigned delayMs)
{
  P8PLATFORM::CLockObject lock(m_mutex);
  m_queue.push(std::make_pair(task, new P8PLATFORM::CTimeout(delayMs)));
  m_queueContent.Signal();
}

void TaskHandler::Clear()
{
  P8PLATFORM::CLockObject lock(m_mutex);
  for (std::vector<Scheduled>::const_iterator it = m_delayed.begin(); it != m_delayed.end(); ++it)
  {
    delete it->second;
    delete it->first;
  }
  m_delayed.clear();
  while (!m_queue.empty())
  {
    Scheduled& item = m_queue.front();
    delete item.second;
    delete item.first;
    m_queue.pop();
  }
}

void TaskHandler::Suspend()
{
  if (IsStopped())
    return;
  StopThread(-1);
  m_queueContent.Signal();
}

bool TaskHandler::resume()
{
  if (!IsStopped())
    return true;
  // wait until stopped
  if (IsRunning() && !StopThread(0))
    return false;
  // wait until running
  return CreateThread(true);
}


void *TaskHandler::Process()
{
  P8PLATFORM::CLockObject lock(m_mutex);
  while (!IsStopped())
  {
    P8PLATFORM::CTimeout later;
    unsigned left = 0;

    // refill all delayed in queue
    for (std::vector<Scheduled>::const_iterator it = m_delayed.begin(); it != m_delayed.end(); ++it)
      m_queue.push(*it);
    m_delayed.clear();

    while (!m_queue.empty() && !IsStopped())
    {
      Scheduled& item = m_queue.front();
      m_queue.pop();
      // delay the job else process it
      if ((left = item.second->TimeLeft()) > 0)
      {
        m_delayed.push_back(item);
        lock.Unlock();
        if (!later.IsSet() || later.TimeLeft() > left)
          later.Init(left);
      }
      else
      {
        lock.Unlock();
        item.first->Execute();
        delete item.second;
        delete item.first;
      }

      lock.Lock();
    }

    if (IsStopped())
      break;

    lock.Unlock();

    if (!later.IsSet())
      m_queueContent.Wait();
    else if ((left = later.TimeLeft()) > 0)
      m_queueContent.Wait(left);

    lock.Lock();
  }
  return NULL;
}
