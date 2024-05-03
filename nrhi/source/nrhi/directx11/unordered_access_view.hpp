#pragma once

/** @file nrhi/directx11/unordered_access_view.hpp
*
*   Implement directx11 unordered_access_view.
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

#include <nrhi/directx11/resource_view.hpp>

#pragma endregion



namespace nrhi {

	class A_device;



	class NRHI_API F_directx11_unordered_access_view : public F_directx11_resource_view {

	public:
		F_directx11_unordered_access_view(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc,
			E_resource_view_type overrided_type = E_resource_view_type::SRV
		);
		F_directx11_unordered_access_view(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc,
			E_resource_view_type overrided_type,
			ID3D11UnorderedAccessView* d3d11_unordered_access_view_p
		);
		virtual ~F_directx11_unordered_access_view();

	private:
		static ID3D11UnorderedAccessView* create_d3d11_unordered_access_view(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc
		);

	};

}