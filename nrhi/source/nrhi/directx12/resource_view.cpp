#include <nrhi/directx12/resource_view.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/descriptor.hpp>
#include <nrhi/resource_view.hpp>



namespace nrhi {

	F_directx12_resource_view::F_directx12_resource_view(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& managed_descriptor,
		ED_resource_view_type overrided_type
	) :
		A_resource_view(device_p, desc, managed_descriptor, overrided_type)
	{
	}
	F_directx12_resource_view::F_directx12_resource_view(
		TKPA_valid<A_device> device_p,
		const F_descriptor_handle& unmanaged_descriptor_handle,
		ED_resource_view_type overrided_type
	) :
		A_resource_view(device_p, unmanaged_descriptor_handle, overrided_type)
	{
	}
	F_directx12_resource_view::~F_directx12_resource_view()
	{
	}

	void F_directx12_resource_view::release_driver_specific_implementation()
	{
		NRHI_DRIVER_ENABLE_IF_ENABLE_INTERFACE_ONLY_SUPPORTS(
			H_resource_view::ALTERNATIVE::release_driver_specific_implementation(NCPP_KTHIS());
		);

		A_resource_view::release_driver_specific_implementation();
	}



	F_directx12_managed_resource_view::F_directx12_managed_resource_view(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& managed_descriptor,
		ED_resource_view_type overrided_type
	) :
		F_directx12_resource_view(device_p, desc, managed_descriptor, overrided_type)
	{
	}
	F_directx12_managed_resource_view::~F_directx12_managed_resource_view() {

		if(managed_descriptor())
			F_directx12_managed_resource_view::release_driver_specific_implementation();
	}

	void F_directx12_managed_resource_view::rebuild() {

		NCPP_ASSERT(managed_descriptor()) << "invalid descriptor";

		HD_directx12_descriptor::initialize_resource_view(
			NCPP_FOH_VALID(managed_descriptor().heap_p),
			managed_descriptor().handle.cpu_address,
			desc()
		);

		finalize_rebuild();
	}
	void F_directx12_managed_resource_view::rebuild(
		const F_resource_view_desc& desc
	) {
		NCPP_ASSERT(managed_descriptor()) << "invalid descriptor";

		HD_directx12_descriptor::initialize_resource_view(
			NCPP_FOH_VALID(managed_descriptor().heap_p),
			managed_descriptor().handle.cpu_address,
			desc
		);

		finalize_rebuild(
			desc
		);
	}
	void F_directx12_managed_resource_view::rebuild_with_managed_descriptor(
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		if(managed_descriptor())
			F_directx12_managed_resource_view::release_driver_specific_implementation();

		NCPP_ASSERT(descriptor) << "invalid descriptor";

		HD_directx12_descriptor::initialize_resource_view(
			NCPP_FOH_VALID(descriptor.heap_p),
			descriptor.handle.cpu_address,
			desc
		);

		finalize_rebuild_with_managed_descriptor(
			desc,
			descriptor
		);
	}



	F_directx12_unmanaged_resource_view::F_directx12_unmanaged_resource_view(
		TKPA_valid<A_device> device_p,
		const F_descriptor_handle& unmanaged_descriptor_handle,
		ED_resource_view_type overrided_type
	) :
		F_directx12_resource_view(device_p, unmanaged_descriptor_handle, overrided_type)
	{
	}
	F_directx12_unmanaged_resource_view::~F_directx12_unmanaged_resource_view()
	{
	}

	void F_directx12_unmanaged_resource_view::rebuild_with_unmanaged_descriptor_handle(
		const F_descriptor_handle& unmanaged_descriptor_handle
	)
	{
		finalize_rebuild_with_unmanaged_descriptor_handle(unmanaged_descriptor_handle);
	}



	TU<A_resource_view> HD_directx12_resource_view::create_with_managed_descriptor(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		return TU<F_directx12_managed_resource_view>()(
			device_p,
			desc,
			descriptor,
			desc.type
		);
	}

	U_srv_handle HD_directx12_resource_view::create_srv_with_managed_descriptor(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		return {
			TU<F_directx12_managed_resource_view>()(
				device_p,
				desc,
				descriptor,
				desc.type
			)
		};
	}
	U_uav_handle HD_directx12_resource_view::create_uav_with_managed_descriptor(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		return {
			TU<F_directx12_managed_resource_view>()(
				device_p,
				desc,
				descriptor,
				desc.type
			)
		};
	}
	U_rtv_handle HD_directx12_resource_view::create_rtv_with_managed_descriptor(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		return {
			TU<F_directx12_managed_resource_view>()(
				device_p,
				desc,
				descriptor,
				desc.type
			)
		};
	}
	U_dsv_handle HD_directx12_resource_view::create_dsv_with_managed_descriptor(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		const F_descriptor& descriptor
	) {
		return {
			TU<F_directx12_managed_resource_view>()(
				device_p,
				desc,
				descriptor,
				desc.type
			)
		};
	}

