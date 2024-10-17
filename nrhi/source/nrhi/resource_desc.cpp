#include <nrhi/resource_desc.hpp>
#include <nrhi/format_helper.hpp>
#include <nrhi/resource.hpp>



namespace nrhi
{
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
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
    ) {
        return {
            .element_count = count,

            .stride = stride,
			.type = ED_resource_type::BUFFER,

            .flags = flags,

            .heap_type = heap_type,

        	.subresource_infos = {
				F_subresource_info {
					.element_count = count
				}
        	}

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, .initial_state = initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, .layout = layout,
			.alignment = alignment,
			.clear_value = clear_value
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
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
    ) {

		u32 stride = H_format::stride(format);

        return {
            .element_count = count,

            .format = format,
            .stride = stride,
			.type = ED_resource_type::BUFFER,

            .flags = flags,

        	.heap_type = heap_type,

			.subresource_infos = {
            	F_subresource_info {
            		.element_count = count
				}
			}

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, .initial_state = initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, .layout = layout,
			.alignment = alignment,
			.clear_value = clear_value
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
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
    ) {
		u32 stride = H_format::stride(format);

		F_subresource_infos subresource_infos(mip_level_count);
    	u32 subresource_width = width;
    	for(u32 i = 0; i < mip_level_count; ++i)
    	{
    		subresource_infos[i] = {
    			.width = subresource_width,

    			.mip_level = i
    		};

    		subresource_width = element_max((u32)subresource_width / 2, (u32)1);
    	}

        return {
            .width = width,
            .height = 1,
            .depth = 1,

            .format = format,
            .stride = stride,
            .mip_level_count = mip_level_count,
			.type = ED_resource_type::TEXTURE_1D,

            .flags = flags,

        	.heap_type = heap_type,

			.subresource_infos = std::move(subresource_infos)

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, .initial_state = initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, .layout = layout,
        	.alignment = alignment,
			.clear_value = clear_value
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
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
    ) {
		u32 stride = H_format::stride(format);

    	F_subresource_infos subresource_infos(mip_level_count);
    	u32 subresource_width = width;
    	u32 subresource_height = height;
    	for(u32 i = 0; i < mip_level_count; ++i)
    	{
    		subresource_infos[i] = {
				.width = subresource_width,
				.height = subresource_height,

				.mip_level = i
			};

    		subresource_width = element_max((u32)subresource_width / 2, (u32)1);
    		subresource_height = element_max((u32)subresource_height / 2, (u32)1);
    	}

        return {
            .width = width,
            .height = height,
            .depth = 1,

            .format = format,
            .stride = stride,
            .mip_level_count = mip_level_count,
			.type = ED_resource_type::TEXTURE_2D,

            .sample_desc = sample_desc,
            .flags = flags,

            .heap_type = heap_type,

        	.subresource_infos = std::move(subresource_infos)

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, .initial_state = initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, .layout = layout,
        	.alignment = alignment,
			.clear_value = clear_value
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
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
    ) {
		u32 stride = H_format::stride(format);

    	F_subresource_infos subresource_infos(mip_level_count);
    	u32 subresource_width = width;
    	u32 subresource_height = height;
    	u32 subresource_depth = depth;
    	for(u32 i = 0; i < mip_level_count; ++i)
    	{
    		subresource_infos[i] = {
				.width = subresource_width,
				.height = subresource_height,

				.mip_level = i
			};

    		subresource_width = element_max((u32)subresource_width / 2, (u32)1);
    		subresource_height = element_max((u32)subresource_height / 2, (u32)1);
    		subresource_depth = element_max((u32)subresource_depth / 2, (u32)1);
    	}

        return {

            .width = width,
            .height = height,
            .depth = depth,

            .format = format,
            .stride = stride,
            .mip_level_count = mip_level_count,
			.type = ED_resource_type::TEXTURE_3D,

            .flags = flags,

        	.heap_type = heap_type,

			.subresource_infos = std::move(subresource_infos)

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, .initial_state = initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, .layout = layout,
        	.alignment = alignment,
			.clear_value = clear_value
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
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
    	u32 stride = H_format::stride(format);

    	F_subresource_infos subresource_infos(mip_level_count * array_size);
    	for(u32 j = 0; j < array_size; ++j)
    	{
    		u32 subresource_width = width;
    		u32 subresource_height = height;

    		for(u32 i = 0; i < mip_level_count; ++i)
    		{
    			subresource_infos[j * mip_level_count + i] = {
					.width = subresource_width,
					.height = subresource_height,

					.mip_level = i,
    				.array_slice = j
				};

    			subresource_width = element_max((u32)subresource_width / 2, (u32)1);
    			subresource_height = element_max((u32)subresource_height / 2, (u32)1);
    		}
    	}

		return {
			.width = width,
			.height = height,
			.array_size = array_size,

			.format = format,
			.stride = stride,
			.mip_level_count = mip_level_count,
			.type = ED_resource_type::TEXTURE_2D_ARRAY,

			.sample_desc = sample_desc,
			.flags = flags,

			.heap_type = heap_type,

			.subresource_infos = std::move(subresource_infos)

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, .initial_state = initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, .layout = layout,
			.alignment = alignment,
			.clear_value = clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		};
	}
}