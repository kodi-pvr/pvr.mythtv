/*
 * File:   mythrecordingplayback.h
 * Author: jlb
 *
 * Created on 1 février 2014, 14:24
 */

#ifndef MYTHRECORDINGPLAYBACK_H
#define	MYTHRECORDINGPLAYBACK_H

#include "proto/mythprotoplayback.h"
#include "proto/mythprototransfer.h"
#include "mythstream.h"
#include "mytheventhandler.h"

namespace Myth
{

  class RecordingPlayback : public ProtoPlayback, public Stream, private EventSubscriber
  {
  public:
    RecordingPlayback(EventHandler& handler);
    RecordingPlayback(const std::string& server, unsigned port);
    ~RecordingPlayback();

    void Close();
    bool OpenTransfer(ProgramPtr recording);
    void CloseTransfer();
    bool TransferIsOpen();

    // Implement Stream
    int64_t GetSize() const;
    int Read(void *buffer, unsigned n);
    int64_t Seek(int64_t offset, WHENCE_t whence);
    int64_t GetPosition() const;

    // Implement EventSubscriber
    void HandleBackendMessage(const EventMessage& msg);

  private:
    EventHandler m_eventHandler;
    unsigned m_eventSubscriberId;
    ProtoTransferPtr m_transfer;
    ProgramPtr m_recording;
  };

}

#endif	/* MYTHRECORDINGPLAYBACK_H */
