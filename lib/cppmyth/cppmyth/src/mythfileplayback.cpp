
#include "mythfileplayback.h"
#include "mythlivetvplayback.h"
#include "mythdebug.h"
#include "private/builtin.h"
#include "private/platform/threads/mutex.h"

#include <limits>
#include <cstdio>

using namespace Myth;

///////////////////////////////////////////////////////////////////////////////
////
//// Protocol connection to control playback
////

FilePlayback::FilePlayback(const std::string& server, unsigned port)
: ProtoPlayback(server, port)
, m_transfer(new ProtoTransfer(server, port))
{
  ProtoPlayback::Open();
}

FilePlayback::~FilePlayback()
{
  this->Close();
}

void FilePlayback::Close()
{
  PLATFORM::CLockObject lock(*m_mutex);
  CloseTransfer();
  if (IsOpen())
    ProtoPlayback::Close();
}

bool FilePlayback::OpenTransfer(const std::string& pathname, const std::string& sgname)
{
  PLATFORM::CLockObject lock(*m_mutex);
  if (!IsOpen())
    return false;
  CloseTransfer();
  if (m_transfer->Open(pathname, sgname))
    return true;
  return false;
}

void FilePlayback::CloseTransfer()
{
  TransferDone(*m_transfer);
  m_transfer->Close();
}

bool FilePlayback::TransferIsOpen()
{
  return ProtoPlayback::TransferIsOpen(*m_transfer);
}

int64_t FilePlayback::GetSize() const
{
  return m_transfer->fileSize;
}

int FilePlayback::Read(void *buffer, unsigned n)
{
  int r = 0;
  int64_t s;

  s = m_transfer->fileSize - m_transfer->filePosition; // Acceptable block size
  if (s > 0)
  {
    if (s < (int64_t)n)
    n = (unsigned)s ;

    r = TransferRequestBlock(*m_transfer, n);
    if (r > 0)
    {
      // Read block data from transfer socket
      r = (int)m_transfer->ReadData(buffer, (size_t)r);
    }
  }
  return r;
}

int64_t FilePlayback::Seek(int64_t offset, WHENCE_t whence)
{
  return TransferSeek(*m_transfer, offset, whence);
}

int64_t FilePlayback::GetPosition() const
{
  return m_transfer->filePosition;
}
