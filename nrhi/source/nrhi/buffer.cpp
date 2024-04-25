#include <nrhi/buffer.hpp>
#include <nrhi/resource.hpp>



namespace nrhi {

    U_buffer_handle H_buffer::create(
        TKPA_valid<A_device> device_p,
        F_initial_resource_data initial_data,
        u32 count,
        u32 stride,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type
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
        F_initial_resource_data initial_data,
        u32 count,
        E_format format,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type
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
        F_initial_resource_data initial_data,
        u32 count,
        u32 stride,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type
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

    U_single_elemented_buffer_handle H_buffer::create_single_elemented(
        TKPA_valid<A_device> device_p,
        F_initial_resource_data initial_data,
        u32 width,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type
    ) {

        return H_resource::create_single_elemented_buffer(
            device_p,
            initial_data,
            H_resource_desc::create_single_elemented_buffer_desc(
                width,
                bind_flags,
                heap_type
            )
        );
    }

}