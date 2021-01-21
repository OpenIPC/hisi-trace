#include "Common.h"
#include "PTrace.h"
#include "CallDelegate.h"

int main(int argc, char **argv) {
    if (argc <= 1) {
        FATAL("too few arguments: " << argc);
    }

    CallDelegate cd;
    PTrace trace(&cd, argv);
    trace.doWork();

    return 0;
}
