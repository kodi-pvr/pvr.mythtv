/*
 * File:   mythlivetvplayback.h
 * Author: jlb
 *
 * Created on 2 février 2014, 10:14
 */

#ifndef MYTHLIVETVPLAYBACK_H
#define	MYTHLIVETVPLAYBACK_H

#include "proto/mythprotorecorder.h"
#include "proto/mythprototransfer.h"
#include "proto/mythprotomonitor.h"
#include "mythstream.h"
#include "mytheventhandler.h"
#include "mythtypes.h"

#include <vector>

namespace Myth
{

  class LiveTVPlayback : public ProtoMonitor, public Stream, private EventSubscriber
  {
  public:
    LiveTVPlayback(EventHandler& handler);
    LiveTVPlayback(const std::string& server, unsigned port);
    ~LiveTVPlayback();

    void Close();
    void SetTuneDelay(unsigned delay);
    bool SpawnLiveTV(const Channel& channel, uint32_t prefcardid = 0);
    void StopLiveTV();

    // Implement Stream
    int64_t GetSize() const;
    int Read(void *buffer, unsigned n);
    int64_t Seek(int64_t offset, WHENCE_t whence);
    int64_t GetPosition() const;

    bool IsPlaying() const;
    bool IsLiveRecording() const;
    bool KeepLiveRecording(bool keep);
    ProgramPtr GetPlayedProgram() const;
    time_t GetLiveTimeStart() const;
    unsigned GetChainedCount() const;
    ProgramPtr GetChainedProgram(unsigned sequence) const;
    uint32_t GetCardId() const;
    SignalStatusPtr GetSignal() const;

    // Implement EventSubscriber
    void HandleBackendMessage(const EventMessage& msg);

  private:
    EventHandler m_eventHandler;
    unsigned m_eventSubscriberId;

    unsigned m_tuneDelay;
    ProtoRecorderPtr m_recorder;
    SignalStatusPtr m_signal;

    typedef std::vector<std::pair<ProtoTransferPtr, ProgramPtr> > chained_t;
    struct {
      std::string UID;
      chained_t chained;
      ProtoTransferPtr currentTransfer;
      volatile unsigned currentSequence;
      volatile unsigned lastSequence;
      volatile bool watch;
      volatile bool switchOnCreate;
    } m_chain;

    void InitChain();
    int GetRecorderNum();
    bool IsChained(const Program& program);
    void HandleChainUpdate(ProtoRecorder& recorder);
    bool SwitchChain(unsigned sequence);
    bool SwitchChainLast();
  };

}

#endif	/* MYTHLIVETVPLAYBACK_H */

