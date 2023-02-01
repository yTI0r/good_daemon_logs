#ifndef SYSLOG_HPP
#define SYSLOG_HPP

#include <syslog.h>

#include <streambuf>
#include "logpriority.h"

std::ostream& operator<<(std::ostream& os, const LogPriority& log_priority);

class Log : public std::basic_streambuf<char, std::char_traits<char> > {
 public:
  explicit Log(std::string ident, int facility, int level,
               bool console = true) {
    facility_ = facility;
    priority_ = LOG_DEBUG;
    strncpy(ident_, ident.c_str(), sizeof(ident_));
    ident_[sizeof(ident_) - 1] = '\0';
    openlog(ident_, LOG_PID | (console?LOG_CONS:0), facility_);
    setlogmask(LOG_UPTO(level));
  }
  ~Log(){
    closelog();
  }
 protected:
  int sync() {
    if (buffer_.length()) {
      syslog(priority_, "%s", buffer_.c_str());
      buffer_.erase();
      priority_ = LOG_DEBUG;  // default to debug for each message
    }
    return 0;
  }
  int overflow(int c) {
    if (c != EOF) {
      buffer_ += static_cast<char>(c);
    } else {
      sync();
    }
    return c;
  }

 private:
  friend std::ostream& operator<<(std::ostream& os,
                                  const LogPriority& log_priority) {
    static_cast<Log*>(os.rdbuf())->priority_ = (int)log_priority;
    return os;
  }
  std::string buffer_;
  int facility_;
  int priority_;
  bool console_;
  char ident_[50];
};
#endif  // SYSLOG_HPP