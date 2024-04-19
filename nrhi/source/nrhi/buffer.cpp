#include <nrhi/buffer.hpp>
#include <nrhi/resource.hpp>



namespace nrhi {

    U_buffer_handle H_buffer::create(
        TK_valid<A_device> device_p,
        F_initial_resource_data initial_data,
        u32 count,
        E_format format,
        E_resource_heap_type heap_type,
        E_resource_bind_flag bind_flags
    ) {

        return {
            H_resource::create(
                device_p,
                initial_data,
                H_resource_desc::create_buffer_desc(
                    count,
                    format,
                    heap_type,
                    bind_flags
                )
            )
        };
    }

    U_buffer_handle H_buffer::create_structured(
        TK_valid<A_device> device_p,
        F_initial_resource_data initial_data,
        u32 count,
        u32 stride,
        E_resource_heap_type heap_type,
        E_resource_bind_flag bind_flags
    ) {

        return {
            H_resource::create(
                device_p,
                initial_data,
                H_resource_desc::create_structured_buffer_desc(
                    count,
                    stride,
                    heap_type,
                    bind_flags
                )
            )
        };
    }

    U_buffer_handle H_buffer::create_single_elemented(
        TK_valid<A_device> device_p,
        F_initial_resource_data initial_data,
        u32 width,
        E_resource_heap_type heap_type,
        E_resource_bind_flag bind_flags
    ) {

        return {
            H_resource::create(
                device_p,
                initial_data,
                H_resource_desc::create_single_elemented_buffer_desc(
                    width,
                    heap_type,
                    bind_flags
                )
            )
        };
    }

}