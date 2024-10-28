/***********************************************************************
 *                                                                     *
 *   The JIAJIA Software Distributed Shared Memory System              *
 *                                                                     *
 *   Copyright (C) 1997 the Center of High Performance Computing       *
 *   of Institute of Computing Technology, Chinese Academy of          *
 *   Sciences.  All rights reserved.                                   *
 *                                                                     *
 *   Permission to use, copy, modify and distribute this software      *
 *   is hereby granted provided that (1) source code retains these     *
 *   copyright, permission, and disclaimer notices, and (2) redistri-  *
 *   butions including binaries reproduce the notices in supporting    *
 *   documentation, and (3) all advertising materials mentioning       *
 *   features or use of this software display the following            *
 *   acknowledgement: ``This product includes software developed by    *
 *   the Center of High Performance Computing, Institute of Computing  *
 *   Technology, Chinese Academy of Sciences."                         *
 *                                                                     *
 *   This program is distributed in the hope that it will be useful,   *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *                                                                     *
 *   Center of High Performance Computing requests users of this       *
 *   software to return to dsm@water.chpc.ict.ac.cn any                *
 *   improvements that they make and grant CHPC redistribution rights. *
 *                                                                     *
 *           Author: Weiwu Hu, Weisong Shi, Zhimin Tang                *
 * =================================================================== *
 *   This software is ported to SP2 by                                 *
 *                                                                     *
 *         M. Rasit Eskicioglu                                         *
 *         University of Alberta                                       *
 *         Dept. of Computing Science                                  *
 *         Edmonton, Alberta T6G 2H1 CANADA                            *
 * =================================================================== *
 **********************************************************************/

#ifndef JIAGLOBAL_H
#define JIAGLOBAL_H

#ifndef Maxhosts
#define Maxhosts 16
#endif
#define Maxlocks 64
#define Intbytes 4
#define Intbits 32
#define Pagesize 4096
#ifndef Cachepages
#define Cachepages 1024
#endif
#define Startaddr 0x60000000
#define Maxmemsize 0x8000000
#define Maxmempages (Maxmemsize / Pagesize)

#include <math.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef LINUX
#include <stropts.h>
#include <sys/conf.h>
#endif
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#ifdef SOLARIS
#include <siginfo.h>
#include <ucontext.h>
#endif /* SOLARIS */

#include <pwd.h>
#include <signal.h>
#include <unistd.h>

#ifdef SOLARIS
#include <sys/file.h>
#endif /* SOLARIS */

#include <string.h>
#include <sys/resource.h>

#ifdef AIX41
#include <sys/select.h>
#include <sys/signal.h>
#endif /* AIX41 */

#ifdef LINUX
#include <asm/mman.h>
#include <asm/sigcontext.h>
#include <sys/fcntl.h>
#endif

typedef void (*void_func_handler)();

#ifndef ON
#define OFF 0
#define ON 1
#endif

#define HMIG 0
#define ADWD 1
#define BROADCAST 2
#define LOADBAL 3
#define WVEC 4

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif /* TRUE */

#ifndef MAX
#define MAX(x, y) (((x) >= (y)) ? (x) : (y))
#endif /* MAX */

#ifndef MIN
#define MIN(x, y) (((x) >= (y)) ? (y) : (x))
#endif /* MIN */

#define SPACE(right)                                                           \
  {                                                                            \
    if ((right) == 1)                                                          \
      printf("\t\t\t");                                                        \
  }

#endif /* JIAGLOBAL_H */
