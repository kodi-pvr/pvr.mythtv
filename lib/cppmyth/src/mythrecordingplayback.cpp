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

#include "mythrecordingplayback.h"
#include "private/debug.h"
#include "private/os/threads/mutex.h"
#include "private/builtin.h"

#include <limits>
#include <cstdio>

using namespace Myth;

///////////////////////////////////////////////////////////////////////////////
////
//// Protocol connection to control playback
////

RecordingPlayback::RecordingPlayback(EventHandler& handler)
: ProtoPlayback(handler.GetServer(), handler.GetPort()), EventSubscriber()
, m_eventHandler(handler)
, m_eventSubscriberId(0)
, m_transfer(NULL)
, m_recording(NULL)
, m_readAhead(false)
, m_chunk(MYTH_RECORDING_CHUNK_SIZE)
{
  m_buffer.pos = 0;
  m_buffer.len = 0;
  m_buffer.data = new unsigned char[m_chunk];
  m_eventSubscriberId = m_eventHandler.CreateSubscription(this);
  m_eventHandler.SubscribeForEvent(m_eventSubscriberId, EVENT_UPDATE_FILE_SIZE);
  Open();
}

RecordingPlayback::RecordingPlayback(const std::string& server, unsigned port)
: ProtoPlayback(server, port), EventSubscriber()
, m_eventHandler(server, port)
, m_eventSubscriberId(0)
, m_transfer(NULL)
, m_recording(NULL)
, m_readAhead(false)
, m_chunk(MYTH_RECORDING_CHUNK_SIZE)
{
  m_buffer.pos = 0;
  m_buffer.len = 0;
  m_buffer.data = new unsigned char[m_chunk];
  // Private handler will be stopped and closed by destructor.
  m_eventSubscriberId = m_eventHandler.CreateSubscription(this);
  m_eventHandler.SubscribeForEvent(m_eventSubscriberId, EVENT_UPDATE_FILE_SIZE);
  Open();
}

RecordingPlayback::~RecordingPlayback()
{
  if (m_eventSubscriberId)
    m_eventHandler.RevokeSubscription(m_eventSubscriberId);
  Close();
  delete[] m_buffer.data;
}

bool RecordingPlayback::Open()
{
  // Begin critical section
  OS::CLockGuard lock(*m_mutex);
  if (ProtoPlayback::IsOpen())
    return true;
  if (ProtoPlayback::Open())
  {
    if (!m_eventHandler.IsRunning())
      m_eventHandler.Start();
    return true;
  }
  return false;
}

void RecordingPlayback::Close()
{
  // Begin critical section
  OS::CLockGuard lock(*m_mutex);
  CloseTransfer();
  ProtoPlayback::Close();
}

bool RecordingPlayback::OpenTransfer(ProgramPtr recording)
{
  // Begin critical section
  OS::CLockGuard lock(*m_mutex);
  if (!ProtoPlayback::IsOpen())
    return false;
  CloseTransfer();
  if (recording)
  {
    m_transfer.reset(new ProtoTransfer(m_server, m_port, recording->fileName, recording->recording.storageGroup));
    if (m_transfer->Open())
    {
      m_recording.swap(recording);
      m_recording->fileSize = m_transfer->GetSize();
      return true;
    }
    m_transfer.reset();
  }
  return false;
}

void RecordingPlayback::CloseTransfer()
{
  // Begin critical section
  OS::CLockGuard lock(*m_mutex);
  m_recording.reset();
  if (m_transfer)
  {
    TransferDone(*m_transfer);
    m_transfer->Close();
    m_transfer.reset();
  }
}

bool RecordingPlayback::TransferIsOpen()
{
  ProtoTransferPtr transfer(m_transfer);
  if (transfer)
    return ProtoPlayback::TransferIsOpen(*transfer);
  return false;
}

void RecordingPlayback::SetChunk(unsigned size)
{
  if (size < MYTH_RECORDING_CHUNK_MIN)
    size = MYTH_RECORDING_CHUNK_MIN;
  else if (size > MYTH_RECORDING_CHUNK_MAX)
    size = MYTH_RECORDING_CHUNK_MAX;

  m_buffer.pos = m_buffer.len = 0;
  delete[] m_buffer.data;
  m_buffer.data = new unsigned char[size];
  m_chunk = size;
}

int64_t RecordingPlayback::GetSize() const
{
  ProtoTransferPtr transfer(m_transfer);
  if (transfer)
    return transfer->GetSize();
  return 0;
}

