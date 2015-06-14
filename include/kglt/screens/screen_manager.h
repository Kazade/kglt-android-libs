#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <kazbase/signals.h>
#include <kazbase/unicode.h>

#include <unordered_map>
#include <functional>
#include <future>

#include "../generic/managed.h"

namespace kglt {

class WindowBase;
class ScreenBase;

typedef std::shared_ptr<ScreenBase> ScreenBasePtr;
typedef std::function<ScreenBasePtr (WindowBase&)> ScreenFactory;

template<typename T>
ScreenFactory screen_factory() {
    ScreenFactory ret = &T::template create<WindowBase&>;
    return ret;
}


class ScreenManagerInterface {
public:
    virtual ~ScreenManagerInterface() {}

    virtual void register_screen(const unicode& route, ScreenFactory factory) = 0;
    virtual bool has_screen(const unicode& route) const = 0;
    virtual ScreenBasePtr resolve_screen(const unicode& route) = 0;
    virtual void activate_screen(const unicode& route) = 0;
    virtual void load_screen_in_background(const unicode& route, bool redirect_after=true) = 0;
    virtual void unload_screen(const unicode& route) = 0;
    virtual bool is_screen_loaded(const unicode& route) const = 0;
    virtual ScreenBasePtr active_screen() const = 0;
};


class ScreenManager :
    public Managed<ScreenManager>,
    public ScreenManagerInterface {

public:
    ScreenManager(WindowBase& window);
    ~ScreenManager();

    void register_screen(const unicode& route, ScreenFactory factory);
    bool has_screen(const unicode& route) const;
    ScreenBasePtr resolve_screen(const unicode& route);
    void activate_screen(const unicode& route);
    void load_screen_in_background(const unicode& route, bool redirect_after=true);
    void unload_screen(const unicode& route);
    bool is_screen_loaded(const unicode& route) const;
    void reset();
    ScreenBasePtr active_screen() const;

private:
    WindowBase& window_;
    std::unordered_map<unicode, std::function<ScreenBasePtr ()>> screen_factories_;
    std::unordered_map<unicode, ScreenBasePtr> routes_;

    ScreenBasePtr current_screen_;

    ScreenBasePtr get_or_create_route(const unicode& route);

    struct BackgroundTask {
        unicode route;
        std::future<void> future;
    };

    sig::connection step_conn_;

    void step(double dt);
};

}

#endif // SCREEN_MANAGER_H
