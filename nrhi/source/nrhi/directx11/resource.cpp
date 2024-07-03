#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/buffer.hpp>
#include <nrhi/directx11/texture.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

    F_directx11_resource::F_directx11_resource(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        ED_resource_type overrided_type
    ) :
        A_resource(device_p, initial_data, desc, overrided_type)
    {
    }
    F_directx11_resource::F_directx11_resource(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        ED_resource_type overrided_type,
        ID3D11Resource* d3d11_resource_p
    ) :
        A_resource(device_p, initial_data, desc, overrided_type),
        d3d11_resource_p_(d3d11_resource_p)
    {

    }
    F_directx11_resource::~F_directx11_resource() {

        if(d3d11_resource_p_)
            d3d11_resource_p_->Release();
    }



    TU<A_resource> HD_directx11_resource::create(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_resource_data,
        const F_resource_desc& desc
    ){
		NRHI_ENUM_SWITCH(
			desc.type,
			NRHI_ENUM_CASE(
                ED_resource_type::BUFFER,
				return TU<F_directx11_buffer>()(device_p, initial_resource_data, desc);
            )
			NRHI_ENUM_CASE(
                ED_resource_type::STRUCTURED_BUFFER,
				return TU<F_directx11_structured_buffer>()(device_p, initial_resource_data, desc);
            )
			NRHI_ENUM_CASE(
                ED_resource_type::INDIRECT_BUFFER,
				return TU<F_directx11_indirect_buffer>()(device_p, initial_resource_data, desc);
            )
		);
        return null;
    }

    U_buffer_handle HD_directx11_resource::create_buffer(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_resource_data,
        const F_resource_desc& desc
    )  {

        return { TU<F_directx11_buffer>()(device_p, initial_resource_data, desc) };
    }
    U_structured_buffer_handle HD_directx11_resource::create_structured_buffer(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_resource_data,
        const F_resource_desc& desc
    ) {

        return { TU<F_directx11_structured_buffer>()(device_p, initial_resource_data, desc) };
    }
	U_indirect_buffer_handle HD_directx11_resource::create_indirect_buffer(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_resource_data,
		const F_resource_desc& desc
	) {

		return { TU<F_directx11_indirect_buffer>()(device_p, initial_resource_data, desc) };
	}

    U_texture_1d_handle HD_directx11_resource::create_texture_1d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_resource_data,
        const F_resource_desc& desc
    )  {

        return { TU<F_directx11_texture_1d>()(device_p, initial_resource_data, desc) };
    }
    U_texture_2d_handle HD_directx11_resource::create_texture_2d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_resource_data,
        const F_resource_desc& desc
    )  {

        return { TU<F_directx11_texture_2d>()(device_p, initial_resource_data, desc) };
    }
    U_texture_3d_handle HD_directx11_resource::create_texture_3d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_resource_data,
        const F_resource_desc& desc
    )  {

        return { TU<F_directx11_texture_3d>()(device_p, initial_resource_data, desc) };
    }
	U_texture_2d_array_handle HD_directx11_resource::create_texture_2d_array(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_resource_data,
		const F_resource_desc& desc
	)  {

		return { TU<F_directx11_texture_2d_array>()(device_p, initial_resource_data, desc) };
	}
	U_texture_cube_handle HD_directx11_resource::create_texture_cube(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_resource_data,
		const F_resource_desc& desc
	) {

		return { TU<F_directx11_texture_cube>()(device_p, initial_resource_data, desc) };
	}

}