
if(NOT DEFINED LOGCONF_PREFIX)
    set(LOGCONF_PREFIX "/var/log")
endif()

configure_file(logrotate.conf.in ${PROJECT_BINARY_DIR}/logrotate.conf)
configure_file(rsyslog.conf.in ${PROJECT_BINARY_DIR}/rsyslog.conf)