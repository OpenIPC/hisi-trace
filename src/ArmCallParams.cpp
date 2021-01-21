#include "ArmCallParams.h"

#include <asm/ptrace.h>
#include <iostream>

#define ARM_REGS_COUNT 18

void ArmCallParams::dump() {
    std::cout << "[ " << std::endl;

    for (int i = 0 ; i < ARM_REGS_COUNT ; i++)
        std::cout << m_regs.uregs[i] << " , ";

    std::cout << " ]" << std::endl;
}

TReg ArmCallParams::syscall() const {
    return m_regs.ARM_r7;
}

TReg ArmCallParams::retVal() const {
    return m_regs.ARM_r0;
}

TReg ArmCallParams::arg(int i) const {
    return m_regs.uregs[i];
}

void* ArmCallParams::native() {
    return &m_regs;
}

void ArmCallParams::setSyscall(int n) {
    m_regs.ARM_r7 = n;
}

void ArmCallParams::setRetVal(TReg rv) {
    m_regs.ARM_r0 = rv;
    m_regs.ARM_ORIG_r0 = rv;
}
