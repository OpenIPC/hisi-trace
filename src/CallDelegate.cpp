#include "CallDelegate.h"
#include "ArmCallParams.h"
#include "ProcessMemory.h"
#include "HandlerWatcher.h"

#include <syscall.h>
#include <iostream>

CallDelegate::CallDelegate()
    : m_handlers(new HandlerWatcher()) {
}

CallDelegate::~CallDelegate() {
    delete m_handlers;
}

bool CallDelegate::onCallStart(CallParams* cp, ProcessMemory* pm) {
    m_cachedSyscall = cp->syscall();

    switch(m_cachedSyscall) {
    case __NR_open: {
        return m_handlers->onOpenStart(cp, pm);
    }
    case __NR_ioctl: {
        return m_handlers->onIoctlStart(cp, pm);
    }
    }

    return false;
}

void CallDelegate::onCallEnd(CallParams* cp, ProcessMemory* pm) {
    switch(m_cachedSyscall) {
    case __NR_open: {
        return m_handlers->onOpenEnd(cp, pm);
    }
    case __NR_ioctl: {
        return m_handlers->onIoctlEnd(cp, pm);
    }
    }
}

bool CallDelegate::needEmptyCall(CallParams*, ProcessMemory*) {
    return false;
}

bool CallDelegate::needResultReplace(CallParams* cp, ProcessMemory* pm) {
    switch(m_cachedSyscall) {
    case __NR_open: {
        return m_handlers->needResultReplaceOpen(cp, pm);
    }
    case __NR_ioctl: {
        return m_handlers->needResultReplaceIoctl(cp, pm);
    }
    }

    return false;
}

CallParams* CallDelegate::createCallParams() {
    return new ArmCallParams();
}
