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



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	public:
		static U_buffer_handle create_committed(
			TKPA_valid<A_device> device_p,
			u32 count,
			u32 stride,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
		static U_buffer_handle create_committed(
			TKPA_valid<A_device> device_p,
			u32 count,
			ED_format format,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
		static U_structured_buffer_handle create_committed_structured(
			TKPA_valid<A_device> device_p,
			u32 count,
			u32 stride,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
		static U_indirect_buffer_handle create_committed_indirect(
			TKPA_valid<A_device> device_p,
			u32 count,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		template<typename F_element__>
		static U_buffer_handle T_create_committed(
			TKPA_valid<A_device> device_p,
			u32 count,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			return create_committed(
				device_p,
				count,
				sizeof(F_element__),
				bind_flags,
				heap_type
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}
		template<typename F_element__>
		static U_structured_buffer_handle T_create_committed_structured(
			TKPA_valid<A_device> device_p,
			u32 count,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			return create_committed_structured(
				device_p,
				count,
				sizeof(F_element__),
				bind_flags,
				heap_type
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}



	public:
		static void rebuild_committed(
			KPA_valid_buffer_handle buffer_p,
			u32 count,
			u32 stride,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
		static void rebuild_committed(
			KPA_valid_buffer_handle buffer_p,
			u32 count,
			ED_format format,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
		static void rebuild_committed_structured(
			KPA_valid_structured_buffer_handle structured_buffer_p,
			u32 count,
			u32 stride,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
		static void rebuild_committed_indirect(
			KPA_valid_indirect_buffer_handle indirect_buffer_p,
			u32 count,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		template<typename F_element__>
		static void T_rebuild_committed(
			KPA_valid_buffer_handle buffer_p,
			u32 count,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			rebuild_committed(
				buffer_p,
				count,
				sizeof(F_element__),
				bind_flags,
				heap_type
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}
		template<typename F_element__>
		static void T_rebuild_committed_structured(
			KPA_valid_structured_buffer_handle structured_buffer_p,
			u32 count,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			rebuild_committed_structured(
				structured_buffer_p,
				count,
				sizeof(F_element__),
				bind_flags,
				heap_type
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}

    };
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

}
