#ifndef PTRACE_H__
#define PTRACE_H__

#include <sys/wait.h>

class ICallDelegate;

class PTrace {
public:
    PTrace(ICallDelegate*, char **argv);
    ~PTrace();

    void doWork();

private:
    bool m_isParent;
    pid_t m_pid;
    ICallDelegate* m_callDelegate;
};

#endif // PTRACE_H__
