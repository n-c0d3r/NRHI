#include <nrhi/resource_desc.hpp>
#include <nrhi/format_helper.hpp>
#include <nrhi/resource.hpp>



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
    	sz size = count * stride;

        return {

            .element_count = count,
			.size = size,

            .stride = stride,
			.type = ED_resource_type::BUFFER,

            .flags = flags,

            .heap_type = heap_type,

        	.subresources = {
				F_subresource {
					.size = size,

					.element_count = count
				}
        	}

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
		sz size = count * stride;

        return {

            .element_count = count,
			.size = size,

            .format = format,
            .stride = stride,
			.type = ED_resource_type::BUFFER,

            .flags = flags,

        	.heap_type = heap_type,

			.subresources = {
            	F_subresource {
            		.size = size,
            		.element_count = count
				}
			}

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
		sz size = H_resource::first_pitch(stride, width);

		TG_fixed_vector<F_subresource, 1> subresources(mip_level_count);
    	u32 subresource_width = width;
    	sz subresource_offset = 0;
    	sz subresource_size = 0;
    	for(u32 i = 0; i < mip_level_count; ++i)
    	{
    		sz first_pitch = H_resource::first_pitch(
				stride,
				subresource_width
			);

    		subresource_size = first_pitch;

    		subresources[i] = {
				.offset = subresource_offset,
    			.size = subresource_size,

    			.width = subresource_width,

    			.first_pitch = first_pitch,

    			.mip_level = i
    		};

    		subresource_offset += subresource_size;

    		subresource_width = element_max((u32)subresource_width / 2, (u32)1);
    	}

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

        	.heap_type = heap_type,

			.subresources = std::move(subresources)

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
		sz size = H_resource::second_pitch(
			H_resource::first_pitch(stride, width),
			height
		);

    	TG_fixed_vector<F_subresource, 1> subresources(mip_level_count);
    	u32 subresource_width = width;
    	u32 subresource_height = height;
    	sz subresource_offset = 0;
    	sz subresource_size = 0;
    	for(u32 i = 0; i < mip_level_count; ++i)
    	{
    		sz first_pitch = H_resource::first_pitch(
				stride,
				subresource_width
			);
    		sz second_pitch = H_resource::second_pitch(
				first_pitch,
				subresource_height
			);

    		subresource_size = second_pitch;

    		subresources[i] = {
				.offset = subresource_offset,
				.size = subresource_size,

				.width = subresource_width,
				.height = subresource_height,

    			.first_pitch = first_pitch,

				.mip_level = i
			};

    		subresource_offset += subresource_size;

    		subresource_width = element_max((u32)subresource_width / 2, (u32)1);
    		subresource_height = element_max((u32)subresource_height / 2, (u32)1);
    	}

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

            .heap_type = heap_type,

        	.subresources = std::move(subresources)

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
    	sz size = H_resource::third_pitch(
    		H_resource::second_pitch(
				H_resource::first_pitch(stride, width),
				height
			),
			depth
		);

    	TG_fixed_vector<F_subresource, 1> subresources(mip_level_count);
    	u32 subresource_width = width;
    	u32 subresource_height = height;
    	u32 subresource_depth = depth;
    	sz subresource_offset = 0;
    	sz subresource_size = 0;
    	for(u32 i = 0; i < mip_level_count; ++i)
    	{
    		sz first_pitch = H_resource::first_pitch(
				stride,
				subresource_width
			);
    		sz second_pitch = H_resource::second_pitch(
				first_pitch,
				subresource_height
			);
    		sz third_pitch = H_resource::third_pitch(
				second_pitch,
				subresource_depth
			);

    		subresource_size = third_pitch;

    		subresources[i] = {
    			.offset = subresource_offset,
				.size = subresource_size,

				.width = subresource_width,
				.height = subresource_height,

				.first_pitch = first_pitch,

				.mip_level = i
			};

    		subresource_offset += subresource_size;

    		subresource_width = element_max((u32)subresource_width / 2, (u32)1);
    		subresource_height = element_max((u32)subresource_height / 2, (u32)1);
    		subresource_depth = element_max((u32)subresource_depth / 2, (u32)1);
    	}

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

        	.heap_type = heap_type,

			.subresources = std::move(subresources)

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
    	sz size = H_resource::third_pitch(
			H_resource::second_pitch(
				H_resource::first_pitch(stride, width),
				height
			),
			array_size
		);

    	TG_fixed_vector<F_subresource, 1> subresources(mip_level_count * array_size);
    	sz subresource_offset = 0;
    	sz subresource_size = 0;
    	for(u32 j = 0; j < array_size; ++j)
    	{
    		u32 subresource_width = width;
    		u32 subresource_height = height;

    		for(u32 i = 0; i < mip_level_count; ++i)
    		{
    			sz first_pitch = H_resource::first_pitch(
					stride,
					subresource_width
				);
    			sz second_pitch = H_resource::second_pitch(
					first_pitch,
					subresource_height
				);

    			subresource_size = second_pitch;

    			subresources[j * mip_level_count + i] = {
					.offset = subresource_offset,
					.size = subresource_size,

					.width = subresource_width,
					.height = subresource_height,

    				.first_pitch = first_pitch,

					.mip_level = i,
    				.array_slice = j
				};

    			subresource_offset += subresource_size;

    			subresource_width = element_max((u32)subresource_width / 2, (u32)1);
    			subresource_height = element_max((u32)subresource_height / 2, (u32)1);
    		}
    	}

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

			.heap_type = heap_type,

			.subresources = std::move(subresources)

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