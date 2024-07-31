#include <nrhi/buffer.hpp>
#include <nrhi/resource.hpp>



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
		, ED_resource_layout layout
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
				, layout
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
		, ED_resource_layout layout
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
				, layout
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
		, ED_resource_layout layout
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		buffer_p->rebuild(
			{},
			H_resource_desc::create_buffer_desc(
				count,
				stride,
				flags,
				heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout
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
		, ED_resource_layout layout
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	) {
		buffer_p->rebuild(
			{},
			H_resource_desc::create_buffer_desc(
				count,
				format,
				flags,
				heap_type

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			)
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

}