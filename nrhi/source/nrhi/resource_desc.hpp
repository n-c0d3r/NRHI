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
#include <nrhi/resource_structure_type.hpp>
#include <nrhi/sample_desc.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;



    struct F_initial_resource_data {

        void* system_mem_p = 0;
        u32 system_mem_pitch = 0;
        u32 system_mem_slice_pitch = 0;

        NCPP_FORCE_INLINE b8 is_valid() const noexcept { return system_mem_p; }

    };



    struct F_resource_desc {

        u32 width;
        u32 height;
        union {
            u32 array_size = 1;
            u32 depth;
        };

        union {
            E_format format;
            u32 stride;
        };

        u32 mip_level_count = 1;

        F_sample_desc sample_desc;

        E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;

        E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        E_resource_structure_type structure_type = E_resource_structure_type::NONE;

    };

    class NRHI_API H_resource_desc {

    public:
        static F_resource_desc create_buffer_desc(
            u32 width,
            u32 stride,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        );

    public:
        struct F_create_buffer_desc_params {

            u32 width;
            u32 stride;
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        static NCPP_FORCE_INLINE F_resource_desc create_buffer_desc(
            const F_create_buffer_desc_params& params
        ) {

            return create_buffer_desc(
                params.width,
                params.stride,
                params.heap_type,
                params.bind_flags
            );
        }

    public:
        template<typename F_element__>
        static F_resource_desc T_create_buffer_desc(
            u32 count,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        ) {

            return create_buffer_desc(
                sizeof(F_element__) * count,
                sizeof(F_element__),
                heap_type,
                bind_flags
            );
        }

    public:
        template<typename F_element__>
        struct TF_create_buffer_desc_params {

            u32 count;
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        template<typename F_element__>
        static NCPP_FORCE_INLINE F_resource_desc T_create_buffer_desc(
            const TF_create_buffer_desc_params<F_element__>& params
        ) {

            return T_create_buffer_desc<F_element__>(
                params.count,
                params.heap_type,
                params.bind_flags
            );
        }

    public:
        static F_resource_desc create_texture_1d_desc(
            u32 width,
            E_format format = E_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            F_sample_desc sample_desc = F_sample_desc{},
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        );

    public:
        struct F_create_texture_1d_desc_params {

            u32 width;
            E_format format = E_format::R8G8B8A8_UNORM;
            u32 mip_level_count = 1;
            F_sample_desc sample_desc = F_sample_desc{};
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        static NCPP_FORCE_INLINE F_resource_desc create_texture_1d_desc(
            const F_create_texture_1d_desc_params& params
        ) {

            return create_texture_1d_desc(
                params.width,
                params.format,
                params.mip_level_count,
                params.sample_desc,
                params.heap_type,
                params.bind_flags
            );
        }

    public:
        static F_resource_desc create_texture_2d_desc(
            u32 width,
            u32 height,
            E_format format = E_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            F_sample_desc sample_desc = F_sample_desc{},
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        );

    public:
        struct F_create_texture_2d_desc_params {

            u32 width;
            u32 height;
            E_format format = E_format::R8G8B8A8_UNORM;
            u32 mip_level_count = 1;
            F_sample_desc sample_desc = F_sample_desc{};
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        static NCPP_FORCE_INLINE F_resource_desc create_texture_2d_desc(
            const F_create_texture_2d_desc_params& params
        ) {

            return create_texture_2d_desc(
                params.width,
                params.height,
                params.format,
                params.mip_level_count,
                params.sample_desc,
                params.heap_type,
                params.bind_flags
            );
        }

    public:
        static F_resource_desc create_texture_3d_desc(
            u32 width,
            u32 height,
            u32 depth,
            E_format format = E_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            F_sample_desc sample_desc = F_sample_desc{},
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        );

    public:
        struct F_create_texture_3d_desc_params {

            u32 width;
            u32 height;
            u32 depth;
            E_format format = E_format::R8G8B8A8_UNORM;
            u32 mip_level_count = 1;
            F_sample_desc sample_desc = F_sample_desc{};
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        static NCPP_FORCE_INLINE F_resource_desc create_texture_3d_desc(
            const F_create_texture_3d_desc_params& params
        ) {

            return create_texture_3d_desc(
                params.width,
                params.height,
                params.depth,
                params.format,
                params.mip_level_count,
                params.sample_desc,
                params.heap_type,
                params.bind_flags
            );
        }

    };

}
