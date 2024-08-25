#pragma once

/** @file nrhi/directx12/resource_view.hpp
*
*   Implement directx12 sampler state.
*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#pragma region Includes

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/prerequisites.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/resource_view_base.hpp>
#include <nrhi/descriptor_base.hpp>

#pragma endregion



namespace nrhi {

	class A_device;



	class NRHI_API F_directx12_resource_view : public A_resource_view {

	public:
		F_directx12_resource_view(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc,
			const F_descriptor& descriptor,
			ED_resource_view_type overrided_type
		);
		F_directx12_resource_view(
			TKPA_valid<A_device> device_p,
			const F_descriptor& descriptor,
			ED_resource_view_type overrided_type
		);
		virtual ~F_directx12_resource_view();

	public:
		NCPP_OBJECT(F_directx12_resource_view);
	};



	class NRHI_API F_directx12_managed_resource_view : public F_directx12_resource_view {

	public:
		F_directx12_managed_resource_view(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc,
			const F_descriptor& descriptor,
			ED_resource_view_type overrided_type
		);
		virtual ~F_directx12_managed_resource_view();

	public:
		NCPP_OBJECT(F_directx12_managed_resource_view);

	public:
		virtual void rebuild() override;
		virtual void rebuild(
			const F_resource_view_desc& desc
		) override;
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		virtual void rebuild_with_descriptor(
			const F_resource_view_desc& desc,
			const F_descriptor& descriptor
		) override;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

	public:
		virtual void release_driver_specific_implementation() override;
	};



	class NRHI_API F_directx12_unmanaged_resource_view : public F_directx12_resource_view {

	public:
		F_directx12_unmanaged_resource_view(
			TKPA_valid<A_device> device_p,
			const F_descriptor& descriptor,
			ED_resource_view_type overrided_type
		);
		virtual ~F_directx12_unmanaged_resource_view();

	public:
		NCPP_OBJECT(F_directx12_unmanaged_resource_view);

	public:
		virtual void rebuild_unmanaged_with_descriptor(
			const F_descriptor& descriptor
		) override;
	};



	class NRHI_API HD_directx12_resource_view {

	public:
		static TU<A_resource_view> create_with_descriptor(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc,
			const F_descriptor& descriptor
		);

	public:
		static U_srv_handle create_srv_with_descriptor(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc,
			const F_descriptor& descriptor
		);
		static U_uav_handle create_uav_with_descriptor(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc,
			const F_descriptor& descriptor
		);
		static U_rtv_handle create_rtv_with_descriptor(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc,
			const F_descriptor& descriptor
		);
		static U_dsv_handle create_dsv_with_descriptor(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc,
			const F_descriptor& descriptor
			);

	public:
		static TU<A_resource_view> create_unmanaged_with_descriptor(
			TKPA_valid<A_device> device_p,
			const F_descriptor& descriptor,
			ED_resource_view_type overrided_type
		);

	public:
		static U_srv_handle create_unmanaged_srv_with_descriptor(
			TKPA_valid<A_device> device_p,
			const F_descriptor& descriptor
		);
		static U_uav_handle create_unmanaged_uav_with_descriptor(
			TKPA_valid<A_device> device_p,
			const F_descriptor& descriptor
		);
		static U_rtv_handle create_unmanaged_rtv_with_descriptor(
			TKPA_valid<A_device> device_p,
			const F_descriptor& descriptor
		);
		static U_dsv_handle create_unmanaged_dsv_with_descriptor(
			TKPA_valid<A_device> device_p,
			const F_descriptor& descriptor
		);



#pragma region Alternative Functions
#ifdef NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
	public:
		static TU<A_resource_view> create(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc
		);

	public:
		static U_srv_handle create_srv(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc
		);
		static U_uav_handle create_uav(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc
		);
		static U_rtv_handle create_rtv(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc
		);
		static U_dsv_handle create_dsv(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc
		);

	public:
		static U_srv_handle create_default_srv(
			TKPA_valid<A_resource> resource_p
		);
		static U_uav_handle create_default_uav(
			TKPA_valid<A_resource> resource_p
		);
		static U_rtv_handle create_default_rtv(
			TKPA_valid<A_resource> resource_p
		);
		static U_dsv_handle create_default_dsv(
			TKPA_valid<A_resource> resource_p
		);
#endif // NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
#pragma endregion

	};

}
