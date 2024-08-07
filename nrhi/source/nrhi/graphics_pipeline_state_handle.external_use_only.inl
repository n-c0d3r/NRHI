#pragma once

/** @file nrhi/graphics_pipeline_state_handle.external_use_only.inl
*
*   Implement graphics_pipeline_state_handle inline functions that is only used by external.
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

#include <nrhi/graphics_pipeline_state_handle.hpp>
#include <nrhi/pipeline_state.hpp>

#pragma endregion



namespace nrhi {

	NCPP_FHANDLE_TEMPLATE_DEFINE_MEMBER(A_pipeline_state)
	NCPP_FORCE_INLINE const F_graphics_pipeline_state_options& TF_graphics_pipeline_state_handle<F_oref__>::options() const noexcept {

		return H_pipeline_state::graphics_options({ oref });
	}

}
