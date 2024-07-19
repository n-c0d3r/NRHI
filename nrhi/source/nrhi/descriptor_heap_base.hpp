#pragma once

/** @file nrhi/descriptor.hpp
*
*   Implement descriptor.
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

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#include <nrhi/device_child.hpp>
#include <nrhi/descriptor_range_type.hpp>
#include <nrhi/descriptor_heap_type.hpp>
#include <nrhi/descriptor_heap_flag.hpp>
#include <nrhi/descriptor_base.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

#pragma endregion



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
namespace nrhi {

	class A_device;
	class A_descriptor_heap;



	struct F_descriptor_heap_desc {

		ED_descriptor_heap_type type;
		ED_descriptor_heap_flag flags = ED_descriptor_heap_flag::NONE;
		u32 descriptor_count = 0;

	};



	class NRHI_API A_descriptor_heap : public A_device_child {

	private:
		F_descriptor_heap_desc desc_;

	public:
		NCPP_FORCE_INLINE const F_descriptor_heap_desc& desc() const noexcept { return desc_; }



	protected:
		A_descriptor_heap(TK_valid<A_device> device_p, const F_descriptor_heap_desc& desc);

	public:
		virtual ~A_descriptor_heap();

	public:
		NCPP_OBJECT(A_descriptor_heap);

	public:
		F_descriptor_cpu_address base_cpu_address();
		F_descriptor_gpu_address base_gpu_address();

	};

}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING