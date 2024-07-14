#pragma once

/** @file nrhi/directx12/committed_resource.hpp
*
*   Implement directx12 committed resource.
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

#include <nrhi/directx12/resource.hpp>

#pragma endregion



namespace nrhi {

	class NRHI_API F_directx12_committed_resource : public F_directx12_resource {

	public:
		F_directx12_committed_resource(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);
		F_directx12_committed_resource(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc,
			ED_resource_type overrided_type
		);
		F_directx12_committed_resource(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc,
			ED_resource_type overrided_type,
			ID3D12Resource* d3d12_resource_p
		);
		virtual ~F_directx12_committed_resource();

	private:
		static ID3D12Resource* create_d3d12_committed_resource(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);

	};

}
