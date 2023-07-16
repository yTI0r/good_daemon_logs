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
* `checkatomic.cmake` - Resolve problem of usage libatomic on different platforms espeshaly on Raspberry. Linker error: `undefined reference to '__atomic_store_8'` - can be resolved by this script. 
  * Usage. In CMakeLists.txt - add 

  ```cmake
  include(checkatomic.cmake)
  if(ATOMIC_LINKER_LIBS)
  target_link_libraries(YOUR_TARGET
    ${ATOMIC_LINKER_LIBS}
  )
  endif()
  ```
