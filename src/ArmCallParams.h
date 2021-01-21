#ifndef ARM_CALL_PARAMS_H__
#define ARM_CALL_PARAMS_H__

#include "CallParams.h"

#include <asm/ptrace.h>

class ArmCallParams : public CallParams {
public:
    TReg syscall() const;
    TReg retVal() const;
    TReg arg(int i) const;
    void* native();
    void setSyscall(int);
    void setRetVal(TReg);

    void dump();

private:
    pt_regs m_regs;
};

#endif // ARM_CALL_PARAMS_H__
