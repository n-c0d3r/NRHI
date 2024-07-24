#include <nrhi/directx12/resource_view.hpp>
#include <nrhi/directx12/device.hpp>



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

		finalize_rebuild();
	}
	void F_directx12_resource_view::rebuild(
		const F_resource_view_desc& desc
	) {
		finalize_rebuild(
			desc
		);
	}
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	void F_directx12_resource_view::rebuild(
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		finalize_rebuild(
			desc,
			descriptor
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

}