#pragma once

/** @file nrhi/directx12/reserved_resource.hpp
*
*   Implement directx12 reserved resource.
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

	class NRHI_API F_directx12_reserved_resource : public F_directx12_resource {

	public:
		F_directx12_reserved_resource(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);
		F_directx12_reserved_resource(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc,
			ED_resource_type overrided_type
		);
		F_directx12_reserved_resource(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc,
			ED_resource_type overrided_type,
			ID3D12Resource* d3d12_resource_p
		);
		virtual ~F_directx12_reserved_resource();

	private:
		static ID3D12Resource* create_d3d12_reserved_resource(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);

	public:
		virtual void rebuild_reserved(
			const F_resource_desc& desc
		) override;

	public:
		virtual E_resource_management_type management_type() const override;

	};

}
