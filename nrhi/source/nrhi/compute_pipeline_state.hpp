#pragma once

/** @file nrhi/compute_pipeline_state.hpp
*
*   Implement compute pipeline_state.
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

#include <nrhi/pipeline_state_base.hpp>
#include <nrhi/compute_pipeline_state_handle.hpp>

#pragma endregion



namespace nrhi {

	class NRHI_API H_compute_pipeline_state {

	public:
#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
		static U_compute_pipeline_state_handle create(
			TKPA_valid<A_device> device_p,
			const F_compute_pipeline_state_options& options
		);
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		static U_compute_pipeline_state_handle create_with_root_signature(
			TKPA_valid<A_device> device_p,
			const F_compute_pipeline_state_options& options,
			TKPA_valid<A_root_signature> root_signature_p
		);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

	};
	
}
