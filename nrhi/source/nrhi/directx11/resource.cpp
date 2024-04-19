#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/buffer.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

    F_directx11_resource::F_directx11_resource(
        TK_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc
    ) :
        A_resource(device_p, initial_data, desc)
    {
    }
    F_directx11_resource::F_directx11_resource(
        TK_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        ID3D11Resource* d3d11_resource_p
    ) :
        A_resource(device_p, initial_data, desc),
        d3d11_resource_p_(d3d11_resource_p)
    {

    }
    F_directx11_resource::~F_directx11_resource() {

        if(d3d11_resource_p_)
            d3d11_resource_p_->Release();
    }



    TU<A_resource> HD_directx11_resource::create(
        TK_valid<A_device> device_p,
        const F_initial_resource_data& initial_resource_data,
        const F_resource_desc& desc
    ){

        switch (desc.type) {
            case E_resource_type::BUFFER:
                return TU<F_directx11_buffer>()(device_p, initial_resource_data, desc);
            case E_resource_type::STRUCTURED_BUFFER:
                return TU<F_directx11_buffer>()(device_p, initial_resource_data, desc);
            case E_resource_type::SINGLE_ELEMENTED_BUFFER:
                return TU<F_directx11_buffer>()(device_p, initial_resource_data, desc);
        }
        return null;
    }
    U_buffer_handle HD_directx11_resource::create_buffer(
        TK_valid<A_device> device_p,
        const F_initial_resource_data& initial_resource_data,
        const F_resource_desc& desc
    )  {

        return { TU<F_directx11_buffer>()(device_p, initial_resource_data, desc) };
    }
    U_buffer_handle HD_directx11_resource::create_structured_buffer(
        TK_valid<A_device> device_p,
        const F_initial_resource_data& initial_resource_data,
        const F_resource_desc& desc
    ) {

        return { TU<F_directx11_buffer>()(device_p, initial_resource_data, desc) };
    }
    U_buffer_handle HD_directx11_resource::create_single_elemented_buffer(
        TK_valid<A_device> device_p,
        const F_initial_resource_data& initial_resource_data,
        const F_resource_desc& desc
    ) {

        return { TU<F_directx11_buffer>()(device_p, initial_resource_data, desc) };
    }

}