#include <nrhi/buffer.hpp>
#include <nrhi/resource.hpp>



namespace nrhi {

    U_buffer_handle H_buffer::create(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        u32 count,
        u32 stride,
        ED_resource_bind_flag bind_flags,
        ED_resource_heap_type heap_type
    ) {
        return H_resource::create_buffer(
            device_p,
            initial_data,
            H_resource_desc::create_buffer_desc(
                count,
                stride,
                bind_flags,
                heap_type
            )
        );
    }

    U_buffer_handle H_buffer::create(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        u32 count,
        ED_format format,
        ED_resource_bind_flag bind_flags,
        ED_resource_heap_type heap_type
    ) {
        return H_resource::create_buffer(
            device_p,
            initial_data,
            H_resource_desc::create_buffer_desc(
                count,
                format,
                bind_flags,
                heap_type
            )
        );
    }

    U_structured_buffer_handle H_buffer::create_structured(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        u32 count,
        u32 stride,
        ED_resource_bind_flag bind_flags,
        ED_resource_heap_type heap_type
    ) {
        return H_resource::create_structured_buffer(
            device_p,
            initial_data,
            H_resource_desc::create_structured_buffer_desc(
                count,
                stride,
                bind_flags,
                heap_type
            )
        );
    }

	U_indirect_buffer_handle H_buffer::create_indirect(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_data,
		u32 count,
		ED_resource_bind_flag bind_flags,
		ED_resource_heap_type heap_type
	) {
		return H_resource::create_indirect_buffer(
			device_p,
			initial_data,
			H_resource_desc::create_indirect_buffer_desc(
				count,
				bind_flags,
				heap_type
			)
		);
	}



	void H_buffer::rebuild(
		KPA_valid_buffer_handle buffer_p,
		const F_initial_resource_data& initial_data,
		u32 count,
		u32 stride,
		ED_resource_bind_flag bind_flags,
		ED_resource_heap_type heap_type
	) {
		buffer_p->rebuild(
			initial_data,
			H_resource_desc::create_buffer_desc(
				count,
				stride,
				bind_flags,
				heap_type
			)
		);
	}

	void H_buffer::rebuild(
		KPA_valid_buffer_handle buffer_p,
		const F_initial_resource_data& initial_data,
		u32 count,
		ED_format format,
		ED_resource_bind_flag bind_flags,
		ED_resource_heap_type heap_type
	) {
		buffer_p->rebuild(
			initial_data,
			H_resource_desc::create_buffer_desc(
				count,
				format,
				bind_flags,
				heap_type
			)
		);
	}

	void H_buffer::rebuild_structured(
		KPA_valid_structured_buffer_handle structured_buffer_p,
		const F_initial_resource_data& initial_data,
		u32 count,
		u32 stride,
		ED_resource_bind_flag bind_flags,
		ED_resource_heap_type heap_type
	) {
		structured_buffer_p->rebuild(
			initial_data,
			H_resource_desc::create_structured_buffer_desc(
				count,
				stride,
				bind_flags,
				heap_type
			)
		);
	}

	void H_buffer::rebuild_indirect(
		KPA_valid_indirect_buffer_handle indirect_buffer_p,
		const F_initial_resource_data& initial_data,
		u32 count,
		ED_resource_bind_flag bind_flags,
		ED_resource_heap_type heap_type
	) {
		indirect_buffer_p->rebuild(
			initial_data,
			H_resource_desc::create_indirect_buffer_desc(
				count,
				bind_flags,
				heap_type
			)
		);
	}

}