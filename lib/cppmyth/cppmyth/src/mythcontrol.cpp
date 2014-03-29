
#include "mythcontrol.h"

using namespace Myth;

Control::Control(const std::string& server, unsigned port)
: ProtoMonitor(server, port)
{
  ProtoMonitor::Open();
}

Control::~Control()
{
  this->Close();
}

void Control::Close()
{
  ProtoMonitor::Close();
}
