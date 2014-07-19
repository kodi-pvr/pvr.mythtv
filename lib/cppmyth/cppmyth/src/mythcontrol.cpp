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

#include "mythcontrol.h"

using namespace Myth;

Control::Control(const std::string& server, unsigned protoPort, unsigned wsapiPort)
: m_monitor(server, protoPort)
, m_wsapi(server, wsapiPort)
{
  m_monitor.Open();
}

Control::~Control()
{
  Close();
}

bool Control::Open()
{
  if (m_monitor.IsOpen())
    return true;
  return m_monitor.Open();
}

void Control::Close()
{
  m_monitor.Close();
}
