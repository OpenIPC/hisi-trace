#include "ProcessMemory.h"
#include "Common.h"

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <errno.h>
#include <sys/ptrace.h>
#include <sys/user.h>


ProcessMemory::ProcessMemory(pid_t pid)
    : m_pid(pid) {
    attach(m_pid);
}

ProcessMemory::~ProcessMemory() {
    detach(m_pid);
}

// from https://github.com/scanmem/scanmem/blob/master/ptrace.c
bool ProcessMemory::attach(pid_t target) {
    char mem[32];

    /* print the path to mem file */
    snprintf(mem, sizeof(mem), "/proc/%d/mem", target);

    /* attempt to open the file */
    if ((m_memFd = open(mem, O_RDWR)) == -1) {
        FATAL("unable to open " << mem);
        return false;
    }

    return true;
}

bool ProcessMemory::detach(pid_t) {
    /* close the mem file before detaching */
    close(m_memFd);
    return true;
}

std::string ProcessMemory::readStr(const char *target_address) {
    if (target_address == NULL)
        return "";

    char sym;
    uint8_t* buf = reinterpret_cast<uint8_t*>(&sym);
    std::string result;

    for (size_t i = 0 ; ; i++) {
        if (read(buf, target_address + i, 1) != 1)
            return 0;

        if (sym == 0)
            return result;

        result += sym;
    }

    return result;
}

/* Reads data from the target process, and places it on the `dest_buffer`
 * using either `ptrace` or `pread` on `/proc/pid/mem`.
 * `sm_attach()` MUST be called before this function. */
size_t ProcessMemory::read(uint8_t *dest_buffer, const char *target_address, size_t size) {
    size_t nread = 0;

    do {
        ssize_t ret = pread(m_memFd, dest_buffer + nread,
                            size - nread, (unsigned long)(target_address + nread));
        if (ret == -1) {
            /* we can't read further, report what was read */
            return nread;
        }
        else {
            /* some data was read */
            nread += ret;
        }
    } while (nread < size);

    return nread;
}

bool ProcessMemory::writeArray(void *addr, const void *data, size_t len) {
    unsigned int i, j;
    long peek_value;
    char* chAddr = (char*) addr;
    char* chData = (char*) data;

    for (i = 0; i + sizeof(long) < len; i += sizeof(long)) {
        if (ptrace(PTRACE_POKEDATA, m_pid, chAddr + i, *(long *)(chData + i)) == -1L) {
            FATAL("failed.\n" << __func__);
            return false;
        }
    }

    if (len - i > 0) { /* something left (shorter than a long) */
        if (len > sizeof(long)) { /* rewrite last sizeof(long) bytes of the buffer */
            if (ptrace(PTRACE_POKEDATA, m_pid, chAddr + len - sizeof(long), *(long *)(chData + len - sizeof(long))) == -1L) {
                FATAL("failed.\n" << __func__);
                return false;
            }
        } else { /* we have to play with bits... */
            /* try all possible shifting read and write */
            for(j = 0; j <= sizeof(long) - (len - i); ++j) {
                errno = 0;

                if(((peek_value = ptrace(PTRACE_PEEKDATA, m_pid, chAddr - j, NULL)) == -1L) && (errno != 0)) {
                    if (errno == EIO || errno == EFAULT) /* may try next shift */
                        continue;
                    else {
                        FATAL("failed.\n" <<  __func__);
                        return false;
                    }
                } else { /* peek success */
                    /* write back */
                    memcpy(((int8_t*)&peek_value)+j, chData+i, len-i);

                    if (ptrace(PTRACE_POKEDATA, m_pid, chAddr - j, peek_value) == -1L) {
                        FATAL("failed.\n" << __func__);
                        return false;
                    }

                    break;
                }
            }
        }
    }

    return true;
}
