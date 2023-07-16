INCLUDE(CheckCXXSourceCompiles)
include(CMakePushCheckState)
set(ATOMIC32_TEST_CODE "
    #include <atomic>
    #include <stdint.h>
    int main() {
        std::atomic<int32_t> x;
        x.store(1);
        x--;
        return x.load();
    }")

set(ATOMIC64_TEST_CODE "
    #include <atomic>
    #include <stdint.h>
    int main() {
        std::atomic<int64_t> x;
        x.store(1);
        x--;
        return x.load();
    }")

macro(ATOMIC_CHECK)

    # test whether atomic works
    check_cxx_source_compiles("${ATOMIC32_TEST_CODE}" atomic32_test)
    check_cxx_source_compiles("${ATOMIC64_TEST_CODE}" atomic64_test)

    # if doesn't work, attempt to find the atomic library, link with it and try again
    if(NOT atomic32_test OR NOT atomic64_test)
        find_library(ATOMIC NAMES libatomic.so.1
        HINTS
          $ENV{HOME}/local/lib64
          $ENV{HOME}/local/lib
          /usr/local/lib64
          /usr/local/lib
          /opt/local/lib64
          /opt/local/lib
          /usr/lib64
          /usr/lib
          /lib64
          /lib
          /usr/lib/arm-linux-gnueabihf
        )

        if(ATOMIC)
            set(LIBATOMIC ${ATOMIC})
            message(STATUS "Found libatomic: ${LIBATOMIC}")
            message(STATUS "Attempting to test atomic with atomic library linked")

            get_filename_component(atomic_lib_dir ${LIBATOMIC} DIRECTORY)

            # Before setting CMAKE_REQUIRED_FLAGS, we preserve the current state
            cmake_push_check_state()

            set(CMAKE_REQUIRED_LIBRARIES "-L${atomic_lib_dir}" "-latomic")
            check_cxx_source_compiles("${ATOMIC32_TEST_CODE}" atomic32_test_with_atomic_linking)
            check_cxx_source_compiles("${ATOMIC64_TEST_CODE}" atomic64_test_with_atomic_linking)

            cmake_pop_check_state()

            if(NOT atomic32_test_with_atomic_linking)
                message(FATAL_ERROR "Even after linking with the atomic library, atomic 32-bit compilation failed.")
            endif()

            if(NOT atomic64_test_with_atomic_linking)
                message(FATAL_ERROR "Even after linking with the atomic library, atomic 64-bit compilation failed.")
            endif()

            set(ATOMIC_LINKER_LIBS "-L${atomic_lib_dir} -latomic")
        else()
            message(FATAL_ERROR "Failed to find libatomic even though it seems to be required")
        endif()
    endif()

endmacro()

ATOMIC_CHECK()