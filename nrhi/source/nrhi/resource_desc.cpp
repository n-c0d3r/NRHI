#include <nrhi/resource_desc.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

    F_resource_desc H_resource_desc::create_buffer_desc(
        u32 count,
        u32 stride,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type
    ) {

        return {

            .element_count = count,

			.size = count * stride,

            .stride = stride,

            .bind_flags = bind_flags,
            .heap_type = heap_type,

            .type = E_resource_type::BUFFER

        };
    }

    F_resource_desc H_resource_desc::create_buffer_desc(
        u32 count,
        E_format format,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type
    ) {

		u32 stride = H_format::stride(format);

        return {

            .element_count = count,

			.size = count * stride,

            .format = format,
            .stride = stride,

            .bind_flags = bind_flags,
            .heap_type = heap_type,

            .type = E_resource_type::BUFFER

        };
    }

    F_resource_desc H_resource_desc::create_structured_buffer_desc(
        u32 count,
        u32 stride,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type
    ) {

        return {

            .element_count = count,

            .size = count * stride,

            .stride = stride,

            .bind_flags = bind_flags,
            .heap_type = heap_type,

            .type = E_resource_type::STRUCTURED_BUFFER

        };
    }

    F_resource_desc H_resource_desc::create_texture_1d_desc(
        u32 width,
        E_format format,
        u32 mip_level_count,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type,
		b8 is_mip_map_generatable
    ) {

		u32 stride = H_format::stride(format);
		u32 size = width * stride;

        return {

            .width = width,
            .height = 1,
            .depth = 1,

            .size = size,

            .format = format,
            .stride = stride,

            .mip_level_count = mip_level_count,

            .bind_flags = bind_flags,
            .heap_type = heap_type,

			.is_mip_map_generatable = is_mip_map_generatable,

            .type = E_resource_type::TEXTURE_1D

        };
    }

    F_resource_desc H_resource_desc::create_texture_2d_desc(
        u32 width,
        u32 height,
        E_format format,
        u32 mip_level_count,
        F_sample_desc sample_desc,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type,
		b8 is_mip_map_generatable
    ) {

		u32 stride = H_format::stride(format);
		u32 size = width * height * stride;

        return {

            .width = width,
            .height = height,
            .depth = 1,

            .size = size,

            .format = format,
            .stride = stride,

            .mip_level_count = mip_level_count,
            .sample_desc = sample_desc,

            .bind_flags = bind_flags,
            .heap_type = heap_type,

			.is_mip_map_generatable = is_mip_map_generatable,

            .type = E_resource_type::TEXTURE_2D

        };
    }

    F_resource_desc H_resource_desc::create_texture_3d_desc(
        u32 width,
        u32 height,
        u32 depth,
        E_format format,
        u32 mip_level_count,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type,
		b8 is_mip_map_generatable
    ) {

		u32 stride = H_format::stride(format);
		u32 size = width * height * depth * stride;

        return {

            .width = width,
            .height = height,
            .depth = depth,

            .size = size,

            .format = format,
            .stride = stride,

            .mip_level_count = mip_level_count,

            .bind_flags = bind_flags,
            .heap_type = heap_type,

			.is_mip_map_generatable = is_mip_map_generatable,

            .type = E_resource_type::TEXTURE_3D

        };
    }

	F_resource_desc H_resource_desc::create_texture_2d_array_desc(
		u32 width,
		u32 height,
		u32 count,
		E_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		E_resource_bind_flag bind_flags,
		E_resource_heap_type heap_type,
		b8 is_mip_map_generatable
	) {

		u32 stride = H_format::stride(format);
		u32 size = width * height * count * stride;

		return {

			.width = width,
			.height = height,
			.array_size = count,

			.size = size,

			.format = format,
			.stride = stride,

			.mip_level_count = mip_level_count,
			.sample_desc = sample_desc,

			.bind_flags = bind_flags,
			.heap_type = heap_type,

			.is_mip_map_generatable = is_mip_map_generatable,

			.type = E_resource_type::TEXTURE_2D

		};
	}

	F_resource_desc H_resource_desc::create_texture_cube_desc(
		u32 width,
		E_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		E_resource_bind_flag bind_flags,
		E_resource_heap_type heap_type,
		b8 is_mip_map_generatable
	) {

		u32 stride = H_format::stride(format);
		u32 size = width * width * 6 * stride;

		return {

			.width = width,
			.height = width,
			.array_size = 6,

			.size = size,

			.format = format,
			.stride = stride,

			.mip_level_count = mip_level_count,
			.sample_desc = sample_desc,

			.bind_flags = bind_flags,
			.heap_type = heap_type,

			.is_mip_map_generatable = is_mip_map_generatable,

			.type = E_resource_type::TEXTURE_CUBE

		};
	}

	F_resource_desc H_resource_desc::create_indirect_buffer_desc(
		u32 count,
		E_resource_bind_flag bind_flags,
		E_resource_heap_type heap_type
	) {

		return {

			.element_count = count,

			.size = count * u32(sizeof(u32)),

			.format = E_format::R32_UINT,
			.stride = sizeof(u32),

			.mip_level_count = 1,

			.bind_flags = bind_flags,
			.heap_type = heap_type,

			.type = E_resource_type::INDIRECT_BUFFER

		};
	}

}