/*
 * File:   mythfileplayback.h
 * Author: jlb
 *
 * Created on 28 janvier 2014, 21:29
 */

#ifndef MYTHFILEPLAYBACK_H
#define	MYTHFILEPLAYBACK_H

#include "proto/mythprotoplayback.h"
#include "proto/mythprototransfer.h"
#include "mythstream.h"

namespace Myth
{

  class FilePlayback : public ProtoPlayback, public Stream
  {
  public:
    FilePlayback(const std::string& server, unsigned port);
    ~FilePlayback();

    void Close();
    bool OpenTransfer(const std::string& pathname, const std::string& sgname);
    void CloseTransfer();
    bool TransferIsOpen();

    // Implement Stream
    int64_t GetSize() const;
    int Read(void *buffer, unsigned n);
    int64_t Seek(int64_t offset, WHENCE_t whence);
    int64_t GetPosition() const;

  private:
    ProtoTransferPtr m_transfer;
  };

}

#endif	/* MYTHFILEPLAYBACK_H */
