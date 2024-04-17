#include <nrhi/buffer.hpp>
#include <nrhi/resource.hpp>



namespace nrhi {

    U_buffer_handle H_buffer::create(
        TK_valid<A_device> device_p,
        u32 width,
        u32 stride,
        E_resource_heap_type heap_type,
        E_resource_bind_flag bind_flags
    ) {

        return {
            H_resource::create(
                device_p,
                H_resource_desc::create_buffer_desc(
                    width,
                    stride,
                    heap_type,
                    bind_flags
                )
            )
        };
    }

}