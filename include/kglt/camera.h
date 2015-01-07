#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "kazmath/mat4.h"
#include "generic/identifiable.h"
#include "generic/managed.h"
#include "generic/protected_ptr.h"

#include "utils/parent_setter_mixin.h"

#include "object.h"
#include "frustum.h"
#include "renderer.h"
#include "sound.h"

namespace kglt {

class Camera;

class CameraProxy:
    public ParentSetterMixin<Object>,
    public generic::Identifiable<CameraID>,
    public Managed<CameraProxy>,
    public Protectable {

public:
    CameraProxy(Stage* stage, CameraID camera_id);
    ~CameraProxy();

    void follow(ActorID actor, const kglt::Vec3& offset, float lag_in_seconds=1.0);
    void ask_owner_for_destruction();

    void _update_following(double dt);

    Frustum& frustum();
    kmVec3 project_point(const RenderTarget &target, const Viewport& viewport, const kmVec3& point);

    void set_orthographic_projection(double left, double right, double bottom, double top, double near=-1.0, double far=1.0);

    unicode __unicode__() const {
        if(has_name()) {
            return name();
        } else {
            return _u("Camera {0}").format(this->id());
        }
    }
private:
    ActorID following_actor_;
    Vec3 following_offset_;
    float following_lag_ = 0.0;

    void do_update(double dt);


    CameraPtr camera();
};

class Camera:
    public generic::Identifiable<CameraID>,
    public Managed<Camera>,
    public Protectable {

public:
    Camera(WindowBase* window, CameraID id);

    kmVec3 project_point(const RenderTarget& target, const Viewport& viewport, const kmVec3& point);

    const Mat4& view_matrix() { return view_matrix_; }
    const Mat4& projection_matrix() const { return projection_matrix_; }

    Frustum& frustum() { return frustum_; }

    void set_perspective_projection(double fov, double aspect, double near=1.0, double far=1000.0f);
    void set_orthographic_projection(double left, double right, double bottom, double top, double near=-1.0, double far=1.0);
    double set_orthographic_projection_from_height(double desired_height_in_units, double ratio);

    void set_transform(const kglt::Mat4& transform);

    bool has_proxy() const { return bool(proxy_); }
    void set_proxy(CameraProxy* proxy) { proxy_ = proxy; }

    CameraProxy& proxy() {
        assert(proxy_);
        return *proxy_;
    }

    const Mat4& transform() const { return transform_; }
private:
    WindowBase* window_;
    CameraProxy* proxy_;

    Frustum frustum_;

    kglt::Mat4 transform_;
    Mat4 view_matrix_;
    Mat4 projection_matrix_;

    void update_frustum();
};

}


#endif // CAMERA_H_INCLUDED
