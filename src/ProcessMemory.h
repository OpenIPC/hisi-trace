#ifndef PROCESS_MEMORY_H
#define PROCESS_MEMORY_H

#include <sys/wait.h>
#include <stdint.h>
#include <string>

#include "CallParams.h"

class ProcessMemory {
public:
    ProcessMemory(pid_t pid);
    ~ProcessMemory();

    std::string strArg(TReg reg) {
        return readStr(asAddr(reg));
    }

    static char* asAddr(TReg r) {
        intptr_t strPtr = static_cast<intptr_t>(r);
        return reinterpret_cast<char*>(strPtr);
    }

    size_t read(uint8_t *dest_buffer, const char *target_address, size_t size);
    std::string readStr(const char *target_address);

    bool writeArray(void *addr, const void *data, size_t len);

private:
    bool attach(pid_t target);
    bool detach(pid_t target);

    pid_t m_pid;
    int m_memFd;
};

#endif // PROCESS_MEMORY_H
