#include <nrhi/resource_base.hpp>



namespace nrhi {

    F_resource_desc H_resource_desc::create_buffer_desc(
        u32 width,
        u32 stride,
        E_resource_heap_type heap_type,
        E_resource_bind_flag bind_flags
    ) {

        return {

            .width = width,
            .height = 1,
            .depth = 1,

            .stride = stride,

            .mip_level_count = 1,

            .heap_type = heap_type,
            .bind_flags = bind_flags,

            .structure_type = E_resource_structure_type::BUFFER,

        };
    }

    F_resource_desc H_resource_desc::create_texture_1d_desc(
        u32 width,
        E_format format,
        u32 mip_level_count,
        F_sample_desc sample_desc,
        E_resource_heap_type heap_type,
        E_resource_bind_flag bind_flags
    ) {

        return {

            .width = width,
            .height = 1,
            .depth = 1,

            .format = format,

            .mip_level_count = mip_level_count,
            .sample_desc = sample_desc,

            .heap_type = heap_type,
            .bind_flags = bind_flags,

            .structure_type = E_resource_structure_type::TEXTURE_1D,

        };
    }

    F_resource_desc H_resource_desc::create_texture_2d_desc(
        u32 width,
        u32 height,
        E_format format,
        u32 mip_level_count,
        F_sample_desc sample_desc,
        E_resource_heap_type heap_type,
        E_resource_bind_flag bind_flags
    ) {

        return {

            .width = width,
            .height = height,
            .depth = 1,

            .format = format,

            .mip_level_count = mip_level_count,
            .sample_desc = sample_desc,

            .heap_type = heap_type,
            .bind_flags = bind_flags,

            .structure_type = E_resource_structure_type::TEXTURE_2D,

        };
    }

    F_resource_desc H_resource_desc::create_texture_3d_desc(
        u32 width,
        u32 height,
        u32 depth,
        E_format format,
        u32 mip_level_count,
        F_sample_desc sample_desc,
        E_resource_heap_type heap_type,
        E_resource_bind_flag bind_flags
    ) {

        return {

            .width = width,
            .height = height,
            .depth = depth,

            .format = format,

            .mip_level_count = mip_level_count,
            .sample_desc = sample_desc,

            .heap_type = heap_type,
            .bind_flags = bind_flags,

            .structure_type = E_resource_structure_type::TEXTURE_3D,

        };
    }



    A_resource::A_resource(TK_valid<A_device> device_p, const F_resource_desc& desc) :
        device_p_(device_p),
        desc_(desc)
    {

    }
    A_resource::~A_resource() {

    }

}