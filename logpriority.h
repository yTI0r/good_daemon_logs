#ifndef LOGPRIORITY_H
#define LOGPRIORITY_H

#include <syslog.h>

enum LogPriority {
  kLogEmerg = LOG_EMERG,      // system is unusable
  kLogAlert = LOG_ALERT,      // action must be taken immediately
  kLogCrit = LOG_CRIT,        // critical conditions
  kLogErr = LOG_ERR,          // error conditions
  kLogWarning = LOG_WARNING,  // warning conditions
  kLogNotice = LOG_NOTICE,    // normal, but significant, condition
  kLogInfo = LOG_INFO,        // informational message
  kLogDebug = LOG_DEBUG       // debug-level message
};
#endif //LOGPRIORITY_H