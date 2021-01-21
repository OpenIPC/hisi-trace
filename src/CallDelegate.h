#ifndef CALL_DELEGATE_H__
#define CALL_DELEGATE_H__

#include "ICallDelegate.h"

class HandlerWatcher;

class CallDelegate : public ICallDelegate {
public:
    CallDelegate();
    ~CallDelegate();

    CallParams* createCallParams();

    bool onCallStart(CallParams*, ProcessMemory*);
    void onCallEnd(CallParams*, ProcessMemory*);

    bool needEmptyCall(CallParams*, ProcessMemory*);

    bool needResultReplace(CallParams* cp, ProcessMemory* pm);
private:

    HandlerWatcher *m_handlers;
    int m_cachedSyscall;
};

#endif // ICALL_DELEGATE_H__
