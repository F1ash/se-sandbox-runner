#ifndef PGREP_H
#define PGREP_H


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <pwd.h>
#include <grp.h>
#include <regex.h>
#include <errno.h>

#include "/usr/include/proc/readproc.h"
#include "/usr/include/proc/sig.h"
#include "/usr/include/proc/devname.h"
#include "/usr/include/proc/sysinfo.h"
#include "/usr/include/proc/version.h" /* procps_version */

char* pgrep (char *argv);

#endif // PGREP_H
