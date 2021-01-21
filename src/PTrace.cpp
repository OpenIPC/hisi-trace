#include "PTrace.h"
#include "Common.h"
#include "CallParams.h"
#include "ICallDelegate.h"
#include "ProcessMemory.h"

#include <errno.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <iostream>

#define SHOW_CALL_STACK 0

PTrace::PTrace(ICallDelegate* cd, char **argv)
    : m_isParent(false), m_callDelegate(cd) {
    m_pid = fork();

    switch (m_pid) {
    case -1:
        FATAL(strerror(errno));
    case 0:  /* child */
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execvp(argv[1], argv + 1);
        return;
    }

    m_isParent = true;
}

PTrace::~PTrace() {
}

void PTrace::doWork() {
    if (!m_isParent)
        return;

    /* parent */
    waitpid(m_pid, 0, 0); // sync with execvp

    ProcessMemory* pm = new ProcessMemory(m_pid);
    CallParams *cp = m_callDelegate->createCallParams();

    for (unsigned long i = 0 ; ; i++) {
        /* Enter next system call */
        if (ptrace(PTRACE_SYSCALL, m_pid, 0, 0) == -1)
            FATAL(strerror(errno));

        if (waitpid(m_pid, 0, 0) == -1)
            FATAL(strerror(errno));

        /* Gather system call arguments */
        if (ptrace(PTRACE_GETREGS, m_pid, 0, cp->native()) == -1)
            FATAL(strerror(errno));

#if SHOW_CALL_STACK
        std::cout << cp->syscall() << " [" << cp->arg(0) << " : " << cp->arg(1) << "] = ";
#endif
        const bool callChanged = m_callDelegate->onCallStart(cp, pm);
        const bool needEmptyCall = m_callDelegate->needEmptyCall(cp, pm);
        const bool syscallChanged = callChanged || needEmptyCall;

        if (needEmptyCall) {
            // set syscall to invalid number
            cp->setSyscall(-1);
        }

        if (syscallChanged) {
            // attach new regs
            if (ptrace(PTRACE_SETREGS, m_pid, 0, cp->native()) == -1)
                FATAL(strerror(errno));
        }

        /* Run system call and stop on exit */
        if (ptrace(PTRACE_SYSCALL, m_pid, 0, 0) == -1)
            FATAL(strerror(errno));

        if (waitpid(m_pid, 0, 0) == -1)
            FATAL(strerror(errno));

        const bool newResult = m_callDelegate->needResultReplace(cp, pm);

        if (newResult) {
            // set new result value
            if (ptrace(PTRACE_SETREGS, m_pid, 0, cp->native()) == -1)
                FATAL(strerror(errno));
        }

        /* Get system call result */
        if (ptrace(PTRACE_GETREGS, m_pid, 0, cp->native()) == -1)
            FATAL(strerror(errno));

#if SHOW_CALL_STACK
        std::cout << cp->retVal() << std::dec << std::endl;
#endif

        m_callDelegate->onCallEnd(cp, pm);
    }

    delete cp;
    delete pm;
}
