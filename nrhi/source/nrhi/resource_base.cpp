#include <nrhi/resource_base.hpp>



namespace nrhi {

    A_resource::A_resource(TK_valid<A_device> device_p, const F_resource_desc& desc, b8 is_managed) :
        device_p_(device_p),
        desc_(desc),
        is_managed_(is_managed)
    {

    }
    A_resource::~A_resource() {

    }

}