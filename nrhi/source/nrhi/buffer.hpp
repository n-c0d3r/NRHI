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

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	class A_resource_heap;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT



    class NRHI_API H_buffer {

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT
    public:
        static U_buffer_handle create(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 count,
            u32 stride,
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F_element__>
        static U_buffer_handle T_create(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 count,
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        ) {
            return create(
                device_p,
                initial_data,
                count,
                sizeof(F_element__),
                flags,
                heap_type
            );
        }

    public:
        template<typename F_element__>
        static U_buffer_handle T_create(
            TKPA_valid<A_device> device_p,
            const TG_span<F_element__>& data,
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        ) {
            return create(
                device_p,
                { { .data_p = (void*)data.data() } },
				(u32)(data.size()),
				(u32)(sizeof(F_element__)),
                flags,
                heap_type
            );
        }

    public:
        static U_buffer_handle create(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 count,
            ED_format format,
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        );

    public:
        template<typename F_element__>
        static U_buffer_handle T_create(
            TKPA_valid<A_device> device_p,
            const TG_span<F_element__>& data,
            ED_format format,
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        ) {
            return create(
                device_p,
				{ { .data_p = (void*)data.data() } },
				(u32)(data.size()),
                format,
                flags,
                heap_type
            );
        }



	public:
		static void rebuild(
			KPA_valid_buffer_handle buffer_p,
			const F_initial_resource_data& initial_data,
			u32 count,
			u32 stride,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);

	public:
		template<typename F_element__>
		static void T_rebuild(
			KPA_valid_buffer_handle buffer_p,
			const F_initial_resource_data& initial_data,
			u32 count,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		) {

			return rebuild(
				buffer_p,
				initial_data,
				count,
				sizeof(F_element__),
				flags,
				heap_type
			);
		}

	public:
		template<typename F_element__>
		static void T_rebuild(
			KPA_valid_buffer_handle buffer_p,
			const TG_span<F_element__>& data,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		) {

			return rebuild(
				buffer_p,
				{ { .data_p = (void*)data.data() } },
				(u32)(data.size()),
				(u32)(sizeof(F_element__)),
				flags,
				heap_type
			);
		}

	public:
		static void rebuild(
			KPA_valid_buffer_handle buffer_p,
			const F_initial_resource_data& initial_data,
			u32 count,
			ED_format format,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);

	public:
		template<typename F_element__>
		static void T_rebuild(
			KPA_valid_buffer_handle buffer_p,
			const TG_span<F_element__>& data,
			ED_format format,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		) {

			return rebuild(
				buffer_p,
				{ { .data_p = (void*)data.data() } },
				(u32)(data.size()),
				format,
				flags,
				heap_type
			);
		}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	public:
		static U_buffer_handle create_committed(
			TKPA_valid<A_device> device_p,
			u32 count,
			u32 stride,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
		static U_buffer_handle create_committed(
			TKPA_valid<A_device> device_p,
			u32 count,
			ED_format format,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		template<typename F_element__>
		static U_buffer_handle T_create_committed(
			TKPA_valid<A_device> device_p,
			u32 count,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			return create_committed(
				device_p,
				count,
				sizeof(F_element__),
				flags,
				heap_type
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}
		template<typename F_element__>
		static U_buffer_handle T_create_committed(
			TKPA_valid<A_device> device_p,
			u32 count,
			ED_format format,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			return create_committed(
				device_p,
				count,
				format,
				flags,
				heap_type
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}



	public:
		static void rebuild_committed(
			KPA_valid_buffer_handle buffer_p,
			u32 count,
			u32 stride,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
		static void rebuild_committed(
			KPA_valid_buffer_handle buffer_p,
			u32 count,
			ED_format format,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		template<typename F_element__>
		static void T_rebuild_committed(
			KPA_valid_buffer_handle buffer_p,
			u32 count,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			rebuild_committed(
				buffer_p,
				count,
				sizeof(F_element__),
				flags,
				heap_type
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	public:
		static U_buffer_handle create_placed(
			TKPA_valid<A_device> device_p,
			u32 count,
			u32 stride,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
		static U_buffer_handle create_placed(
			TKPA_valid<A_device> device_p,
			u32 count,
			ED_format format,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		template<typename F_element__>
		static U_buffer_handle T_create_placed(
			TKPA_valid<A_device> device_p,
			u32 count,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			return create_placed(
				device_p,
				count,
				sizeof(F_element__),
				heap_p,
				heap_offset,
				flags
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}
		template<typename F_element__>
		static U_buffer_handle T_create_placed(
			TKPA_valid<A_device> device_p,
			u32 count,
			ED_format format,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			return create_placed(
				device_p,
				count,
				format,
				heap_p,
				heap_offset,
				flags
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}



	public:
		static void rebuild_placed(
			KPA_valid_buffer_handle buffer_p,
			u32 count,
			u32 stride,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
		static void rebuild_placed(
			KPA_valid_buffer_handle buffer_p,
			u32 count,
			ED_format format,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		template<typename F_element__>
		static void T_rebuild_placed(
			KPA_valid_buffer_handle buffer_p,
			u32 count,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			rebuild_placed(
				buffer_p,
				count,
				sizeof(F_element__),
				heap_p,
				heap_offset,
				flags
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	public:
		static U_buffer_handle create_reserved(
			TKPA_valid<A_device> device_p,
			u32 count,
			u32 stride,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
		static U_buffer_handle create_reserved(
			TKPA_valid<A_device> device_p,
			u32 count,
			ED_format format,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		template<typename F_element__>
		static U_buffer_handle T_create_reserved(
			TKPA_valid<A_device> device_p,
			u32 count,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			return create_reserved(
				device_p,
				count,
				sizeof(F_element__),
				flags
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}
		template<typename F_element__>
		static U_buffer_handle T_create_reserved(
			TKPA_valid<A_device> device_p,
			u32 count,
			ED_format format,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			return create_reserved(
				device_p,
				count,
				format,
				flags
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}



	public:
		static void rebuild_reserved(
			KPA_valid_buffer_handle buffer_p,
			u32 count,
			u32 stride,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
		static void rebuild_reserved(
			KPA_valid_buffer_handle buffer_p,
			u32 count,
			ED_format format,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		template<typename F_element__>
		static void T_rebuild_reserved(
			KPA_valid_buffer_handle buffer_p,
			u32 count,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::ROW_MAJOR,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		) {
			rebuild_reserved(
				buffer_p,
				count,
				sizeof(F_element__),
				flags
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
				, initial_state
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
				, layout,
				alignment,
				clear_value
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			);
		}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

	};

}
