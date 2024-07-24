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
            const F_initial_resource_data& initial_data,
            u32 count,
            u32 stride,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F_element__>
        static U_buffer_handle T_create(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 count,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
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
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        ) {

            return create(
                device_p,
                { { .data_p = (void*)data.data() } },
				(u32)(data.size()),
				(u32)(sizeof(F_element__)),
                bind_flags,
                heap_type
            );
        }

    public:
        static U_buffer_handle create(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 count,
            ED_format format,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F_element__>
        static U_buffer_handle T_create(
            TKPA_valid<A_device> device_p,
            const TG_span<F_element__>& data,
            ED_format format,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        ) {

            return create(
                device_p,
				{ { .data_p = (void*)data.data() } },
				(u32)(data.size()),
                format,
                bind_flags,
                heap_type
            );
        }

    public:
        static U_structured_buffer_handle create_structured(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 count,
            u32 stride,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F_element__>
        static U_structured_buffer_handle T_create_structured(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 count,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
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
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        ) {

            return create_structured(
                device_p,
				{ { .data_p = (void*)data.data() } },
				(u32)(data.size()),
				(u32)(sizeof(F_element__)),
                bind_flags,
                heap_type
            );
        }

	public:
		static U_indirect_buffer_handle create_indirect(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& initial_data,
			u32 count,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);

	public:
		static U_indirect_buffer_handle create_indirect(
			TKPA_valid<A_device> device_p,
			const TG_span<u32>& data,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		) {

			return create_indirect(
				device_p,
				{ { .data_p = (void*)(data.data()) } },
				(u32)(data.size()),
				bind_flags,
				heap_type
			);
		}



	public:
		static void rebuild(
			KPA_valid_buffer_handle buffer_p,
			const F_initial_resource_data& initial_data,
			u32 count,
			u32 stride,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);

	public:
		template<typename F_element__>
		static void T_rebuild(
			KPA_valid_buffer_handle buffer_p,
			const F_initial_resource_data& initial_data,
			u32 count,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		) {

			return rebuild(
				buffer_p,
				initial_data,
				count,
				sizeof(F_element__),
				bind_flags,
				heap_type
			);
		}

	public:
		template<typename F_element__>
		static void T_rebuild(
			KPA_valid_buffer_handle buffer_p,
			const TG_span<F_element__>& data,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		) {

			return rebuild(
				buffer_p,
				{ { .data_p = (void*)data.data() } },
				(u32)(data.size()),
				(u32)(sizeof(F_element__)),
				bind_flags,
				heap_type
			);
		}

	public:
		static void rebuild(
			KPA_valid_buffer_handle buffer_p,
			const F_initial_resource_data& initial_data,
			u32 count,
			ED_format format,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);

	public:
		template<typename F_element__>
		static void T_rebuild(
			KPA_valid_buffer_handle buffer_p,
			const TG_span<F_element__>& data,
			ED_format format,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		) {

			return rebuild(
				buffer_p,
				{ { .data_p = (void*)data.data() } },
				(u32)(data.size()),
				format,
				bind_flags,
				heap_type
			);
		}

	public:
		static void rebuild_structured(
			KPA_valid_structured_buffer_handle structured_buffer_p,
			const F_initial_resource_data& initial_data,
			u32 count,
			u32 stride,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);

	public:
		template<typename F_element__>
		static void T_rebuild_structured(
			KPA_valid_structured_buffer_handle structured_buffer_p,
			const F_initial_resource_data& initial_data,
			u32 count,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		) {

			return rebuild_structured(
				structured_buffer_p,
				initial_data,
				count,
				sizeof(F_element__),
				bind_flags,
				heap_type
			);
		}

	public:
		template<typename F_element__>
		static void T_rebuild_structured(
			KPA_valid_structured_buffer_handle structured_buffer_p,
			const TG_span<F_element__>& data,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		) {

			return rebuild_structured(
				structured_buffer_p,
				{ { .data_p = (void*)data.data() } },
				(u32)(data.size()),
				(u32)(sizeof(F_element__)),
				bind_flags,
				heap_type
			);
		}

	public:
		static void rebuild_indirect(
			KPA_valid_indirect_buffer_handle indirect_buffer_p,
			const F_initial_resource_data& initial_data,
			u32 count,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);

	public:
		static void rebuild_indirect(
			KPA_valid_indirect_buffer_handle indirect_buffer_p,
			const TG_span<u32>& data,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		) {

			return rebuild_indirect(
				indirect_buffer_p,
				{ { .data_p = (void*)(data.data()) } },
				(u32)(data.size()),
				bind_flags,
				heap_type
			);
		}

    };

}
