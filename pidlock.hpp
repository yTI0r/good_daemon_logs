#ifndef PID_SET_H
#define PID_SET_H

#include <fcntl.h>     //open
#include <sys/stat.h>  //exist
#include <unistd.h>    //close

#include <cerrno>   //errno
#include <cstring>  //strerror
#include <stdexcept>
#include <string>

class PidLock {
 private:
  int lockFD;
  const char* lockFileName;

 public:
  PidLock(const char* FileName) {
    lockFileName = FileName;
    lockFD = open(lockFileName, O_RDWR | O_CREAT | O_EXCL, 0644);

    if (lockFD == -1)  //Или создать не удалось или файл уже существует.
    {
      if (errno != EEXIST) {
        //Это не ошибка существования файла, а значит Game over.
        throw std::runtime_error(std::string("Can't create lock file:") +
                                 std::strerror(errno) +
                                 " path: " + lockFileName);
      }

      /* The lock file already exists. Try to open it */

      std::FILE* lfp = fopen(lockFileName, "r");  //Попробуем открыть на чтение.

      if (lfp == 0) /* Game over */
      {
        throw std::runtime_error(std::string(lockFileName) +
                                 " - Lock file exist. Can't open it.");
      }
      /* Файл открылся, похоже демон исполняется.
                      Прочитаем строку с ПИД из блок файла. */
      char pidBuf[17];
      if (!fgets(pidBuf, 16, lfp)) {
        fclose(lfp);
        throw std::runtime_error(std::string(lockFileName) +
                                 " - Lock file exist. Can't read it.");
      }

      int len = (int)strlen(pidBuf);
      if (pidBuf[len - 1] == '\n') /* strip linefeed */
      {
        pidBuf[len - 1] = 0;
      }

      throw std::runtime_error(std::string(lockFileName) +
                               " - Lock file exist. Proc id:" + pidBuf);

      fclose(lfp);
    }
    struct flock exclusiveLock;
    exclusiveLock.l_type = F_WRLCK;                  /* exclusive write lock */
    exclusiveLock.l_whence = SEEK_SET;               /* use start and len */
    exclusiveLock.l_len = exclusiveLock.l_start = 0; /* whole file */
    exclusiveLock.l_pid = 0;                         /* don't care about this */
    int lockResult = fcntl(lockFD, F_SETLK, &exclusiveLock);

    if (lockResult < 0) /* can't get a lock */
    {
      close(lockFD);
      throw std::runtime_error(std::string("Can't lock lockfile! Path:") +
                               lockFileName);
    }
  }
  ~PidLock() {
    close(lockFD);
    unlink(lockFileName);
  }
};

#endif  // !CONF_H
