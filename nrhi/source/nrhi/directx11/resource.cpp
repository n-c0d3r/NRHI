#include <nrhi/directx11/resource.hpp>



namespace nrhi {

    F_directx11_resource::F_directx11_resource(TK_valid<A_device> device_p, const F_resource_desc& desc) :
        A_resource(device_p, desc)
    {

    }
    F_directx11_resource::~F_directx11_resource() {

    }



    TU<A_resource> HD_directx11_resource::create(TK_valid<A_device> device_p, const F_resource_desc& desc){

        return TU<F_directx11_resource>()(device_p, desc);
    }

}