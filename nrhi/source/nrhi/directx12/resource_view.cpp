#include <nrhi/directx12/resource_view.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/descriptor.hpp>



namespace nrhi {

	F_directx12_resource_view::F_directx12_resource_view(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		ED_resource_view_type overrided_type
	) :
		A_resource_view(device_p, desc, overrided_type)
	{
	}
	F_directx12_resource_view::F_directx12_resource_view(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor,
		ED_resource_view_type overrided_type
	) :
		A_resource_view(device_p, desc, descriptor, overrided_type)
	{
	}
	F_directx12_resource_view::~F_directx12_resource_view() {
	}

	void F_directx12_resource_view::rebuild() {

		HD_directx12_descriptor::initialize_resource_view(
			NCPP_FOH_VALID(descriptor_.heap_p),
			descriptor_.handle.cpu_address,
			desc()
		);

		finalize_rebuild();
	}
	void F_directx12_resource_view::rebuild(
		const F_resource_view_desc& desc
	) {
		NCPP_ASSERT(descriptor_) << "invalid descriptor";

		HD_directx12_descriptor::initialize_resource_view(
			NCPP_FOH_VALID(descriptor_.heap_p),
			descriptor_.handle.cpu_address,
			desc
		);

		finalize_rebuild(
			desc
		);
	}
	void F_directx12_resource_view::rebuild(
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		HD_directx12_descriptor::initialize_resource_view(
			NCPP_FOH_VALID(descriptor.heap_p),
			descriptor.handle.cpu_address,
			desc
		);

		finalize_rebuild(
			desc,
			descriptor
		);
	}



	TU<A_resource_view> HD_directx12_resource_view::create_with_descriptor(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		return TU<F_directx12_resource_view>()(
			device_p,
			desc,
			descriptor,
			desc.type
		);
	}

	U_srv_handle HD_directx12_resource_view::create_srv_with_descriptor(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		return {
			TU<F_directx12_resource_view>()(
				device_p,
				desc,
				descriptor,
				desc.type
			)
		};
	}
	U_uav_handle HD_directx12_resource_view::create_uav_with_descriptor(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		return {
			TU<F_directx12_resource_view>()(
				device_p,
				desc,
				descriptor,
				desc.type
			)
		};
	}
	U_rtv_handle HD_directx12_resource_view::create_rtv_with_descriptor(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		return {
			TU<F_directx12_resource_view>()(
				device_p,
				desc,
				descriptor,
				desc.type
			)
		};
	}
	U_dsv_handle HD_directx12_resource_view::create_dsv_with_descriptor(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		return {
			TU<F_directx12_resource_view>()(
				device_p,
				desc,
				descriptor,
				desc.type
			)
		};
	}

}