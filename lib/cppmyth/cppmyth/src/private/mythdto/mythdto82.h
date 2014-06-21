/* 
 * File:   mythdto82.h
 * Author: jlb
 *
 * Created on 21 juin 2014, 01:44
 */

#ifndef MYTHDTO82_H
#define	MYTHDTO82_H

#include "mythdto.h"
#include "version.h"
#include "list.h"
#include "program.h"
#include "channel.h"
#include "recording.h"
#include "artwork.h"
#include "capturecard.h"
#include "videosource.h"
#include "recordschedule.h"

namespace MythDTO82
{
  attr_bind_t recording[] =
  {
    { "RecordId",       IS_UINT32,  (setter_t)MythDTORecording::SetRecording_RecordId },
    { "Priority",       IS_UINT32,  (setter_t)MythDTORecording::SetRecording_Priority },
    { "Status",         IS_INT8,    (setter_t)MythDTORecording::SetRecording_Status },
    { "EncoderId",      IS_UINT32,  (setter_t)MythDTORecording::SetRecording_EncoderId },
    { "RecType",        IS_UINT8,   (setter_t)MythDTORecording::SetRecording_RecType },
    { "DupInType",      IS_UINT8,   (setter_t)MythDTORecording::SetRecording_DupInType },
    { "DupMethod",      IS_UINT8,   (setter_t)MythDTORecording::SetRecording_DupMethod },
    { "StartTs",        IS_TIME,    (setter_t)MythDTORecording::SetRecording_StartTs },
    { "EndTs",          IS_TIME,    (setter_t)MythDTORecording::SetRecording_EndTs },
    { "Profile",        IS_STRING,  (setter_t)MythDTORecording::SetRecording_Profile },
    { "RecGroup",       IS_STRING,  (setter_t)MythDTORecording::SetRecording_RecGroup },
    { "StorageGroup",   IS_STRING,  (setter_t)MythDTORecording::SetRecording_StorageGroup },
    { "PlayGroup",      IS_STRING,  (setter_t)MythDTORecording::SetRecording_PlayGroup },
    { "RecordedId",     IS_UINT32,  (setter_t)MythDTORecording::SetRecording_RecordedId },
  };
  bindings_t RecordingBindArray = { sizeof(recording) / sizeof(attr_bind_t), recording };
}

#endif	/* MYTHDTO82_H */
