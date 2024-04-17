#include <nrhi/directx11/resource_view.hpp>



namespace nrhi {

    F_directx11_resource_view::F_directx11_resource_view(
        TK_valid<A_device> device_p,
        TK_valid<A_resource> resource_p,
        E_resource_view_type type
    ) :
        A_resource_view(device_p, resource_p, type)
    {

    }
    F_directx11_resource_view::~F_directx11_resource_view() {

    }



    TU<A_resource_view> HD_directx11_resource_view::create_srv(TK_valid<A_device> device_p, TK_valid<A_resource> resource_p){

        return {};
    }
    TU<A_resource_view> HD_directx11_resource_view::create_uav(TK_valid<A_device> device_p, TK_valid<A_resource> resource_p){

        return {};
    }
    TU<A_resource_view> HD_directx11_resource_view::create_rtv(TK_valid<A_device> device_p, TK_valid<A_resource> resource_p){

        return {};
    }
    TU<A_resource_view> HD_directx11_resource_view::create_dsv(TK_valid<A_device> device_p, TK_valid<A_resource> resource_p){

        return {};
    }
    TU<A_resource_view> HD_directx11_resource_view::create_sampler(TK_valid<A_device> device_p, TK_valid<A_resource> resource_p) {

        return {};
    }

}