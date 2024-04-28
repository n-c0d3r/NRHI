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
#include <nrhi/buffer_handle.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;



    class NRHI_API H_buffer {

    public:
        static U_buffer_handle create(
            TKPA_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            u32 count,
            u32 stride,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F_element__>
        static U_buffer_handle T_create(
            TKPA_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            u32 count,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        ) {

            return create(
                device_p,
                initial_data,
                count,
                sizeof(F_element__),
                bind_flags,
                heap_type
            );
        }

    public:
        template<typename F_element__>
        static U_buffer_handle T_create(
            TKPA_valid<A_device> device_p,
            const TG_span<F_element__>& data,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        ) {

            return create(
                device_p,
                { .system_mem_p = (void*)data.data() },
				(u32)(data.size()),
				(u32)(sizeof(F_element__)),
                bind_flags,
                heap_type
            );
        }

    public:
        static U_buffer_handle create(
            TKPA_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            u32 count,
            E_format format,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F_element__>
        static U_buffer_handle T_create(
            TKPA_valid<A_device> device_p,
            const TG_span<F_element__>& data,
            E_format format,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        ) {

            return create(
                device_p,
                { .system_mem_p = (void*)data.data() },
				(u32)(data.size()),
                format,
                bind_flags,
                heap_type
            );
        }

    public:
        static U_structured_buffer_handle create_structured(
            TKPA_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            u32 count,
            u32 stride,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F_element__>
        static U_structured_buffer_handle T_create_structured(
            TKPA_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            u32 count,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        ) {

            return create_structured(
                device_p,
                initial_data,
                count,
                sizeof(F_element__),
                bind_flags,
                heap_type
            );
        }

    public:
        template<typename F_element__>
        static U_structured_buffer_handle T_create_structured(
            TKPA_valid<A_device> device_p,
            const TG_span<F_element__>& data,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        ) {

            return create_structured(
                device_p,
                { .system_mem_p = (void*)data.data() },
				(u32)(data.size()),
				(u32)(sizeof(F_element__)),
                bind_flags,
                heap_type
            );
        }

    public:
        static U_single_elemented_buffer_handle create_single_elemented(
            TKPA_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            u32 width,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F__>
        static U_single_elemented_buffer_handle T_create_single_elemented(
            TKPA_valid<A_device> device_p,
            F_initial_resource_data initial_data,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        ) {

            return create_single_elemented(
                device_p,
                initial_data,
                sizeof(F__),
                bind_flags,
                heap_type
            );
        }

    public:
        template<typename F__>
        static U_single_elemented_buffer_handle T_create_single_elemented(
            TKPA_valid<A_device> device_p,
            const F__* data,
            E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
            E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
        ) {

            return create_single_elemented(
                device_p,
                { .system_mem_p = (void*)data },
                sizeof(F__),
                bind_flags,
                heap_type
            );
        }

	public:
		static U_indirect_buffer_handle create_indirect(
			TKPA_valid<A_device> device_p,
			F_initial_resource_data initial_data,
			u32 count,
			E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
			E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
		);

	public:
		static U_indirect_buffer_handle create_indirect(
			TKPA_valid<A_device> device_p,
			const TG_span<u32>& data,
			E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE,
			E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE
		) {

			return create_indirect(
				device_p,
				{ .system_mem_p = (void*)(data.data()) },
				(u32)(data.size()),
				bind_flags,
				heap_type
			);
		}

    };

}
