#pragma once

/** @file nrhi/texture.hpp
*
*   Implement texture.
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
#include <nrhi/texture_handle.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	class A_resource_heap;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT



    class NRHI_API H_texture {

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT
    public:
        static U_texture_1d_handle create_1d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 width,
            ED_format format = ED_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        );

    public:
        static U_texture_2d_handle create_2d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 width,
            u32 height,
            ED_format format = ED_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            F_sample_desc sample_desc = F_sample_desc{},
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        );

    public:
        static U_texture_3d_handle create_3d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 width,
            u32 height,
            u32 depth,
            ED_format format = ED_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            ED_resource_flag flags = ED_resource_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
        );

	public:
		static U_texture_2d_array_handle create_2d_array(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& initial_data,
			u32 width,
			u32 height,
			u32 array_size,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);



	public:
		static void rebuild_1d(
			KPA_valid_texture_1d_handle texture_1d_p,
			const F_initial_resource_data& initial_data,
			u32 width,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);

	public:
		static void rebuild_2d(
			KPA_valid_texture_2d_handle texture_2d_p,
			const F_initial_resource_data& initial_data,
			u32 width,
			u32 height,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);

	public:
		static void rebuild_3d(
			KPA_valid_texture_3d_handle texture_3d_p,
			const F_initial_resource_data& initial_data,
			u32 width,
			u32 height,
			u32 depth,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);

	public:
		static void rebuild_2d_array(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			const F_initial_resource_data& initial_data,
			u32 width,
			u32 height,
			u32 array_size,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
		);
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	public:
		static U_texture_1d_handle create_committed_1d(
			TKPA_valid<A_device> device_p,
			u32 width,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static U_texture_2d_handle create_committed_2d(
			TKPA_valid<A_device> device_p,
			u32 width,
			u32 height,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static U_texture_3d_handle create_committed_3d(
			TKPA_valid<A_device> device_p,
			u32 width,
			u32 height,
			u32 depth,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static U_texture_2d_array_handle create_committed_2d_array(
			TKPA_valid<A_device> device_p,
			u32 width,
			u32 height,
			u32 array_size,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);



	public:
		static void rebuild_committed_1d(
			KPA_valid_texture_1d_handle texture_1d_p,
			u32 width,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static void rebuild_committed_2d(
			KPA_valid_texture_2d_handle texture_2d_p,
			u32 width,
			u32 height,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static void rebuild_committed_3d(
			KPA_valid_texture_3d_handle texture_3d_p,
			u32 width,
			u32 height,
			u32 depth,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static void rebuild_committed_2d_array(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			u32 width,
			u32 height,
			u32 array_size,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	public:
		static U_texture_1d_handle create_placed_1d(
			TKPA_valid<A_device> device_p,
			u32 width,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static U_texture_2d_handle create_placed_2d(
			TKPA_valid<A_device> device_p,
			u32 width,
			u32 height,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static U_texture_3d_handle create_placed_3d(
			TKPA_valid<A_device> device_p,
			u32 width,
			u32 height,
			u32 depth,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static U_texture_2d_array_handle create_placed_2d_array(
			TKPA_valid<A_device> device_p,
			u32 width,
			u32 height,
			u32 array_size,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);



	public:
		static void rebuild_placed_1d(
			KPA_valid_texture_1d_handle texture_1d_p,
			u32 width,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static void rebuild_placed_2d(
			KPA_valid_texture_2d_handle texture_2d_p,
			u32 width,
			u32 height,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static void rebuild_placed_3d(
			KPA_valid_texture_3d_handle texture_3d_p,
			u32 width,
			u32 height,
			u32 depth,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static void rebuild_placed_2d_array(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			u32 width,
			u32 height,
			u32 array_size,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	public:
		static U_texture_1d_handle create_reserved_1d(
			TKPA_valid<A_device> device_p,
			u32 width,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static U_texture_2d_handle create_reserved_2d(
			TKPA_valid<A_device> device_p,
			u32 width,
			u32 height,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static U_texture_3d_handle create_reserved_3d(
			TKPA_valid<A_device> device_p,
			u32 width,
			u32 height,
			u32 depth,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static U_texture_2d_array_handle create_reserved_2d_array(
			TKPA_valid<A_device> device_p,
			u32 width,
			u32 height,
			u32 array_size,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);



	public:
		static void rebuild_reserved_1d(
			KPA_valid_texture_1d_handle texture_1d_p,
			u32 width,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static void rebuild_reserved_2d(
			KPA_valid_texture_2d_handle texture_2d_p,
			u32 width,
			u32 height,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static void rebuild_reserved_3d(
			KPA_valid_texture_3d_handle texture_3d_p,
			u32 width,
			u32 height,
			u32 depth,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);

	public:
		static void rebuild_reserved_2d_array(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			u32 width,
			u32 height,
			u32 array_size,
			ED_format format = ED_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_flag flags = ED_resource_flag::NONE

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
			, ED_resource_state initial_state = ED_resource_state::COMMON
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
			, ED_resource_layout layout = ED_resource_layout::UNKNOWN,
			u64 alignment = u64(ED_resource_placement_alignment::DEFAULT),
			const F_resource_clear_value& clear_value = {}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

    };

}
