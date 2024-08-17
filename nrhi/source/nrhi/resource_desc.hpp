#pragma once

/** @file nrhi/resource_desc.hpp
*
*   Implement resource desc.
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

#include <nrhi/format.hpp>
#include <nrhi/resource_heap_type.hpp>
#include <nrhi/resource_flag.hpp>

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
#include <nrhi/resource_state.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#include <nrhi/resource_type.hpp>

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#include <nrhi/resource_layout.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

#include <nrhi/sample_desc.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;



	struct F_subresource_data {

		void* data_p = 0;

	};
    using F_initial_resource_data = TG_vector<F_subresource_data>;



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	enum class E_resource_management_type {

		NONE,
		COMMITTED,
		PLACED,
		RESERVED

	};
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT



	struct F_subresource
	{
		sz offset = 0;
		sz size = 0;

		union {
			u32 width = 1;
			u32 element_count;
		};
		u32 height = 1;
		union {
			u32 array_size = 1;
			u32 depth;
		};

		sz first_pitch = 0;

		u32 mip_level = 0;
		u32 array_slice = 0;
	};



    struct F_resource_desc {

		union {
			u32 width = 0;
			u32 element_count;
		};
        u32 height = 0;
        union {
            u32 array_size = 0;
            u32 depth;
        };
		sz size = 0;

        ED_format format = ED_format::NONE;
        u32 stride = 0;
        u32 mip_level_count = 0;
		ED_resource_type type = ED_resource_type::NONE;

        F_sample_desc sample_desc;
        ED_resource_flag flags = ED_resource_flag::NONE;

        ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE;

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			b8 can_create_view = true;
		);

    	TG_fixed_vector<F_subresource, 1> subresources;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		ED_resource_state initial_state = ED_resource_state::COMMON;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		ED_resource_layout layout = ED_resource_layout::UNKNOWN;
		u64 alignment = 0;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

    };

    class NRHI_API H_resource_desc {

    public:
        static F_resource_desc create_buffer_desc(
            u32 count,
            u32 stride,
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = 0
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
        );

    public:
        static F_resource_desc create_buffer_desc(
            u32 count,
            ED_format format,
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = 0
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
        );

    public:
        template<typename F_element__>
        static F_resource_desc T_create_buffer_desc(
            u32 count,
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = 0
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
        ) {
            return create_buffer_desc(
                count,
                sizeof(F_element__),
                flags,
                heap_type
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
            );
        }

    public:
        static F_resource_desc create_texture_1d_desc(
            u32 width,
            ED_format format = ED_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = 0
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
        );

    public:
        static F_resource_desc create_texture_2d_desc(
            u32 width,
            u32 height,
            ED_format format = ED_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = 0
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
        );

    public:
        static F_resource_desc create_texture_3d_desc(
            u32 width,
            u32 height,
            u32 depth,
            ED_format format = ED_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = 0
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
        );

	public:
		static F_resource_desc create_texture_2d_array_desc(
			u32 width,
			u32 height,
			u32 array_size,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = 0
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

    };

}
