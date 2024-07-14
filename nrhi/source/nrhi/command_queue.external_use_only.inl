#pragma once

/** @file nrhi/command_queue.external_use_only.inl
*
*   Implement command queue inline functions that is only used by external.
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

#include <nrhi/command_queue_base.hpp>
#include <nrhi/command_queue.hpp>

#pragma endregion



namespace nrhi {

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION
	NCPP_FORCE_INLINE void A_command_queue::execute_command_lists(TG_span<TK_valid<A_command_list>> command_list_p_span) {

		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(
			H_command_queue::execute_command_lists(NCPP_KTHIS(), command_list_p_span);
		);
	}
	NCPP_FORCE_INLINE void A_command_queue::execute_command_list(TKPA_valid<A_command_list> command_list_p) {

		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(
			H_command_queue::execute_command_list(NCPP_KTHIS(), command_list_p);
		);
	}
#endif

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
	NCPP_FORCE_INLINE void A_command_queue::async_signal(
		TKPA_valid<A_fence> fence_p,
		u64 new_value
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_queue::async_signal(NCPP_KTHIS(), fence_p, new_value);
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
}
