#ifndef CALL_PARAMS_H__
#define CALL_PARAMS_H__

typedef long TReg;

class CallParams {
public:
    virtual ~CallParams() { }

    virtual TReg syscall() const = 0;
    virtual TReg retVal() const = 0;
    virtual TReg arg(int i) const = 0;

    virtual void setSyscall(int) = 0;
    virtual void setRetVal(TReg) = 0;

    virtual void dump() = 0;

    virtual void* native() = 0;
};

#endif // CALL_PARAMS_H__
