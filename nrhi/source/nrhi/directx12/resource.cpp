#include <nrhi/directx12/resource.hpp>
#include <nrhi/directx12/committed_resource.hpp>
#include <nrhi/directx12/placed_resource.hpp>
#include <nrhi/directx12/reserved_resource.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/format_helper.hpp>
#include <nrhi/resource.hpp>



namespace nrhi {

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	void F_directx12_resource::set_debug_name(const F_debug_name& value)
	{
		A_render_object::set_debug_name(value);

		auto wvalue = G_to_wstring(value.c_str());
		d3d12_resource_p_->SetName(wvalue.data());
	}
#endif



	F_directx12_resource::F_directx12_resource(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc,
		ED_resource_type overrided_type
	) :
		A_resource(device_p, {}, desc, overrided_type)
	{
	}
	F_directx12_resource::F_directx12_resource(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc,
		ED_resource_type overrided_type,
		ID3D12Resource* d3d12_resource_p
	) :
		A_resource(device_p, {}, desc, overrided_type),
		d3d12_resource_p_(d3d12_resource_p)
	{
	}
	F_directx12_resource::~F_directx12_resource() {

		if(d3d12_resource_p_)
			d3d12_resource_p_->Release();
	}

	void F_directx12_resource::release_driver_specific_implementation()
	{
		if(d3d12_resource_p_)
		{
			d3d12_resource_p_->Release();
			d3d12_resource_p_ = 0;
		}
	}




	TU<A_resource> HD_directx12_resource::create_committed(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	){
		return TU<F_directx12_committed_resource>()(device_p, desc);
	}

	U_buffer_handle HD_directx12_resource::create_committed_buffer(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	)  {
		return { TU<F_directx12_committed_resource>()(device_p, desc) };
	}

	U_texture_1d_handle HD_directx12_resource::create_committed_texture_1d(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	) {
		return { TU<F_directx12_committed_resource>()(device_p, desc) };
	}
	U_texture_2d_handle HD_directx12_resource::create_committed_texture_2d(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	) {
		return { TU<F_directx12_committed_resource>()(device_p, desc) };
	}
	U_texture_3d_handle HD_directx12_resource::create_committed_texture_3d(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	) {
		return { TU<F_directx12_committed_resource>()(device_p, desc) };
	}
	U_texture_2d_array_handle HD_directx12_resource::create_committed_texture_2d_array(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	) {
		return { TU<F_directx12_committed_resource>()(device_p, desc) };
	}

	TU<A_resource> HD_directx12_resource::create_placed(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset
	) {
		return TU<F_directx12_placed_resource>()(device_p, desc, heap_p, heap_offset);
	}

	U_buffer_handle HD_directx12_resource::create_placed_buffer(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset
	) {
		return { TU<F_directx12_placed_resource>()(device_p, desc, heap_p, heap_offset) };
	}

	U_texture_1d_handle HD_directx12_resource::create_placed_texture_1d(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset
	) {
		return { TU<F_directx12_placed_resource>()(device_p, desc, heap_p, heap_offset) };
	}
	U_texture_2d_handle HD_directx12_resource::create_placed_texture_2d(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset
	) {
		return { TU<F_directx12_placed_resource>()(device_p, desc, heap_p, heap_offset) };
	}
	U_texture_3d_handle HD_directx12_resource::create_placed_texture_3d(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset
	) {
		return { TU<F_directx12_placed_resource>()(device_p, desc, heap_p, heap_offset) };
	}
	U_texture_2d_array_handle HD_directx12_resource::create_placed_texture_2d_array(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset
	) {
		return { TU<F_directx12_placed_resource>()(device_p, desc, heap_p, heap_offset) };
	}

	TU<A_resource> HD_directx12_resource::create_reserved(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	) {
		return { TU<F_directx12_committed_resource>()(device_p, desc) };
	}

	U_buffer_handle HD_directx12_resource::create_reserved_buffer(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	) {
		return { TU<F_directx12_committed_resource>()(device_p, desc) };
	}

	U_texture_1d_handle HD_directx12_resource::create_reserved_texture_1d(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	) {
		return { TU<F_directx12_committed_resource>()(device_p, desc) };
	}
	U_texture_2d_handle HD_directx12_resource::create_reserved_texture_2d(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	) {
		return { TU<F_directx12_committed_resource>()(device_p, desc) };
	}
	U_texture_3d_handle HD_directx12_resource::create_reserved_texture_3d(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	) {
		return { TU<F_directx12_committed_resource>()(device_p, desc) };
	}
	U_texture_2d_array_handle HD_directx12_resource::create_reserved_texture_2d_array(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	) {
		return { TU<F_directx12_committed_resource>()(device_p, desc) };
	}

