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
				ED_resource_type::TEXTURE_1D,
				return TU<F_directx11_texture_1d>()(device_p, initial_resource_data, desc);
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D,
				return TU<F_directx11_texture_2d>()(device_p, initial_resource_data, desc);
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_3D,
				return TU<F_directx11_texture_3d>()(device_p, initial_resource_data, desc);
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D_ARRAY,
				return TU<F_directx11_texture_2d_array>()(device_p, initial_resource_data, desc);
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

	F_mapped_subresource HD_directx11_resource::map(
		TKPA_valid<A_resource> resource_p,
		u32 subresource_index
	) {
		auto d3d11_resource_p = resource_p.T_cast<F_directx11_resource>()->d3d11_resource_p();
		auto d3d11_immediate_device_context_p = resource_p->device_p().T_cast<F_directx11_device>()->d3d11_immediate_ctx_p_unsafe();

		D3D11_MAP d3d11_map_type;

		const auto& desc = resource_p->desc();

		NRHI_ENUM_SWITCH(
			desc.heap_type,
			NRHI_ENUM_CASE(
				ED_resource_heap_type::GREAD_CWRITE,
				d3d11_map_type = D3D11_MAP_WRITE_DISCARD
			)
			NRHI_ENUM_CASE(
				ED_resource_heap_type::CREAD_GWRITE,
				d3d11_map_type = D3D11_MAP_READ
			)
		);

		D3D11_MAPPED_SUBRESOURCE d3d11_mapped_resource;

		HRESULT hr = d3d11_immediate_device_context_p->Map(
			d3d11_resource_p,
			subresource_index,
			d3d11_map_type,
			0,
			&d3d11_mapped_resource
		);
		NCPP_ASSERT(!FAILED(hr)) << "can't map resource";

		return { (u8*)(d3d11_mapped_resource.pData), desc.size };
	}
	void HD_directx11_resource::unmap(
		TKPA_valid<A_resource> resource_p,
		u32 subresource_index
	) {
		auto d3d11_resource_p = resource_p.T_cast<F_directx11_resource>()->d3d11_resource_p();
		auto d3d11_immediate_device_context_p = resource_p->device_p().T_cast<F_directx11_device>()->d3d11_immediate_ctx_p_unsafe();

		d3d11_immediate_device_context_p->Unmap(
			d3d11_resource_p,
			subresource_index
		);
	}

}