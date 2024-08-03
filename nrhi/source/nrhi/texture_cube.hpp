#pragma once

/** @file nrhi/texture_cube.hpp
*
*   Implement texture cube.
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

#include <nrhi/texture_cube_face.hpp>
#include <nrhi/texture_2d_array.hpp>

#pragma endregion



namespace nrhi
{

	class NRHI_API H_texture_cube {

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
	public:
		static NCPP_FORCE_INLINE U_srv_handle create_face_srv(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			ED_texture_cube_face face,
			u32 base_mip_level = 0,
			ED_resource_flag resource_flags = ED_resource_flag::NONE
		) {
			return H_texture_2d_array::create_sub_array_srv(
				texture_2d_array_p,
				(u32)face,
				1,
				base_mip_level,
				resource_flags | ED_resource_flag::TEXTURE_CUBE
			);
		}
		static NCPP_FORCE_INLINE U_uav_handle create_face_uav(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			ED_texture_cube_face face,
			u32 base_mip_level = 0,
			ED_resource_flag resource_flags = ED_resource_flag::NONE
		) {
			return H_texture_2d_array::create_sub_array_uav(
				texture_2d_array_p,
				(u32)face,
				1,
				base_mip_level,
				resource_flags | ED_resource_flag::TEXTURE_CUBE
			);
		}
		static NCPP_FORCE_INLINE U_rtv_handle create_element_rtv(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			ED_texture_cube_face face,
			u32 target_mip_level = 0,
			ED_resource_flag resource_flags = ED_resource_flag::NONE
		) {
			return H_texture_2d_array::create_element_rtv(
				texture_2d_array_p,
				(u32)face,
				target_mip_level,
				resource_flags | ED_resource_flag::TEXTURE_CUBE
			);
		}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	public:
		static NCPP_FORCE_INLINE void initialize_face_srv_with_descriptor_cpu_address(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			TKPA_valid<A_descriptor_heap> descriptor_heap_p,
			F_descriptor_cpu_address descriptor_cpu_address,
			ED_texture_cube_face face,
			u32 base_mip_level = 0,
			ED_resource_flag resource_flags = ED_resource_flag::NONE
		) {
			H_texture_2d_array::initialize_sub_array_srv_with_descriptor_cpu_address(
				texture_2d_array_p,
				descriptor_heap_p,
				descriptor_cpu_address,
				(u32)face,
				1,
				base_mip_level,
				resource_flags | ED_resource_flag::TEXTURE_CUBE
			);
		}
		static NCPP_FORCE_INLINE void initialize_face_uav_with_descriptor_cpu_address(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			TKPA_valid<A_descriptor_heap> descriptor_heap_p,
			F_descriptor_cpu_address descriptor_cpu_address,
			ED_texture_cube_face face,
			u32 base_mip_level = 0,
			ED_resource_flag resource_flags = ED_resource_flag::NONE
		) {
			H_texture_2d_array::initialize_sub_array_uav_with_descriptor_cpu_address(
				texture_2d_array_p,
				descriptor_heap_p,
				descriptor_cpu_address,
				(u32)face,
				1,
				base_mip_level,
				resource_flags | ED_resource_flag::TEXTURE_CUBE
			);
		}
		static NCPP_FORCE_INLINE void initialize_face_rtv_with_descriptor_cpu_address(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			TKPA_valid<A_descriptor_heap> descriptor_heap_p,
			F_descriptor_cpu_address descriptor_cpu_address,
			ED_texture_cube_face face,
			u32 target_mip_level = 0,
			ED_resource_flag resource_flags = ED_resource_flag::NONE
		) {
			H_texture_2d_array::initialize_element_rtv_with_descriptor_cpu_address(
				texture_2d_array_p,
				descriptor_heap_p,
				descriptor_cpu_address,
				(u32)face,
				target_mip_level,
				resource_flags | ED_resource_flag::TEXTURE_CUBE
			);
		}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

	};

}