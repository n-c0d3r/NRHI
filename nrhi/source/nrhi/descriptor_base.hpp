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
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

#pragma endregion



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
namespace nrhi {

    class A_device;
    class A_descriptor_heap;



    struct F_descriptor_range_desc {

        ED_descriptor_range_type type;

        u32 base_register = 0;
        u32 register_space = 0;

    };

    struct F_descriptor_table_desc {

        TG_span<F_descriptor_range_desc> range_descs;

    };



	struct F_descriptor_handle {

		u64 cpu_address = 0;
		u64 gpu_address = 0;

	};

	struct F_descriptor {

		F_descriptor_handle handle;
		TK<A_descriptor_heap> heap_p;

	};

}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING