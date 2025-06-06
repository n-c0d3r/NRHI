#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/buffer.hpp>
#include <nrhi/directx11/texture.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	void F_directx11_resource::set_debug_name(const F_debug_name& value)
	{
		A_render_object::set_debug_name(value);

		d3d11_resource_p_->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			value.size() - 1,
			value.data()
		);
	}
#endif



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



	void F_directx11_resource::release_driver_specific_implementation()
	{
		if(d3d11_resource_p_)
		{
			d3d11_resource_p_->Release();
			d3d11_resource_p_ = 0;
		}
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
		const auto& footprint = resource_p->footprint();

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

		return { (u8*)(d3d11_mapped_resource.pData), footprint.size };
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

	sz HD_directx11_resource::texture_first_pitch(u32 element_stride, u32 count)
    {
    	return element_stride * count;
    }
	sz HD_directx11_resource::texture_second_pitch(sz first_pitch, u32 count)
	{
    	return first_pitch * count;
	}
	sz HD_directx11_resource::texture_third_pitch(sz second_pitch, u32 count)
	{
    	return second_pitch * count;
	}

	u32 HD_directx11_resource::subresource_index(
		u32 array_slice,
		u32 mip_level,
		u32 mip_level_count
	)
    {
    	return (
			+ (array_slice * mip_level_count)
			+ mip_level
		);
    }
	u32 HD_directx11_resource::subresource_index_with_plane_slice(
		u32 array_slice,
		u32 mip_level,
		u32 mip_level_count,
		u32 array_size,
		u32 plane_slice
	)
    {
    	NCPP_ASSERT(plane_slice == 0) << "non-zero plane slice is not supported";
		return 0;
    }

	F_vector3_u32 HD_directx11_resource::most_detailed_subresource_volume(
		ED_resource_type type,
		PA_vector3_u32 resource_volume
	)
    {
    	NCPP_ASSERT(type != ED_resource_type::BUFFER);
    	NRHI_ENUM_SWITCH(
			type,
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_1D,
				return { resource_volume.x, 1, 1 };
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D,
				return { resource_volume.xy(), 1 };
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D_ARRAY,
				return { resource_volume.xy(), 1 };
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_3D,
				return resource_volume;
			)
		);
    }
	F_vector3_u32 HD_directx11_resource::mip_divisor(
		ED_resource_type type
	)
	{
    	NCPP_ASSERT(type != ED_resource_type::BUFFER);
    	NRHI_ENUM_SWITCH(
			type,
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_1D,
				return { 2, 1, 1 };
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D,
				return { 2, 2, 1 };
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D_ARRAY,
				return { 2, 2, 1 };
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_3D,
				return { 2, 2, 2 };
			)
		);
	}

	u32 HD_directx11_resource::subresource_count(
		const F_resource_desc& desc
	)
	{
		return (
			eastl::max<u32>(desc.mip_level_count, 1)
			* (
				(desc.type == ED_resource_type::TEXTURE_3D)
				? 1
				: eastl::max<u32>(desc.array_size, 1)
			)
		);
	}
	F_resource_footprint HD_directx11_resource::footprint(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	)
	{
		F_resource_footprint result;

		result.size = 0;

		u32 subresource_count = HD_directx11_resource::subresource_count(desc);
		result.placed_subresource_footprints.resize(subresource_count);
		result.subresource_sizes.resize(subresource_count);

		for(u32 i = 0; i < subresource_count; ++i)
		{
			auto& placed_subresource_footprint = result.placed_subresource_footprints[i];
			auto& subresource_info = desc.subresource_infos[i];

			placed_subresource_footprint.offset = result.size;
			placed_subresource_footprint.footprint.width = subresource_info.width;
			placed_subresource_footprint.footprint.height = subresource_info.height;
			placed_subresource_footprint.footprint.depth = subresource_info.depth;
			placed_subresource_footprint.footprint.format = desc.format;

			if(desc.type == ED_resource_type::BUFFER)
			{
				placed_subresource_footprint.footprint.first_pitch = desc.stride * desc.element_count;
				result.subresource_sizes[i] = placed_subresource_footprint.footprint.first_pitch;
			}
			else
			{
				auto first_pitch = HD_directx11_resource::texture_first_pitch(desc.stride, subresource_info.width);
				auto second_pitch = HD_directx11_resource::texture_second_pitch(first_pitch, subresource_info.height);
				auto third_pitch = HD_directx11_resource::texture_third_pitch(second_pitch, subresource_info.depth);

				placed_subresource_footprint.footprint.first_pitch = first_pitch;
				result.subresource_sizes[i] = third_pitch;
			}

			result.size += result.subresource_sizes[i];
		}

		return eastl::move(result);
	}
	sz HD_directx11_resource::calculate_size(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	)
	{
		u32 subresource_count = HD_directx11_resource::subresource_count(desc);

		sz result = 0;

		for(u32 i = 0; i < subresource_count; ++i)
		{
			auto& subresource_info = desc.subresource_infos[i];

			if(desc.type == ED_resource_type::BUFFER)
			{
				result += desc.stride * desc.element_count;
			}
			else
			{
				auto first_pitch = HD_directx11_resource::texture_first_pitch(desc.stride, subresource_info.width);
				auto second_pitch = HD_directx11_resource::texture_second_pitch(first_pitch, subresource_info.height);
				auto third_pitch = HD_directx11_resource::texture_third_pitch(second_pitch, subresource_info.depth);

				result += third_pitch;
			}
		}

		return result;
	}
}