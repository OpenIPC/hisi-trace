#ifndef ICALL_DELEGATE_H__
#define ICALL_DELEGATE_H__

#include "CallParams.h"

class ProcessMemory;

class ICallDelegate {
public:
    virtual ~ICallDelegate() { }

    virtual CallParams* createCallParams() = 0;

    virtual bool onCallStart(CallParams*, ProcessMemory*) = 0;
    virtual void onCallEnd(CallParams*, ProcessMemory*) = 0;

    virtual bool needEmptyCall(CallParams*, ProcessMemory*) = 0;
    virtual bool needResultReplace(CallParams* cp, ProcessMemory* pm) = 0;
};

#endif // ICALL_DELEGATE_H__
