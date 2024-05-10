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



    struct F_initial_resource_data {

        void* data_p = 0;
        u32 system_mem_pitch = 0;
        u32 system_mem_slice_pitch = 0;

        NCPP_FORCE_INLINE b8 is_valid() const noexcept { return data_p; }

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
		u32 size = 0;

        E_format format = E_format::NONE;
        u32 stride = 0;

        u32 mip_level_count = 0;

        F_sample_desc sample_desc;

        E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;

        E_resource_type type = E_resource_type::NONE;

        b8 can_create_view = true;

    };

    class NRHI_API H_resource_desc {

    public:
        static F_resource_desc create_buffer_desc(
            u32 count,
            u32 stride,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        );

    public:
        static F_resource_desc create_buffer_desc(
            u32 count,
            E_format format,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F_element__>
        static F_resource_desc T_create_buffer_desc(
            u32 count,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
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
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F_element__>
        static F_resource_desc T_create_structured_buffer_desc(
            u32 count,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
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
            E_format format = E_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        );

    public:
        static F_resource_desc create_texture_2d_desc(
            u32 width,
            u32 height,
            E_format format = E_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        );

    public:
        static F_resource_desc create_texture_3d_desc(
            u32 width,
            u32 height,
            u32 depth,
            E_format format = E_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        );

	public:
		static F_resource_desc create_texture_2d_array_desc(
			u32 width,
			u32 height,
			u32 count,
			E_format format = E_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
			E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
		);

	public:
		static F_resource_desc create_texture_cube_desc(
			u32 width,
			E_format format = E_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
			E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
		);

	public:
		static F_resource_desc create_indirect_buffer_desc(
			u32 count,
			E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
			E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
		);

    };

}
