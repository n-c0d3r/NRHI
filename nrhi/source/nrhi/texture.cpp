#include <nrhi/texture.hpp>
#include <nrhi/resource.hpp>

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#include <nrhi/resource_heap.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT


namespace nrhi {

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT
    U_texture_1d_handle H_texture::create_1d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        u32 width,
        ED_format format,
        u32 mip_level_count,
        ED_resource_flag flags,
        ED_resource_heap_type heap_type
    ) {

        return H_resource::create_texture_1d(
            device_p,
            initial_data,
            H_resource_desc::create_texture_1d_desc(
                width,
                format,
                mip_level_count,
                flags,
                heap_type
            )
        );
    }

    U_texture_2d_handle H_texture::create_2d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        u32 width,
        u32 height,
        ED_format format,
        u32 mip_level_count,
        F_sample_desc sample_desc,
        ED_resource_flag flags,
        ED_resource_heap_type heap_type
    ) {

        return H_resource::create_texture_2d(
            device_p,
            initial_data,
            H_resource_desc::create_texture_2d_desc(
                width,
                height,
                format,
                mip_level_count,
                sample_desc,
                flags,
                heap_type
            )
        );
    }

    U_texture_3d_handle H_texture::create_3d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        u32 width,
        u32 height,
        u32 depth,
        ED_format format,
        u32 mip_level_count,
        ED_resource_flag flags,
        ED_resource_heap_type heap_type
    ) {

        return H_resource::create_texture_3d(
            device_p,
            initial_data,
            H_resource_desc::create_texture_3d_desc(
                width,
                height,
                depth,
                format,
                mip_level_count,
                flags,
                heap_type
            )
        );
    }

	U_texture_2d_array_handle H_texture::create_2d_array(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_data,
		u32 width,
		u32 height,
		u32 array_size,
		ED_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_flag flags,
		ED_resource_heap_type heap_type
	) {

		return H_resource::create_texture_2d_array(
			device_p,
			initial_data,
			H_resource_desc::create_texture_2d_array_desc(
				width,
				height,
				array_size,
				format,
				mip_level_count,
				sample_desc,
				flags,
				heap_type
			)
		);
	}



	void H_texture::rebuild_1d(
		KPA_valid_texture_1d_handle texture_1d_p,
		const F_initial_resource_data& initial_data,
		u32 width,
		ED_format format,
		u32 mip_level_count,
		ED_resource_flag flags,
		ED_resource_heap_type heap_type
	) {

		texture_1d_p->rebuild(
			initial_data,
			H_resource_desc::create_texture_1d_desc(
				width,
				format,
				mip_level_count,
				flags,
				heap_type
			)
		);
	}

	void H_texture::rebuild_2d(
		KPA_valid_texture_2d_handle texture_2d_p,
		const F_initial_resource_data& initial_data,
		u32 width,
		u32 height,
		ED_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_flag flags,
		ED_resource_heap_type heap_type
	) {

		texture_2d_p->rebuild(
			initial_data,
			H_resource_desc::create_texture_2d_desc(
				width,
				height,
				format,
				mip_level_count,
				sample_desc,
				flags,
				heap_type
			)
		);
	}

	void H_texture::rebuild_3d(
		KPA_valid_texture_3d_handle texture_3d_p,
		const F_initial_resource_data& initial_data,
		u32 width,
		u32 height,
		u32 depth,
		ED_format format,
		u32 mip_level_count,
		ED_resource_flag flags,
		ED_resource_heap_type heap_type
	) {

		texture_3d_p->rebuild(
			initial_data,
			H_resource_desc::create_texture_3d_desc(
				width,
				height,
				depth,
				format,
				mip_level_count,
				flags,
				heap_type
			)
		);
	}

	void H_texture::rebuild_2d_array(
		KPA_valid_texture_2d_array_handle texture_2d_array_p,
		const F_initial_resource_data& initial_data,
		u32 width,
		u32 height,
		u32 array_size,
		ED_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_flag flags,
		ED_resource_heap_type heap_type
	) {

		texture_2d_array_p->rebuild(
			initial_data,
			H_resource_desc::create_texture_2d_array_desc(
				width,
				height,
				array_size,
				format,
				mip_level_count,
				sample_desc,
				flags,
				heap_type
			)
		);
	}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	U_texture_1d_handle H_texture::create_committed_1d(
		TKPA_valid<A_device> device_p,
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
		return H_resource::create_committed_texture_1d(
			device_p,
			H_resource_desc::create_texture_1d_desc(
				width,
				format,
				mip_level_count,
				flags,
				heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}

	U_texture_2d_handle H_texture::create_committed_2d(
		TKPA_valid<A_device> device_p,
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
		return H_resource::create_committed_texture_2d(
			device_p,
			H_resource_desc::create_texture_2d_desc(
				width,
				height,
				format,
				mip_level_count,
				sample_desc,
				flags,
				heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}

	U_texture_3d_handle H_texture::create_committed_3d(
		TKPA_valid<A_device> device_p,
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
		return H_resource::create_committed_texture_3d(
			device_p,
			H_resource_desc::create_texture_3d_desc(
				width,
				height,
				depth,
				format,
				mip_level_count,
				flags,
				heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}

	U_texture_2d_array_handle H_texture::create_committed_2d_array(
		TKPA_valid<A_device> device_p,
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
		return H_resource::create_committed_texture_2d_array(
			device_p,
			H_resource_desc::create_texture_2d_array_desc(
				width,
				height,
				array_size,
				format,
				mip_level_count,
				sample_desc,
				flags,
				heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}



	void H_texture::rebuild_committed_1d(
		KPA_valid_texture_1d_handle texture_1d_p,
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
		texture_1d_p->rebuild_committed(
			H_resource_desc::create_texture_1d_desc(
				width,
				format,
				mip_level_count,
				flags,
				heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}

	void H_texture::rebuild_committed_2d(
		KPA_valid_texture_2d_handle texture_2d_p,
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
		texture_2d_p->rebuild_committed(
			H_resource_desc::create_texture_2d_desc(
				width,
				height,
				format,
				mip_level_count,
				sample_desc,
				flags,
				heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}

	void H_texture::rebuild_committed_3d(
		KPA_valid_texture_3d_handle texture_3d_p,
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
		texture_3d_p->rebuild_committed(
			H_resource_desc::create_texture_3d_desc(
				width,
				height,
				depth,
				format,
				mip_level_count,
				flags,
				heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}

	void H_texture::rebuild_committed_2d_array(
		KPA_valid_texture_2d_array_handle texture_2d_array_p,
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
		texture_2d_array_p->rebuild_committed(
			H_resource_desc::create_texture_2d_array_desc(
				width,
				height,
				array_size,
				format,
				mip_level_count,
				sample_desc,
				flags,
				heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	U_texture_1d_handle H_texture::create_placed_1d(
		TKPA_valid<A_device> device_p,
		u32 width,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
		ED_format format,
		u32 mip_level_count,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		return H_resource::create_placed_texture_1d(
			device_p,
			H_resource_desc::create_texture_1d_desc(
				width,
				format,
				mip_level_count,
				flags,
				heap_p->desc().type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			),
			heap_p,
			heap_offset
		);
	}

	U_texture_2d_handle H_texture::create_placed_2d(
		TKPA_valid<A_device> device_p,
		u32 width,
		u32 height,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
		ED_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		return H_resource::create_placed_texture_2d(
			device_p,
			H_resource_desc::create_texture_2d_desc(
				width,
				height,
				format,
				mip_level_count,
				sample_desc,
				flags,
				heap_p->desc().type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			),
			heap_p,
			heap_offset
		);
	}

	U_texture_3d_handle H_texture::create_placed_3d(
		TKPA_valid<A_device> device_p,
		u32 width,
		u32 height,
		u32 depth,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
		ED_format format,
		u32 mip_level_count,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		return H_resource::create_placed_texture_3d(
			device_p,
			H_resource_desc::create_texture_3d_desc(
				width,
				height,
				depth,
				format,
				mip_level_count,
				flags,
				heap_p->desc().type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			),
			heap_p,
			heap_offset
		);
	}

	U_texture_2d_array_handle H_texture::create_placed_2d_array(
		TKPA_valid<A_device> device_p,
		u32 width,
		u32 height,
		u32 array_size,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
		ED_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		return H_resource::create_placed_texture_2d_array(
			device_p,
			H_resource_desc::create_texture_2d_array_desc(
				width,
				height,
				array_size,
				format,
				mip_level_count,
				sample_desc,
				flags,
				heap_p->desc().type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			),
			heap_p,
			heap_offset
		);
	}



	void H_texture::rebuild_placed_1d(
		KPA_valid_texture_1d_handle texture_1d_p,
		u32 width,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
		ED_format format,
		u32 mip_level_count,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		texture_1d_p->rebuild_placed(
			H_resource_desc::create_texture_1d_desc(
				width,
				format,
				mip_level_count,
				flags,
				heap_p->desc().type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			),
			heap_p,
			heap_offset
		);
	}

	void H_texture::rebuild_placed_2d(
		KPA_valid_texture_2d_handle texture_2d_p,
		u32 width,
		u32 height,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
		ED_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		texture_2d_p->rebuild_placed(
			H_resource_desc::create_texture_2d_desc(
				width,
				height,
				format,
				mip_level_count,
				sample_desc,
				flags,
				heap_p->desc().type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			),
			heap_p,
			heap_offset
		);
	}

	void H_texture::rebuild_placed_3d(
		KPA_valid_texture_3d_handle texture_3d_p,
		u32 width,
		u32 height,
		u32 depth,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
		ED_format format,
		u32 mip_level_count,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		texture_3d_p->rebuild_placed(
			H_resource_desc::create_texture_3d_desc(
				width,
				height,
				depth,
				format,
				mip_level_count,
				flags,
				heap_p->desc().type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			),
			heap_p,
			heap_offset
		);
	}

	void H_texture::rebuild_placed_2d_array(
		KPA_valid_texture_2d_array_handle texture_2d_array_p,
		u32 width,
		u32 height,
		u32 array_size,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
		ED_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		texture_2d_array_p->rebuild_placed(
			H_resource_desc::create_texture_2d_array_desc(
				width,
				height,
				array_size,
				format,
				mip_level_count,
				sample_desc,
				flags,
				heap_p->desc().type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			),
			heap_p,
			heap_offset
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	U_texture_1d_handle H_texture::create_reserved_1d(
		TKPA_valid<A_device> device_p,
		u32 width,
		ED_format format,
		u32 mip_level_count,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		return H_resource::create_reserved_texture_1d(
			device_p,
			H_resource_desc::create_texture_1d_desc(
				width,
				format,
				mip_level_count,
				flags,
				ED_resource_heap_type::DEFAULT

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}

	U_texture_2d_handle H_texture::create_reserved_2d(
		TKPA_valid<A_device> device_p,
		u32 width,
		u32 height,
		ED_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		return H_resource::create_reserved_texture_2d(
			device_p,
			H_resource_desc::create_texture_2d_desc(
				width,
				height,
				format,
				mip_level_count,
				sample_desc,
				flags,
				ED_resource_heap_type::DEFAULT

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}

	U_texture_3d_handle H_texture::create_reserved_3d(
		TKPA_valid<A_device> device_p,
		u32 width,
		u32 height,
		u32 depth,
		ED_format format,
		u32 mip_level_count,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		return H_resource::create_reserved_texture_3d(
			device_p,
			H_resource_desc::create_texture_3d_desc(
				width,
				height,
				depth,
				format,
				mip_level_count,
				flags,
				ED_resource_heap_type::DEFAULT

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}

	U_texture_2d_array_handle H_texture::create_reserved_2d_array(
		TKPA_valid<A_device> device_p,
		u32 width,
		u32 height,
		u32 array_size,
		ED_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		return H_resource::create_reserved_texture_2d_array(
			device_p,
			H_resource_desc::create_texture_2d_array_desc(
				width,
				height,
				array_size,
				format,
				mip_level_count,
				sample_desc,
				flags,
				ED_resource_heap_type::DEFAULT

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}



	void H_texture::rebuild_reserved_1d(
		KPA_valid_texture_1d_handle texture_1d_p,
		u32 width,
		ED_format format,
		u32 mip_level_count,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		texture_1d_p->rebuild_reserved(
			H_resource_desc::create_texture_1d_desc(
				width,
				format,
				mip_level_count,
				flags,
				ED_resource_heap_type::DEFAULT

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}

	void H_texture::rebuild_reserved_2d(
		KPA_valid_texture_2d_handle texture_2d_p,
		u32 width,
		u32 height,
		ED_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		texture_2d_p->rebuild_reserved(
			H_resource_desc::create_texture_2d_desc(
				width,
				height,
				format,
				mip_level_count,
				sample_desc,
				flags,
				ED_resource_heap_type::DEFAULT

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}

	void H_texture::rebuild_reserved_3d(
		KPA_valid_texture_3d_handle texture_3d_p,
		u32 width,
		u32 height,
		u32 depth,
		ED_format format,
		u32 mip_level_count,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		texture_3d_p->rebuild_reserved(
			H_resource_desc::create_texture_3d_desc(
				width,
				height,
				depth,
				format,
				mip_level_count,
				flags,
				ED_resource_heap_type::DEFAULT

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}

	void H_texture::rebuild_reserved_2d_array(
		KPA_valid_texture_2d_array_handle texture_2d_array_p,
		u32 width,
		u32 height,
		u32 array_size,
		ED_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_flag flags

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		, ED_resource_state initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		, ED_resource_layout layout,
		u64 alignment,
		const F_resource_clear_value& clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		texture_2d_array_p->rebuild_reserved(
			H_resource_desc::create_texture_2d_array_desc(
				width,
				height,
				array_size,
				format,
				mip_level_count,
				sample_desc,
				flags,
				ED_resource_heap_type::DEFAULT

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

}