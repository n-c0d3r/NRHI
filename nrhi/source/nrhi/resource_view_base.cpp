#include <nrhi/resource_view_base.hpp>



namespace nrhi {

    A_resource_view::A_resource_view(
        TK_valid<A_device> device_p,
        TK_valid<A_resource> resource_p,
        E_resource_view_type type
    ) :
        device_p_(device_p),
        resource_p_(resource_p),
        type_(type)
    {

    }
    A_resource_view::~A_resource_view() {

    }

}