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
#include <nrhi/resource_bind_flag.hpp>
#include <nrhi/resource_type.hpp>
#include <nrhi/sample_desc.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;



	struct F_subresource_data {

		void* data_p = 0;

	};
    using F_initial_resource_data = TG_vector<F_subresource_data>;



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
		u32 size = 0;

        ED_format format = ED_format::NONE;
        u32 stride = 0;

        u32 mip_level_count = 0;

        F_sample_desc sample_desc;

        ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE;

        ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE;

		b8 is_mip_map_generatable = false;

        ED_resource_type type = ED_resource_type::NONE;

        b8 can_create_view = true;

    };

    class NRHI_API H_resource_desc {

    public:
        static F_resource_desc create_buffer_desc(
            u32 count,
            u32 stride,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        );

    public:
        static F_resource_desc create_buffer_desc(
            u32 count,
            ED_format format,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F_element__>
        static F_resource_desc T_create_buffer_desc(
            u32 count,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        ) {

            return create_buffer_desc(
                count,
                sizeof(F_element__),
                bind_flags,
                heap_type
            );
        }

    public:
        static F_resource_desc create_structured_buffer_desc(
            u32 count,
            u32 stride,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F_element__>
        static F_resource_desc T_create_structured_buffer_desc(
            u32 count,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        ) {

            return create_structured_buffer_desc(
                count,
                sizeof(F_element__),
                bind_flags,
                heap_type
            );
        }

    public:
        static F_resource_desc create_texture_1d_desc(
            u32 width,
            ED_format format = ED_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
        );

    public:
        static F_resource_desc create_texture_2d_desc(
            u32 width,
            u32 height,
            ED_format format = ED_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
        );

    public:
        static F_resource_desc create_texture_3d_desc(
            u32 width,
            u32 height,
            u32 depth,
            ED_format format = ED_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
        );

	public:
		static F_resource_desc create_texture_2d_array_desc(
			u32 width,
			u32 height,
			u32 count,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
		);

	public:
		static F_resource_desc create_texture_cube_desc(
			u32 width,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
		);

	public:
		static F_resource_desc create_indirect_buffer_desc(
			u32 count,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);

    };

}
