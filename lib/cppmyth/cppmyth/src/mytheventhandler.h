/*
 * File:   mytheventhandler.h
 * Author: jlb
 *
 * Created on 29 janvier 2014, 21:19
 */

#ifndef MYTHEVENTHANDLER_H
#define	MYTHEVENTHANDLER_H

#include "mythtypes.h"

#include <string>

#define EVENTHANDLER_CONNECTED      "CONNECTED"
#define EVENTHANDLER_DISCONNECTED   "DISCONNECTED"
#define EVENTHANDLER_TIMEOUT        1 // 1 sec

namespace Myth
{

  class ProtoEvent;

  class EventSubscriber
  {
  public:
    virtual ~EventSubscriber() {};
    virtual void HandleBackendMessage(const EventMessage& msg) = 0;
  };

  class EventHandler
  {
  public:
    EventHandler(const std::string& server, unsigned port);

    bool Start() { return m_imp->Start(); }
    void Stop() { m_imp->Stop(); }
    std::string GetServer() const { return m_imp->GetServer(); }
    unsigned GetPort() const { return m_imp->GetPort(); }
    bool IsRunning() { return m_imp->IsRunning(); }
    bool IsConnected() { return m_imp->IsConnected(); }

    unsigned CreateSubscription(EventSubscriber *sub) { return m_imp->CreateSubscription(sub); }
    bool SubscribeForEvent(unsigned subid, EVENT_t event) { return m_imp->SubscribeForEvent(subid, event);}
    void RevokeSubscription(unsigned subid) { m_imp->RevokeSubscription(subid); }

    class EventHandlerThread
    {
      friend class EventHandler;
    public:
      EventHandlerThread(const std::string& server, unsigned port);
      virtual ~EventHandlerThread();
      virtual std::string GetServer() const = 0;
      virtual unsigned GetPort() const = 0;
      virtual bool Start() = 0;
      virtual void Stop() = 0;
      virtual bool IsRunning() = 0;
      virtual bool IsConnected() = 0;
      virtual unsigned CreateSubscription(EventSubscriber *sub) = 0;
      virtual bool SubscribeForEvent(unsigned subid, EVENT_t event) = 0;
      virtual void RevokeSubscription(unsigned subid) = 0;
    protected:
      ProtoEvent *m_event;
    };

    typedef MYTH_SHARED_PTR<EventHandlerThread> EventHandlerThreadPtr;

  private:
    EventHandlerThreadPtr m_imp;
  };

}

#endif	/* MYTHEVENTHANDLER_H */
