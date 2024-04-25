#include <nrhi/resource_view_base.hpp>



namespace nrhi {

    A_resource_view::A_resource_view(
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc,
        E_resource_view_type overrided_type
    ) :
        device_p_(device_p),
        desc_(desc)
    {

        desc_.type = overrided_type;
    }
    A_resource_view::~A_resource_view() {

    }

}