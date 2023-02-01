# Good daemon tools

It's a tool set for daemon loging.

Headers:

* C++ style Syslog wraper `syslog.hpp`
  * init:
  ```c++
  std::clog.rdbuf(new Log("ProgramName", Facility, LogLevel));
  ```
* C++ pid file lock `pidlock.h`
  * Usage: 
  ```c++
  try {
    PidLock Lock(pidFileName.c_str());
  } catch (const std::exception& e) {
    std::clog << kLogErr << e.what() << std::endl;
    return -1;
  }
  ```

CMAKE:

* rsyslog and logrotate config templates
