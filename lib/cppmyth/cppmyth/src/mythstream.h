/*
 * File:   mythstream.h
 * Author: jlb
 *
 * Created on 18 février 2014, 23:10
 */

#ifndef MYTHSTREAM_H
#define	MYTHSTREAM_H

#include "mythtypes.h"

namespace Myth
{
  class Stream
  {
  public:
    virtual ~Stream() {};
    virtual int64_t GetSize() const = 0;
    virtual int Read(void *buffer, unsigned n) = 0;
    virtual int64_t Seek(int64_t offset, WHENCE_t whence) = 0;
    virtual int64_t GetPosition() const = 0;
  };
}

#endif	/* MYTHSTREAM_H */
