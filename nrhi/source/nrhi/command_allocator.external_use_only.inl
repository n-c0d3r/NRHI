#pragma once

/** @file nrhi/command_allocator.external_use_only.inl
*
*   Implement command allocator inline functions that is only used by external.
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

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
#include <nrhi/command_allocator.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#pragma endregion



namespace nrhi {

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
	NCPP_FORCE_INLINE void A_command_allocator::flush() {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(
			H_command_allocator::flush(NCPP_KTHIS());
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
}
