#ifndef HANDLER_WATCHER_H
#define HANDLER_WATCHER_H

#include <list>
#include <set>
#include <string>

class CallParams;
class ProcessMemory;

class HandlerWatcher {
public:
    struct Handler {
        Handler(const std::string&, int);

        std::string m_name;
        int m_handler;
    };

    HandlerWatcher();
    ~HandlerWatcher();

    bool onOpenStart(CallParams*, ProcessMemory*);
    void onOpenEnd(CallParams*, ProcessMemory*);

    bool onIoctlStart(CallParams*, ProcessMemory*);
    void onIoctlEnd(CallParams*, ProcessMemory*);

    bool needResultReplaceOpen(CallParams*, ProcessMemory*);
    bool needResultReplaceIoctl(CallParams*, ProcessMemory*);

private:
    bool isWatched(int fd) const;

    std::list<Handler> m_handlers;
    std::set<std::string> m_watchDb;
    std::string m_name;
    bool m_needWatch;
    bool m_needIoctlNewRet;
};


#endif // HANDLER_WATCHER_H