int RecordingPlayback::Read(void* buffer, unsigned n)
{
  int c = 0;
  bool refill = true;
  for (;;)
  {
    // all requested data are in the buffer
    if (m_buffer.len >= n)
    {
      memcpy(static_cast<unsigned char*>(buffer) + c, m_buffer.data + m_buffer.pos, n);
      c += n;
      m_buffer.pos += n;
      m_buffer.len -= n;
      return c;
    }
    // fill with the rest of data before read a new chunk
    if (m_buffer.len > 0)
    {
      memcpy(static_cast<unsigned char*>(buffer) + c, m_buffer.data + m_buffer.pos, m_buffer.len);
      c += m_buffer.len;
      n -= m_buffer.len;
      m_buffer.len = 0;
    }
    if (!refill)
      break;
    m_buffer.pos = 0;
    int r = _read(m_buffer.data, m_chunk);
    if (r < 0)
      return -1;
    m_buffer.len += r;
    refill = false; // won't read again
  }
  return c;
}

int RecordingPlayback::_read(void *buffer, unsigned n)
{
  ProtoTransferPtr transfer(m_transfer);
  if (transfer)
  {
    if (!m_readAhead)
    {
      int64_t s = transfer->GetRemaining(); // Acceptable block size
      if (s > 0)
      {
        if (s < (int64_t)n)
          n = (unsigned)s;
        // Request block data from transfer socket
        return TransferRequestBlock(*transfer, buffer, n);
      }
      return 0;
    }
    else
    {
      // Request block data from transfer socket
      return TransferRequestBlock(*transfer, buffer, n);
    }
  }
  return -1;
}

int64_t RecordingPlayback::Seek(int64_t offset, WHENCE_t whence)
{
  if (whence == WHENCE_CUR)
  {
    if (offset == 0)
    {
      int64_t p = _seek(offset, whence);
      // it returns the current position of the first byte in buffer
      return (p >= m_buffer.len ? p - m_buffer.len : p);
    }
    // rebase to the first position in the buffer
    offset -= m_buffer.len;
  }
  m_buffer.len = 0; // clear data in buffer
  return _seek(offset, whence);
}

int64_t RecordingPlayback::_seek(int64_t offset, WHENCE_t whence)
{
  ProtoTransferPtr transfer(m_transfer);
  if (transfer)
    return TransferSeek(*transfer, offset, whence);
  return -1;
}

int64_t RecordingPlayback::GetPosition() const
{
  ProtoTransferPtr transfer(m_transfer);
  if (transfer)
  {
    // it returns the current position of first byte in buffer
    return transfer->GetPosition() - m_buffer.len;
  }
  return 0;
}

void RecordingPlayback::HandleBackendMessage(EventMessagePtr msg)
{
  // First of all i hold shared resources using copies
  ProgramPtr recording(m_recording);
  ProtoTransferPtr transfer(m_transfer);
  switch (msg->event)
  {
    case EVENT_UPDATE_FILE_SIZE:
      if (msg->subject.size() >= 3 && recording && transfer)
      {
        int64_t newsize;
        // Message contains chanid + starttime as recorded key
        if (msg->subject.size() >= 4)
        {
          uint32_t chanid;
          time_t startts;
          if (string_to_uint32(msg->subject[1].c_str(), &chanid)
                  || string_to_time(msg->subject[2].c_str(), &startts)
                  || recording->channel.chanId != chanid
                  || recording->recording.startTs != startts
                  || string_to_int64(msg->subject[3].c_str(), &newsize))
            break;
        }
        // Message contains recordedid as key
        else
        {
          uint32_t recordedid;
          if (string_to_uint32(msg->subject[1].c_str(), &recordedid)
                  || recording->recording.recordedId != recordedid
                  || string_to_int64(msg->subject[2].c_str(), &newsize))
            break;
        }
        // The file grows. Allow reading ahead
        m_readAhead = true;
        transfer->SetSize(newsize);
        recording->fileSize = newsize;
        DBG(DBG_DEBUG, "%s: (%d) %s %" PRIi64 "\n", __FUNCTION__,
                msg->event, recording->fileName.c_str(), newsize);
      }
      break;
    //case EVENT_HANDLER_STATUS:
    //  if (msg->subject[0] == EVENTHANDLER_DISCONNECTED)
    //    closeTransfer();
    //  break;
    default:
      break;
  }
}
