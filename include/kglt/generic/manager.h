#ifndef MANAGER_H
#define MANAGER_H

#include "manager_base.h"

#include <kazbase/list_utils.h>
#include <kazbase/signals.h>

namespace kglt {
namespace generic {

template<typename Derived, typename ObjectType, typename ObjectIDType, typename NewIDGenerator=IncrementalGetNextID<ObjectIDType> >
class TemplatedManager {
protected:
    mutable std::recursive_mutex manager_lock_;

public:
    typedef NewIDGenerator GeneratorType;
    typedef ObjectType Type;

    ObjectIDType manager_new() {
        return manager_new(ObjectIDType());
    }

    template<typename... Args>
    ObjectIDType manager_new(ObjectIDType id, Args&&... args) {
        {
            std::lock_guard<std::recursive_mutex> lock(manager_lock_);
            if(!id) {
                id = generator_();
            }
            objects_.insert(std::make_pair(id, ObjectType::create((Derived*)this, id, std::forward<Args>(args)...)));
        }

        signal_post_create_(*objects_[id], id);

        return id;
    }

    void manager_delete_all() {
        for(auto p: objects_) {
            signal_pre_delete_(*p.second, p.first);
        }

        objects_.clear();
    }

    void manager_delete(ObjectIDType id) {
        if(manager_contains(id)) {
            std::lock_guard<std::recursive_mutex> lock(manager_lock_);

            ObjectType& obj = *objects_[id];
            signal_pre_delete_(obj, id);

            if(manager_contains(id)) {
                objects_.erase(id);
            }
        }
    }

    uint32_t manager_count() const {
        return objects_.size();
    }

    std::weak_ptr<ObjectType> manager_get(ObjectIDType id) {
        std::lock_guard<std::recursive_mutex> lock(manager_lock_);

        auto it = objects_.find(id);
        if(it == objects_.end()) {
            throw DoesNotExist<ObjectType>(typeid(ObjectType).name() + _u(" with ID: {0}").format(id));
        }
        return it->second;
    }

    std::weak_ptr<ObjectType> manager_get(ObjectIDType id) const {
        std::lock_guard<std::recursive_mutex> lock(manager_lock_);

        auto it = objects_.find(id);
        if(it == objects_.end()) {
            throw DoesNotExist<ObjectType>(typeid(ObjectType).name() + _u(" with ID: {0}").format(id));
        }
        return it->second;
    }

    bool manager_contains(ObjectIDType id) const {
        return objects_.find(id) != objects_.end();
    }

    sig::signal<void (ObjectType&, ObjectIDType)>& signal_post_create() { return signal_post_create_; }
    sig::signal<void (ObjectType&, ObjectIDType)>& signal_pre_delete() { return signal_pre_delete_; }

    template<typename Func>
    void apply_func_to_objects(Func func) {
        for(std::pair<ObjectIDType, typename ObjectType::ptr> p: objects_) {
            auto thing = manager_get(p.first); //Make sure we lock the object
            std::bind(func, thing.lock().get())();
        }
    }

    //Internal!
    std::unordered_map<ObjectIDType, std::shared_ptr<ObjectType> >& __objects() {
        return objects_;
    }
private:
    sig::signal<void (ObjectType&, ObjectIDType)> signal_post_create_;
    sig::signal<void (ObjectType&, ObjectIDType)> signal_pre_delete_;

    static NewIDGenerator generator_;

protected:
    std::unordered_map<ObjectIDType, std::shared_ptr<ObjectType> > objects_;

    ObjectIDType _get_object_id_from_ptr(ObjectType* ptr) {
        for(std::pair<ObjectIDType, std::shared_ptr<ObjectType> > pair: objects_) {
            if(pair.second.get() == ptr) {
                return pair.first;
            }
        }

        return ObjectIDType();
    }
};

template <typename Derived, typename ObjectType, typename ObjectIDType, typename NewIDGenerator>
NewIDGenerator TemplatedManager<Derived, ObjectType, ObjectIDType, NewIDGenerator>::generator_;


}
}
#endif // MANAGER_H
