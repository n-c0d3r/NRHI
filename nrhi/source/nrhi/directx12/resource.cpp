#include <nrhi/directx12/resource.hpp>
#include <nrhi/directx12/committed_resource.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

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
	U_buffer_handle HD_directx12_resource::create_committed_structured_buffer(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc
	)  {
		return { TU<F_directx12_committed_resource>()(device_p, desc) };
	}
	U_buffer_handle HD_directx12_resource::create_committed_indirect_buffer(
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
	U_texture_cube_handle HD_directx12_resource::create_committed_texture_cube(
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

}