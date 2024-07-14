#include <nrhi/directx12/resource.hpp>
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
		return null;
	}

}