#pragma once

/** @file nrhi/buffer.hpp
*
*   Implement buffer.
*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#pragma region Includes

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/prerequisites.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/resource_desc.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;



    NCPP_FHANDLE_TEMPLATE(A_resource)
    struct TF_buffer_handle {

        NCPP_FHANDLE_GENERATED_BODY(TF_buffer_handle, A_resource);

    };



    using U_buffer_handle = TF_buffer_handle<TU<A_resource>>;
    using S_buffer_handle = TF_buffer_handle<TS<A_resource>>;
    using K_buffer_handle = TF_buffer_handle<TK<A_resource>>;

    using S_valid_buffer_handle = TF_buffer_handle<TS_valid<A_resource>>;
    using K_valid_buffer_handle = TF_buffer_handle<TK_valid<A_resource>>;



    class H_buffer {

    public:
        static U_buffer_handle create(
            TK_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            u32 width,
            u32 stride,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        );

    public:
        struct F_create_params {

            TK_valid<A_device> device_p;

            F_initial_resource_data initial_data;

            u32 width;
            u32 stride;
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        static NCPP_FORCE_INLINE U_buffer_handle create(
            const F_create_params& params
        ) {

            return create(
                params.device_p,
                params.initial_data,
                params.width,
                params.stride,
                params.heap_type,
                params.bind_flags
            );
        }

    public:
        template<typename F_element__>
        static U_buffer_handle T_create(
            TK_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            u32 count,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        ) {

            return create(
                device_p,
                initial_data,
                sizeof(F_element__) * count,
                sizeof(F_element__),
                heap_type,
                bind_flags
            );
        }

    public:
        template<typename F_element__>
        struct TF_create_params {

            TK_valid<A_device> device_p;

            F_initial_resource_data initial_data;

            u32 count;
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        template<typename F_element__>
        static NCPP_FORCE_INLINE U_buffer_handle T_create(
            const TF_create_params<F_element__>& params
        ) {

            return T_create<F_element__>(
                params.device_p,
                params.initial_data,
                params.count,
                params.heap_type,
                params.bind_flags
            );
        }

    public:
        template<typename F_element__>
        static U_buffer_handle T_create_from_span(
            TK_valid<A_device> device_p,
            const TG_span<F_element__>& data_span,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        ) {

            return create(
                device_p,
                F_initial_resource_data { .system_mem_p = data_span.data() },
                sizeof(F_element__) * data_span.size(),
                sizeof(F_element__),
                heap_type,
                bind_flags
            );
        }

    public:
        template<typename F_element__>
        struct TF_create_from_span_params {

            TK_valid<A_device> device_p;

            TG_span<F_element__> data_span;

            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        template<typename F_element__>
        static NCPP_FORCE_INLINE U_buffer_handle T_create_from_span(
            const TF_create_from_span_params<F_element__>& params
        ) {

            return T_create_from_span<F_element__>(
                params.device_p,
                params.data_span,
                params.heap_type,
                params.bind_flags
            );
        }

    };

}