	F_mapped_subresource HD_directx12_resource::map(
		TKPA_valid<A_resource> resource_p,
		u32 subresource_index
	) {
		CD3DX12_RANGE range(0, resource_p->footprint().subresource_sizes[subresource_index]);
		void* data_p = 0;

		HRESULT hr = resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p()->Map(
			subresource_index,
			&range,
			&data_p
		);
		NCPP_ASSERT(!FAILED(hr)) << "can't map resource";

		return { (u8*)data_p, range.End };
	}
	void HD_directx12_resource::unmap(
		TKPA_valid<A_resource> resource_p,
		u32 subresource_index
	) {
		resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p()->Unmap(
			subresource_index,
			0
		);
	}

	F_resource_gpu_virtual_address HD_directx12_resource::gpu_virtual_address(
		TKPA_valid<A_resource> resource_p
	) {
		return resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p()->GetGPUVirtualAddress();
	}

	sz HD_directx12_resource::first_pitch(u32 element_stride, u32 count)
	{
		return align_size(
			element_stride * count,
			D3D12_TEXTURE_DATA_PITCH_ALIGNMENT
		);
	}
	sz HD_directx12_resource::second_pitch(sz first_pitch, u32 count)
	{
		NCPP_ASSERT(first_pitch % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT == 0) << "first pitch is not aligned";
		return first_pitch * count;
	}
	sz HD_directx12_resource::third_pitch(sz second_pitch, u32 count)
	{
		NCPP_ASSERT(second_pitch % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT == 0) << "second pitch is not aligned";
		return second_pitch * count;
	}

