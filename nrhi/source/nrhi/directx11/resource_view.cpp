#include <nrhi/directx11/resource_view.hpp>
#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/shader_resource_view.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

    F_directx11_resource_view::F_directx11_resource_view(
        TK_valid<A_device> device_p,
        const F_resource_view_desc& desc,
        E_resource_view_type overrided_type,
        ID3D11View* d3d11_view_p
    ) :
        A_resource_view(device_p, desc, overrided_type),
        d3d11_view_p_(d3d11_view_p)
    {

    }
    F_directx11_resource_view::~F_directx11_resource_view() {

        if(d3d11_view_p_)
            d3d11_view_p_->Release();
    }



    U_srv_handle HD_directx11_resource_view::create_srv(
        TK_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ){

        return { TU<F_directx11_shader_resource_view>()(device_p, desc) };
    }
    U_uav_handle HD_directx11_resource_view::create_uav(
        TK_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ){

        return {};
    }
    U_rtv_handle HD_directx11_resource_view::create_rtv(
        TK_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ){

        return {};
    }
    U_dsv_handle HD_directx11_resource_view::create_dsv(
        TK_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ){

        return {};
    }
    U_sampler_handle HD_directx11_resource_view::create_sampler(
        TK_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ){

        return {};
    }

}