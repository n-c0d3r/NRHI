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



    class NRHI_API H_texture {

    public:
        static U_texture_1d_handle create_1d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 width,
            E_format format = E_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
        );

    public:
        static U_texture_2d_handle create_2d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 width,
            u32 height,
            E_format format = E_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            F_sample_desc sample_desc = F_sample_desc{},
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
        );

    public:
        static U_texture_3d_handle create_3d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            u32 width,
            u32 height,
            u32 depth,
            E_format format = E_format::R8G8B8A8_UNORM,
            u32 mip_level_count = 1,
            ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
            ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
        );

	public:
		static U_texture_2d_array_handle create_2d_array(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& initial_data,
			u32 width,
			u32 height,
			u32 count,
			E_format format = E_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
		);

	public:
		static U_texture_cube_handle create_cube(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& initial_data,
			u32 width,
			E_format format = E_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
		);



	public:
		static void rebuild_1d(
			KPA_valid_texture_1d_handle texture_1d_p,
			const F_initial_resource_data& initial_data,
			u32 width,
			E_format format = E_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
		);

	public:
		static void rebuild_2d(
			KPA_valid_texture_2d_handle texture_2d_p,
			const F_initial_resource_data& initial_data,
			u32 width,
			u32 height,
			E_format format = E_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
		);

	public:
		static void rebuild_3d(
			KPA_valid_texture_3d_handle texture_3d_p,
			const F_initial_resource_data& initial_data,
			u32 width,
			u32 height,
			u32 depth,
			E_format format = E_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
		);

	public:
		static void rebuild_2d_array(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			const F_initial_resource_data& initial_data,
			u32 width,
			u32 height,
			u32 count,
			E_format format = E_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
		);

	public:
		static void rebuild_cube(
			KPA_valid_texture_cube_handle texture_cube_p,
			const F_initial_resource_data& initial_data,
			u32 width,
			E_format format = E_format::R8G8B8A8_UNORM,
			u32 mip_level_count = 1,
			F_sample_desc sample_desc = F_sample_desc{},
			ED_resource_bind_flag bind_flags = ED_resource_bind_flag::NONE,
			ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE,
			b8 is_mip_map_generatable = false
		);

    };

}
