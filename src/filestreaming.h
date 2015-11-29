#pragma once
/*
 *      Copyright (C) 2005-2015 Team XBMC
 *      http://www.xbmc.org
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
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
 *  MA 02110-1301 USA
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <mythstream.h>

class FileStreaming : public Myth::Stream
{
public:
  FileStreaming(const std::string& filePath);
  virtual ~FileStreaming();

  bool IsValid() { return m_valid; }

  virtual int Read(void* buffer, unsigned n);
  virtual int64_t Seek(int64_t offset, Myth::WHENCE_t whence);
  virtual int64_t GetPosition() const { return m_pos; }
  virtual int64_t GetSize() const { return m_flen; }

private:
  bool m_valid;
  void* m_file;
  int64_t m_flen;
  int64_t m_pos;

  bool _init(const char* filePath);
};
