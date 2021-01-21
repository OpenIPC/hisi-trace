#include "HandlerWatcher.h"
#include "CallParams.h"
#include "ProcessMemory.h"
#include "Common.h"

#include <iostream>
#include <algorithm>

#define XMMTD_GETLOCKVERSION    0x40044da0
#define XMMTD_SETPASSWD         0x40044da1
#define XMMTD_SETPROTECTFLAG    0x40044dbe
#define XMMTD_GETPROTECTFLAG    0x40044dbf

static __U32_TYPE g_mtdVersion = 0x1001;

struct HandlerFdComparator {
    HandlerFdComparator(int fd) : m_fd(fd) {
    }

    bool operator()(const HandlerWatcher::Handler& obj) {
        return obj.m_handler == m_fd;
    }

    int m_fd;
};

HandlerWatcher::Handler::Handler(const std::string& n, int f) :
    m_name(n), m_handler(f) {
}

HandlerWatcher::HandlerWatcher() {
    m_watchDb.insert("/proc/mtd");
    m_watchDb.insert("/dev/mtd");
    m_watchDb.insert("/dev/mtd0");
    m_watchDb.insert("/dev/mtd1");
    m_watchDb.insert("/dev/mtd2");
    m_watchDb.insert("/dev/mtd3");
    m_watchDb.insert("/dev/mtd4");
    m_watchDb.insert("/dev/mtd5");
}

HandlerWatcher::~HandlerWatcher() {
}

bool HandlerWatcher::isWatched(int fd) const {
    return std::find_if(m_handlers.begin(), m_handlers.end(), HandlerFdComparator(fd)) != m_handlers.end();
}

static bool needCmdWatch(unsigned long cmd) {
    return (cmd == XMMTD_GETLOCKVERSION) ||
            (cmd == XMMTD_SETPASSWD) ||
            (cmd == XMMTD_GETPROTECTFLAG) ||
            (cmd == XMMTD_SETPROTECTFLAG);
}

bool HandlerWatcher::onIoctlStart(CallParams* cp, ProcessMemory*) {
    const int fd = cp->arg(0);
    unsigned long cmd = cp->arg(1);
    m_needIoctlNewRet = isWatched(fd) && needCmdWatch(cmd);
    return false;
}

bool HandlerWatcher::needResultReplaceIoctl(CallParams* cp, ProcessMemory* pm) {
    if (!m_needIoctlNewRet)
        return false;

    const unsigned long cmd = cp->arg(1);

    switch(cmd) {
    case XMMTD_GETLOCKVERSION:
        pm->writeArray(ProcessMemory::asAddr(cp->arg(2)), &g_mtdVersion, sizeof(g_mtdVersion));
    case XMMTD_SETPASSWD:
    case XMMTD_GETPROTECTFLAG:
    case XMMTD_SETPROTECTFLAG: {
        cp->setRetVal(0);
        return true;
    }

    }
    return false;
}

void HandlerWatcher::onIoctlEnd(CallParams*, ProcessMemory*) {
}

static void replaceFname3Sym(TReg addr, ProcessMemory* pm, char sym) {
    pm->writeArray(ProcessMemory::asAddr(addr) + 3, &sym, 1);
}

bool HandlerWatcher::onOpenStart(CallParams* cp, ProcessMemory* pm) {
    m_name = pm->strArg(cp->arg(0));
    m_needWatch = (m_watchDb.find(m_name) != m_watchDb.end());

    if (m_needWatch) {
        replaceFname3Sym(cp->arg(0), pm, 'i');
    }

    return false;
}

bool HandlerWatcher::needResultReplaceOpen(CallParams*, ProcessMemory*) {
    return false;
}

void HandlerWatcher::onOpenEnd(CallParams* cp, ProcessMemory*) {
    if (!m_needWatch)
        return;

    m_handlers.push_back(Handler(m_name, cp->retVal()));
}
