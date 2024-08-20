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
			d3d12_resource_p_->Release();
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
		CD3DX12_RANGE range(0, resource_p->desc().size);
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
		CD3DX12_RANGE range(0, resource_p->desc().size);

		resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p()->Unmap(
			subresource_index,
			&range
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