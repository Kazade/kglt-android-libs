#ifndef IDLE_TASK_MANAGER_H
#define IDLE_TASK_MANAGER_H

#include <cstdint>
#include <functional>
#include <map>
#include <mutex>
#include <condition_variable>

namespace kglt {

typedef uint32_t ConnectionID;
class WindowBase;

class IdleTaskManager {
public:
    IdleTaskManager(WindowBase& window);

    ConnectionID add(std::function<bool ()> callback);
    ConnectionID add_once(std::function<void ()> callback);
    ConnectionID add_timeout(float seconds, std::function<void()> callback);
    
    void run_sync(std::function<void()> callback);

    void remove(ConnectionID connection);

    void execute();

    void wait();

private:
    typedef std::map<ConnectionID, std::function<bool ()> > SignalMap;
    typedef std::map<ConnectionID, std::function<void ()> > SignalOnceMap;

    WindowBase& window_;

    SignalMap signals_;
    SignalOnceMap signals_once_;

    std::mutex signals_mutex_;
    std::mutex signals_once_mutex_;

    std::mutex cv_mutex_;
    std::condition_variable cv_;
};

}
#endif // IDLE_TASK_MANAGER_H
