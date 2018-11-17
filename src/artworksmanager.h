#pragma once
/*
 *      Copyright (C) 2018 Team XBMC
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

#include "cppmyth/MythChannel.h"
#include "cppmyth/MythProgramInfo.h"

#include <mythwsapi.h>

#include <string>

class ArtworkManager
{
public:
  enum ArtworksType
  {
    AWTypeThumbnail,
    AWTypeCoverart,
    AWTypeFanart,
    AWTypeChannelIcon,
    AWTypeBanner,
    AWTypeScreenshot,
    AWTypePoster,
    AWTypeBackcover,
    AWTypeInsidecover,
    AWTypeCDImage
  };

  static std::vector<ArtworksType> GetArtworksTypes()
  {
    std::vector<ArtworksType> ret;
    ret.push_back(AWTypeChannelIcon);
    ret.push_back(AWTypeThumbnail);
    ret.push_back(AWTypeCoverart);
    ret.push_back(AWTypeFanart);
    ret.push_back(AWTypeBanner);
    ret.push_back(AWTypeScreenshot);
    ret.push_back(AWTypePoster);
    ret.push_back(AWTypeBackcover);
    ret.push_back(AWTypeInsidecover);
    ret.push_back(AWTypeCDImage);
    return ret;
  }

  static const char *GetTypeNameByArtworksType(ArtworksType type)
  {
    switch(type)
    {
    case AWTypeChannelIcon: return "channelIcon";
    case AWTypeThumbnail: return "thumbnail";
    case AWTypeCoverart: return "coverart";
    case AWTypeFanart: return "fanart";
    case AWTypeBanner: return "banner";
    case AWTypeScreenshot: return "screenshot";
    case AWTypePoster: return "poster";
    case AWTypeBackcover: return "backcover";
    case AWTypeInsidecover: return "insidecover";
    case AWTypeCDImage: return "cdimage";
    default: return "";
    }
  }

  ArtworkManager(const std::string& server, unsigned wsapiport, const std::string& wsapiSecurityPin);
  virtual ~ArtworkManager();

  std::string GetChannelIconPath(const MythChannel& channel);
  std::string GetPreviewIconPath(const MythProgramInfo& recording);
  std::string GetArtworkPath(const MythProgramInfo& recording, ArtworksType type);

  Myth::WSAPI *m_wsapi;
  std::string m_localBasePath;
};