	u32 HD_directx12_resource::subresource_index(
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
	u32 HD_directx12_resource::subresource_index_with_plane_slice(
		u32 array_slice,
		u32 mip_level,
		u32 mip_level_count,
		u32 array_size,
		u32 plane_slice
	)
	{
		return (
			(plane_slice * mip_level_count * array_size)
			+ (array_slice * mip_level_count)
			+ mip_level
		);
	}

	F_vector3_u32 HD_directx12_resource::most_detailed_subresource_volume(
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
	F_vector3_u32 HD_directx12_resource::mip_divisor(
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

	u32 HD_directx12_resource::subresource_count(
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
	F_resource_footprint HD_directx12_resource::footprint(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	)
	{
		F_resource_footprint result;

		result.size = 0;

		u32 subresource_count = HD_directx12_resource::subresource_count(desc);
		result.placed_subresource_footprints.resize(subresource_count);
		result.subresource_sizes.resize(subresource_count);

		D3D12_RESOURCE_DESC d3d12_resource_desc;
		d3d12_resource_desc.Dimension = NRHI_DRIVER_DIRECTX_12_MAP___RESOURCE_TYPE___TO___RESOURCE_DIMENSION(desc.type);
		d3d12_resource_desc.Alignment = desc.alignment;
		d3d12_resource_desc.Width = eastl::max<u32>(1, desc.width);
		d3d12_resource_desc.Height = eastl::max<u32>(1, desc.height);
		d3d12_resource_desc.DepthOrArraySize = eastl::max<u32>(1, desc.depth);
		d3d12_resource_desc.MipLevels = eastl::max<u32>(1, desc.mip_level_count);
		d3d12_resource_desc.Format = DXGI_FORMAT(desc.format);
		d3d12_resource_desc.SampleDesc.Count = desc.sample_desc.count;
		d3d12_resource_desc.SampleDesc.Quality = desc.sample_desc.quality;
		d3d12_resource_desc.Layout = D3D12_TEXTURE_LAYOUT(desc.layout);
		d3d12_resource_desc.Flags = NRHI_DRIVER_DIRECTX_12_MAP___RESOURCE_FLAG___TO___RESOURCE_FLAG(desc.flags);

		if(desc.type == ED_resource_type::BUFFER)
		{
			d3d12_resource_desc.Format = DXGI_FORMAT_UNKNOWN;
			d3d12_resource_desc.Width = desc.element_count * desc.stride;
		}

		TG_fixed_vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT, 6> d3d12_placed_subresource_footprints(subresource_count);

		device_p.T_cast<F_directx12_device>()->d3d12_device_p()->GetCopyableFootprints(
			&d3d12_resource_desc,
			0,
			subresource_count,
			0,
			d3d12_placed_subresource_footprints.data(),
			0,
			0,
			result.subresource_sizes.data()
		);

		for(u32 i = 0; i < subresource_count; ++i)
		{
			result.size += result.subresource_sizes[i];

			auto& placed_subresource_footprint = result.placed_subresource_footprints[i];
			auto& d3d12_placed_subresource_footprint = d3d12_placed_subresource_footprints[i];

			placed_subresource_footprint.offset = d3d12_placed_subresource_footprint.Offset;
			placed_subresource_footprint.footprint.width = d3d12_placed_subresource_footprint.Footprint.Width;
			placed_subresource_footprint.footprint.height = d3d12_placed_subresource_footprint.Footprint.Height;
			placed_subresource_footprint.footprint.depth = d3d12_placed_subresource_footprint.Footprint.Depth;
			placed_subresource_footprint.footprint.format = ED_format(d3d12_placed_subresource_footprint.Footprint.Format);
			placed_subresource_footprint.footprint.first_pitch = d3d12_placed_subresource_footprint.Footprint.RowPitch;
		}

		return eastl::move(result);
	}
	sz HD_directx12_resource::calculate_size(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	)
	{
		u32 subresource_count = HD_directx12_resource::subresource_count(desc);

		sz result = 0;

		F_subresource_sizes subresource_sizes(subresource_count);
		subresource_sizes.resize(subresource_count);

		D3D12_RESOURCE_DESC d3d12_resource_desc;
		d3d12_resource_desc.Dimension = NRHI_DRIVER_DIRECTX_12_MAP___RESOURCE_TYPE___TO___RESOURCE_DIMENSION(desc.type);
		d3d12_resource_desc.Alignment = desc.alignment;
		d3d12_resource_desc.Width = eastl::max<u32>(1, desc.width);
		d3d12_resource_desc.Height = eastl::max<u32>(1, desc.height);
		d3d12_resource_desc.DepthOrArraySize = eastl::max<u32>(1, desc.depth);
		d3d12_resource_desc.MipLevels = eastl::max<u32>(1, desc.mip_level_count);
		d3d12_resource_desc.Format = DXGI_FORMAT(desc.format);
		d3d12_resource_desc.SampleDesc.Count = desc.sample_desc.count;
		d3d12_resource_desc.SampleDesc.Quality = desc.sample_desc.quality;
		d3d12_resource_desc.Layout = D3D12_TEXTURE_LAYOUT(desc.layout);
		d3d12_resource_desc.Flags = NRHI_DRIVER_DIRECTX_12_MAP___RESOURCE_FLAG___TO___RESOURCE_FLAG(desc.flags);

		if(desc.type == ED_resource_type::BUFFER)
		{
			d3d12_resource_desc.Format = DXGI_FORMAT_UNKNOWN;
			d3d12_resource_desc.Width = desc.element_count * desc.stride;
		}

		device_p.T_cast<F_directx12_device>()->d3d12_device_p()->GetCopyableFootprints(
			&d3d12_resource_desc,
			0,
			subresource_count,
			0,
			0,
			0,
			0,
			subresource_sizes.data()
		);

		for(u32 i = 0; i < subresource_count; ++i)
		{
			result += subresource_sizes[i];
		}

		return result;
	}



#pragma region Alternative Functions
#ifdef NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
	TU<A_resource> HD_directx12_resource::create(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& inital_data,
		const F_resource_desc& desc
	) {
		return H_resource::ALTERNATIVE::create(
			device_p,
			inital_data,
			desc
		);
	}

	U_buffer_handle HD_directx12_resource::create_buffer(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& inital_data,
		const F_resource_desc& desc
	) {
		return H_resource::ALTERNATIVE::create_buffer(
			device_p,
			inital_data,
			desc
		);
	}

	U_texture_1d_handle HD_directx12_resource::create_texture_1d(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& inital_data,
		const F_resource_desc& desc
	) {
		return H_resource::ALTERNATIVE::create_texture_1d(
			device_p,
			inital_data,
			desc
		);
	}
	U_texture_2d_handle HD_directx12_resource::create_texture_2d(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& inital_data,
		const F_resource_desc& desc
	) {
		return H_resource::ALTERNATIVE::create_texture_2d(
			device_p,
			inital_data,
			desc
		);
	}
	U_texture_3d_handle HD_directx12_resource::create_texture_3d(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& inital_data,
		const F_resource_desc& desc
	) {
		return H_resource::ALTERNATIVE::create_texture_3d(
			device_p,
			inital_data,
			desc
		);
	}
	U_texture_2d_array_handle HD_directx12_resource::create_texture_2d_array(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& inital_data,
		const F_resource_desc& desc
	) {
		return H_resource::ALTERNATIVE::create_texture_2d_array(
			device_p,
			inital_data,
			desc
		);
	}
#endif // NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
#pragma endregion

}