
#include "mythwsstream.h"
#include "private/mythwsresponse.h"

using namespace Myth;

WSStream::WSStream(WSResponse *response)
: m_response(response)
{
}

WSStream::~WSStream()
{
  if (m_response)
    delete m_response;
  m_response = NULL;
}

bool WSStream::EndOfStream()
{
  if (m_response)
    return (m_response->GetConsumed() == m_response->GetContentLength());
  return true;
}

int WSStream::Read(void* buffer, unsigned n)
{
  return (m_response ? (int)m_response->ReadContent((char *)buffer, n) : 0);
}

int64_t WSStream::GetSize() const
{
  return (m_response ? (int64_t)m_response->GetContentLength() : 0);
}

int64_t WSStream::GetPosition() const
{
  return (m_response ? m_response->GetConsumed() : 0);
}

int64_t WSStream::Seek(int64_t offset, WHENCE_t whence)
{
  (void)offset;
  (void)whence;
  return GetPosition();
}
