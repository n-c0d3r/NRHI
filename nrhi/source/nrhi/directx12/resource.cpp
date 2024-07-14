#include <nrhi/directx12/resource.hpp>
#include <nrhi/directx12/committed_resource.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

	F_directx12_resource::F_directx12_resource(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc,
		ED_resource_type overrided_type
	) :
		A_resource(device_p, initial_data, desc, overrided_type)
	{
	}
	F_directx12_resource::F_directx12_resource(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc,
		ED_resource_type overrided_type,
		ID3D12Resource* d3d12_resource_p
	) :
		A_resource(device_p, initial_data, desc, overrided_type),
		d3d12_resource_p_(d3d12_resource_p)
	{
	}
	F_directx12_resource::~F_directx12_resource() {

		if(d3d12_resource_p_)
			d3d12_resource_p_->Release();
	}



	TU<A_resource> HD_directx12_resource::create(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_resource_data,
		const F_resource_desc& desc
	){
		NRHI_ENUM_SWITCH(
			desc.type,
			NRHI_ENUM_CASE(
				ED_resource_type::BUFFER,
				return TU<F_directx12_committed_resource>()(device_p, initial_resource_data, desc);
			)
		);
		return null;
	}

	U_buffer_handle HD_directx12_resource::create_buffer(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_resource_data,
		const F_resource_desc& desc
	)  {
		return { TU<F_directx12_committed_resource>()(device_p, initial_resource_data, desc) };
	}

}