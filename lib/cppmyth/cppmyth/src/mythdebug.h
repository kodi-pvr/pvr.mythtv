/*
 * File:   mythdebug.h
 * Author: jlb
 *
 * Created on 15 février 2014, 01:41
 */

#ifndef MYTHDEBUG_H
#define	MYTHDEBUG_H

#define MYTH_DBG_NONE  -1
#define MYTH_DBG_ERROR  0
#define MYTH_DBG_WARN   1
#define MYTH_DBG_INFO   2
#define MYTH_DBG_DEBUG  3
#define MYTH_DBG_PROTO  4
#define MYTH_DBG_ALL    6

namespace Myth
{
  void DBGLevel(int l);
  void DBGAll(void);
  void DBGNone(void);
  void DBG(int level, const char *fmt, ...);
  void SetDBGMsgCallback(void (*msgcb)(int level,char *));
}

#endif	/* MYTHDEBUG_H */