	TU<A_resource_view> HD_directx12_resource_view::create_with_unmanaged_descriptor_handle(
		TKPA_valid<A_device> device_p,
        const F_descriptor_handle& unmanaged_descriptor_handle,
		ED_resource_view_type overrided_type
	) {
		return TU<F_directx12_unmanaged_resource_view>()(
			device_p,
			unmanaged_descriptor_handle,
			overrided_type
		);
	}

	U_srv_handle HD_directx12_resource_view::create_srv_with_unmanaged_descriptor_handle(
		TKPA_valid<A_device> device_p,
        const F_descriptor_handle& unmanaged_descriptor_handle
	) {
		return {
			TU<F_directx12_unmanaged_resource_view>()(
				device_p,
				unmanaged_descriptor_handle,
				ED_resource_view_type::SHADER_RESOURCE
			)
		};
	}
	U_uav_handle HD_directx12_resource_view::create_uav_with_unmanaged_descriptor_handle(
		TKPA_valid<A_device> device_p,
        const F_descriptor_handle& unmanaged_descriptor_handle
	) {
		return {
			TU<F_directx12_unmanaged_resource_view>()(
				device_p,
				unmanaged_descriptor_handle,
				ED_resource_view_type::UNORDERED_ACCESS
			)
		};
	}
	U_rtv_handle HD_directx12_resource_view::create_rtv_with_unmanaged_descriptor_handle(
		TKPA_valid<A_device> device_p,
        const F_descriptor_handle& unmanaged_descriptor_handle
	) {
		return {
			TU<F_directx12_unmanaged_resource_view>()(
				device_p,
				unmanaged_descriptor_handle,
				ED_resource_view_type::RENDER_TARGET
			)
		};
	}
	U_dsv_handle HD_directx12_resource_view::create_dsv_with_unmanaged_descriptor_handle(
		TKPA_valid<A_device> device_p,
        const F_descriptor_handle& unmanaged_descriptor_handle
	) {
		return {
			TU<F_directx12_unmanaged_resource_view>()(
				device_p,
				unmanaged_descriptor_handle,
				ED_resource_view_type::DEPTH_STENCIL
			)
		};
	}



#pragma region Alternative Functions
#ifdef NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
	TU<A_resource_view> HD_directx12_resource_view::create(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc
	) {
		return H_resource_view::ALTERNATIVE::create(
			device_p,
			desc
		);
	}

	U_srv_handle HD_directx12_resource_view::create_srv(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc
	) {
		return H_resource_view::ALTERNATIVE::create_srv(
			device_p,
			desc
		);
	}
	U_uav_handle HD_directx12_resource_view::create_uav(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc
	) {
		return H_resource_view::ALTERNATIVE::create_uav(
			device_p,
			desc
		);
	}
	U_rtv_handle HD_directx12_resource_view::create_rtv(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc
	) {
		return H_resource_view::ALTERNATIVE::create_rtv(
			device_p,
			desc
		);
	}
	U_dsv_handle HD_directx12_resource_view::create_dsv(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc
	) {
		return H_resource_view::ALTERNATIVE::create_dsv(
			device_p,
			desc
		);
	}

	U_srv_handle HD_directx12_resource_view::create_default_srv(
		TKPA_valid<A_resource> resource_p
	) {
		return H_resource_view::ALTERNATIVE::create_default_srv(
			resource_p
		);
	}
	U_uav_handle HD_directx12_resource_view::create_default_uav(
		TKPA_valid<A_resource> resource_p
	) {
		return H_resource_view::ALTERNATIVE::create_default_uav(
			resource_p
		);
	}
	U_rtv_handle HD_directx12_resource_view::create_default_rtv(
		TKPA_valid<A_resource> resource_p
	) {
		return H_resource_view::ALTERNATIVE::create_default_rtv(
			resource_p
		);
	}
	U_dsv_handle HD_directx12_resource_view::create_default_dsv(
		TKPA_valid<A_resource> resource_p
	) {
		return H_resource_view::ALTERNATIVE::create_default_dsv(
			resource_p
		);
	}
#endif // NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
#pragma endregion

}