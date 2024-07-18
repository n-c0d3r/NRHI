#pragma once

/** @file nrhi/fence_base.hpp
*
*   Implement fence base class.
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
#include <nrhi/device_child.hpp>
#include <nrhi/fence_flag.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#pragma endregion



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
namespace nrhi {

	class A_device;



	struct F_fence_desc {

		u64 initial_value = 0;
		ED_fence_flag flags = ED_fence_flag::NONE;

	};



	class NRHI_API A_fence : public A_device_child {

	private:
		F_fence_desc desc_;

	public:
		NCPP_FORCE_INLINE const F_fence_desc& desc() const noexcept { return desc_; }



	protected:
		A_fence(
			TKPA_valid<A_device> device_p,
			const F_fence_desc& desc
		);

	public:
		virtual ~A_fence();

	public:
		NCPP_OBJECT(A_fence);



	public:
		u64 value();
		void wait(u64 target_value);
		b8 is_completed(u64 target_value);

	};

}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION