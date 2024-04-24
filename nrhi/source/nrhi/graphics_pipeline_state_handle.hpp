#pragma once

/** @file nrhi/graphics_pipeline_state_handle.hpp
*
*   Implement graphics pipeline state handle.
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

#pragma endregion



namespace nrhi {

	class A_pipeline_state;



	NCPP_FHANDLE_TEMPLATE(A_pipeline_state)
	struct TF_graphics_pipeline_state_handle {

		NCPP_FHANDLE_GENERATED_BODY(TF_graphics_pipeline_state_handle, A_pipeline_state);

	};

	using U_graphics_pipeline_state_handle = TF_graphics_pipeline_state_handle<TU<A_pipeline_state>>;
	using S_graphics_pipeline_state_handle = TF_graphics_pipeline_state_handle<TS<A_pipeline_state>>;
	using K_graphics_pipeline_state_handle = TF_graphics_pipeline_state_handle<TK<A_pipeline_state>>;

	using S_valid_graphics_pipeline_state_handle = TF_graphics_pipeline_state_handle<TS_valid<A_pipeline_state>>;
	using K_valid_graphics_pipeline_state_handle = TF_graphics_pipeline_state_handle<TK_valid<A_pipeline_state>>;

}
