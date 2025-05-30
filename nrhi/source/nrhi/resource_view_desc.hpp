#pragma once

/** @file nrhi/resource_view_desc.hpp
*
*   Implement resource view desc.
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

#include <nrhi/resource_view_type.hpp>
#include <nrhi/resource_type.hpp>
#include <nrhi/resource_flag.hpp>
#include <nrhi/format.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;
	class A_resource_view;



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	enum class E_resource_view_management_type {

		MANAGED,
		UNMANAGED

	};
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING



    struct F_resource_view_desc {

        ED_resource_view_type type = ED_resource_view_type::NONE;

		ED_resource_type overrided_resource_type = ED_resource_type::NONE;
		ED_resource_flag overrided_resource_flags = ED_resource_flag::NONE;

        TK<A_resource> resource_p;

        union {
            u32 base_mip_level = 0;
            u32 target_mip_level;
        };

    	union {
    		u64 mem_offset = 0;
    		u32 first_array_slice;
    	};

        sz overrided_element_count = 0;
        u32 overrided_stride = 0;

		u32 overrided_array_size = 0;
		ED_format overrided_format = ED_format::NONE;

    };
}
