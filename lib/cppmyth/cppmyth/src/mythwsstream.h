/*
 * File:   mythwsstream.h
 * Author: jlb
 *
 * Created on 28 février 2014, 20:29
 */

#ifndef MYTHWSSTREAM_H
#define	MYTHWSSTREAM_H

#include "mythtypes.h"
#include "mythstream.h"

namespace Myth
{
  class WSStream;
  class WSResponse;

  typedef MYTH_SHARED_PTR<WSStream> WSStreamPtr;

  class WSStream : public Stream
  {
  public:
    WSStream(WSResponse *response);
    ~WSStream();

    bool EndOfStream();

    int Read(void* buffer, unsigned n);
    int64_t GetSize() const;
    int64_t GetPosition() const;
    int64_t Seek(int64_t offset, WHENCE_t whence);

  private:
    WSResponse *m_response;
  };
}

#endif	/* MYTHWSSTREAM_H */

