#include <nrhi/buffer.hpp>
#include <nrhi/resource.hpp>

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#include <nrhi/resource_heap.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT



namespace nrhi {

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT
    U_buffer_handle H_buffer::create(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        u32 count,
        u32 stride,
        ED_resource_flag flags,
        ED_resource_heap_type heap_type
    ) {
        return H_resource::create_buffer(
            device_p,
            initial_data,
            H_resource_desc::create_buffer_desc(
                count,
                stride,
                flags,
                heap_type
            )
        );
    }

    U_buffer_handle H_buffer::create(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        u32 count,
        ED_format format,
        ED_resource_flag flags,
        ED_resource_heap_type heap_type
    ) {
        return H_resource::create_buffer(
            device_p,
            initial_data,
            H_resource_desc::create_buffer_desc(
                count,
                format,
                flags,
                heap_type
            )
        );
    }



	void H_buffer::rebuild(
		KPA_valid_buffer_handle buffer_p,
		const F_initial_resource_data& initial_data,
		u32 count,
		u32 stride,
		ED_resource_flag flags,
		ED_resource_heap_type heap_type
	) {
		buffer_p->rebuild(
			initial_data,
			H_resource_desc::create_buffer_desc(
				count,
				stride,
				flags,
				heap_type
			)
		);
	}

	void H_buffer::rebuild(
		KPA_valid_buffer_handle buffer_p,
		const F_initial_resource_data& initial_data,
		u32 count,
		ED_format format,
		ED_resource_flag flags,
		ED_resource_heap_type heap_type
	) {
		buffer_p->rebuild(
			initial_data,
			H_resource_desc::create_buffer_desc(
				count,
				format,
				flags,
				heap_type
			)
		);
	}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	U_buffer_handle H_buffer::create_committed(
		TKPA_valid<A_device> device_p,
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
		return H_resource::create_committed_buffer(
			device_p,
			H_resource_desc::create_buffer_desc(
				count,
				stride,
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
	U_buffer_handle H_buffer::create_committed(
		TKPA_valid<A_device> device_p,
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
		return H_resource::create_committed_buffer(
			device_p,
			H_resource_desc::create_buffer_desc(
				count,
				format,
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



	void H_buffer::rebuild_committed(
		KPA_valid_buffer_handle buffer_p,
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
		buffer_p->rebuild_committed(
			H_resource_desc::create_buffer_desc(
				count,
				stride,
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
	void H_buffer::rebuild_committed(
		KPA_valid_buffer_handle buffer_p,
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
		buffer_p->rebuild_committed(
			H_resource_desc::create_buffer_desc(
				count,
				format,
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
	U_buffer_handle H_buffer::create_placed(
		TKPA_valid<A_device> device_p,
		u32 count,
		u32 stride,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
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
		return H_resource::create_placed_buffer(
			device_p,
			H_resource_desc::create_buffer_desc(
				count,
				stride,
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
	U_buffer_handle H_buffer::create_placed(
		TKPA_valid<A_device> device_p,
		u32 count,
		ED_format format,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
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
		return H_resource::create_placed_buffer(
			device_p,
			H_resource_desc::create_buffer_desc(
				count,
				format,
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



	void H_buffer::rebuild_placed(
		KPA_valid_buffer_handle buffer_p,
		u32 count,
		u32 stride,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
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
		buffer_p->rebuild_placed(
			H_resource_desc::create_buffer_desc(
				count,
				stride,
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
	void H_buffer::rebuild_placed(
		KPA_valid_buffer_handle buffer_p,
		u32 count,
		ED_format format,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
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
		buffer_p->rebuild_placed(
			H_resource_desc::create_buffer_desc(
				count,
				format,
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
	U_buffer_handle H_buffer::create_reserved(
		TKPA_valid<A_device> device_p,
		u32 count,
		u32 stride,
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
		return H_resource::create_reserved_buffer(
			device_p,
			H_resource_desc::create_buffer_desc(
				count,
				stride,
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
	U_buffer_handle H_buffer::create_reserved(
		TKPA_valid<A_device> device_p,
		u32 count,
		ED_format format,
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
		return H_resource::create_reserved_buffer(
			device_p,
			H_resource_desc::create_buffer_desc(
				count,
				format,
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



	void H_buffer::rebuild_reserved(
		KPA_valid_buffer_handle buffer_p,
		u32 count,
		u32 stride,
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
		buffer_p->rebuild_reserved(
			H_resource_desc::create_buffer_desc(
				count,
				stride,
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
	void H_buffer::rebuild_reserved(
		KPA_valid_buffer_handle buffer_p,
		u32 count,
		ED_format format,
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
		buffer_p->rebuild_reserved(
			H_resource_desc::create_buffer_desc(
				count,
				format,
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