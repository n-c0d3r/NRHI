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
            u32 count,
            E_format format,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        );

    public:
        struct F_create_params {

            TK_valid<A_device> device_p;

            F_initial_resource_data initial_data;

            u32 count;
            E_format format;
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        static NCPP_FORCE_INLINE U_buffer_handle create(
            const F_create_params& params
        ) {

            return create(
                params.device_p,
                params.initial_data,
                params.count,
                params.format,
                params.heap_type,
                params.bind_flags
            );
        }

    public:
        static U_buffer_handle create_structured(
            TK_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            u32 count,
            u32 stride,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        );

    public:
        struct F_create_structured_params {

            TK_valid<A_device> device_p;

            F_initial_resource_data initial_data;

            u32 count;
            u32 stride;
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        static NCPP_FORCE_INLINE U_buffer_handle create_structured(
            const F_create_structured_params& params
        ) {

            return create_structured(
                params.device_p,
                params.initial_data,
                params.count,
                params.stride,
                params.heap_type,
                params.bind_flags
            );
        }

    public:
        template<typename F_element__>
        static U_buffer_handle T_create_structured(
            TK_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            u32 count,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        ) {

            return create_structured(
                device_p,
                initial_data,
                count,
                sizeof(F_element__),
                heap_type,
                bind_flags
            );
        }

    public:
        template<typename F_element__>
        struct TF_create_structured_params {

            TK_valid<A_device> device_p;

            F_initial_resource_data initial_data;

            u32 count;
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        template<typename F_element__>
        static NCPP_FORCE_INLINE U_buffer_handle T_create_structured(
            const TF_create_structured_params<F_element__>& params
        ) {

            return T_create_structured<F_element__>(
                params.device_p,
                params.initial_data,
                params.count,
                params.heap_type,
                params.bind_flags
            );
        }

    public:
        template<typename F_element__>
        static U_buffer_handle T_create_structured(
            TK_valid<A_device> device_p,
            const TG_span<F_element__>& data,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        ) {

            return create_structured(
                device_p,
                { .system_mem_p = (void*)data.data() },
                data.size(),
                sizeof(F_element__),
                heap_type,
                bind_flags
            );
        }

    public:
        template<typename F_element__>
        struct TF_create_structured_params_from_span {

            TK_valid<A_device> device_p;

            TG_span<F_element__> data;

            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        template<typename F_element__>
        static NCPP_FORCE_INLINE U_buffer_handle T_create_structured(
            const TF_create_structured_params_from_span<F_element__>& params
        ) {

            return T_create_structured<F_element__>(
                params.device_p,
                params.data,
                params.heap_type,
                params.bind_flags
            );
        }

    public:
        static U_buffer_handle create_single_elemented(
            TK_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            u32 width,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        );

    public:
        struct F_create_single_elemented_params {

            TK_valid<A_device> device_p;

            F_initial_resource_data initial_data;

            u32 width;
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        static NCPP_FORCE_INLINE U_buffer_handle create_single_elemented(
            const F_create_single_elemented_params& params
        ) {

            return create_single_elemented(
                params.device_p,
                params.initial_data,
                params.width,
                params.heap_type,
                params.bind_flags
            );
        }

    public:
        template<typename F__>
        static U_buffer_handle T_create_single_elemented(
            TK_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        ) {

            return create_single_elemented(
                device_p,
                initial_data,
                sizeof(F__),
                heap_type,
                bind_flags
            );
        }

    public:
        template<typename F__>
        struct TF_create_single_elemented_params {

            TK_valid<A_device> device_p;

            F_initial_resource_data initial_data;

            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        template<typename F__>
        static NCPP_FORCE_INLINE U_buffer_handle T_create_single_elemented(
            const TF_create_single_elemented_params<F__>& params
        ) {

            return T_create_single_elemented<F__>(
                params.device_p,
                params.initial_data,
                sizeof(F__),
                params.heap_type,
                params.bind_flags
            );
        }

    public:
        template<typename F__>
        static U_buffer_handle T_create_single_elemented(
            TK_valid<A_device> device_p,
            const F__* data,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE
        ) {

            return create_single_elemented(
                device_p,
                { .system_mem_p = (void*)data },
                sizeof(F__),
                heap_type,
                bind_flags
            );
        }

    public:
        template<typename F__>
        struct TF_create_single_elemented_params_from_data {

            TK_valid<A_device> device_p;

            const F__* data;

            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;

        };
        template<typename F__>
        static NCPP_FORCE_INLINE U_buffer_handle T_create_single_elemented(
            const TF_create_single_elemented_params_from_data<F__>& params
        ) {

            return T_create_single_elemented<F__>(
                params.device_p,
                params.data,
                params.heap_type,
                params.bind_flags
            );
        }

    };

}
