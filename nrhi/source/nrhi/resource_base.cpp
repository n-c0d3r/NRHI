#include <nrhi/resource_base.hpp>



namespace nrhi {

    A_resource::A_resource(TK_valid<A_device> device_p, const F_resource_desc& desc) :
        device_p_(device_p),
        desc_(desc)
    {

    }
    A_resource::~A_resource() {

    }

}