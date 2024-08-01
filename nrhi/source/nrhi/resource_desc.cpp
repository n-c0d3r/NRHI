#include <nrhi/resource_desc.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

    F_resource_desc H_resource_desc::create_buffer_desc(
        u32 count,
        u32 stride,
        ED_resource_flag flags,
        ED_resource_heap_type heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
    ) {

        return {

            .element_count = count,
			.size = count * stride,

            .stride = stride,
			.type = ED_resource_type::BUFFER,

            .flags = flags,

            .heap_type = heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, .initial_state = initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, .layout = layout,
			.alignment = alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

        };
    }

    F_resource_desc H_resource_desc::create_buffer_desc(
        u32 count,
        ED_format format,
        ED_resource_flag flags,
        ED_resource_heap_type heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
    ) {

		u32 stride = H_format::stride(format);

        return {

            .element_count = count,
			.size = count * stride,

            .format = format,
            .stride = stride,
			.type = ED_resource_type::BUFFER,

            .flags = flags,

            .heap_type = heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, .initial_state = initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, .layout = layout,
			.alignment = alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

        };
    }

    F_resource_desc H_resource_desc::create_texture_1d_desc(
        u32 width,
        ED_format format,
        u32 mip_level_count,
        ED_resource_flag flags,
        ED_resource_heap_type heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
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
			.type = ED_resource_type::TEXTURE_1D,

            .flags = flags,

            .heap_type = heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, .initial_state = initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, .layout = layout,
			.alignment = alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

        };
    }

    F_resource_desc H_resource_desc::create_texture_2d_desc(
        u32 width,
        u32 height,
        ED_format format,
        u32 mip_level_count,
        F_sample_desc sample_desc,
        ED_resource_flag flags,
        ED_resource_heap_type heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
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
			.type = ED_resource_type::TEXTURE_2D,

            .sample_desc = sample_desc,
            .flags = flags,

            .heap_type = heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, .initial_state = initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, .layout = layout,
			.alignment = alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

        };
    }

    F_resource_desc H_resource_desc::create_texture_3d_desc(
        u32 width,
        u32 height,
        u32 depth,
        ED_format format,
        u32 mip_level_count,
        ED_resource_flag flags,
        ED_resource_heap_type heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
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
			.type = ED_resource_type::TEXTURE_3D,

            .flags = flags,

            .heap_type = heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, .initial_state = initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, .layout = layout,
			.alignment = alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

        };
    }

	F_resource_desc H_resource_desc::create_texture_2d_array_desc(
		u32 width,
		u32 height,
		u32 array_size,
		ED_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_flag flags,
		ED_resource_heap_type heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {

		u32 stride = H_format::stride(format);
		u32 size = width * height * array_size * stride;

		return {

			.width = width,
			.height = height,
			.array_size = array_size,
			.size = size,

			.format = format,
			.stride = stride,
			.mip_level_count = mip_level_count,
			.type = ED_resource_type::TEXTURE_2D_ARRAY,

			.sample_desc = sample_desc,
			.flags = flags,

			.heap_type = heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, .initial_state = initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, .layout = layout,
			.alignment = alignment
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

		};
	}

}