#pragma once

/** @file nrhi/texture_2d_array.hpp
*
*   Implement texture 2d_array.
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

#include <nrhi/texture.hpp>

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#include <nrhi/descriptor_base.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

#pragma endregion



namespace nrhi
{
	class A_device;



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	class A_descriptor_heap;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

	class NRHI_API H_texture_2d_array {

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
	public:
		static U_srv_handle create_sub_array_srv(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			u32 index,
			u32 array_size,
			u32 base_mip_level = 0,
			ED_resource_flag resource_flags = ED_resource_flag::NONE
		);
		static U_uav_handle create_sub_array_uav(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			u32 index,
			u32 array_size,
			u32 base_mip_level = 0,
			ED_resource_flag resource_flags = ED_resource_flag::NONE
		);
		static U_rtv_handle create_element_rtv(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			u32 index,
			u32 target_mip_level = 0,
			ED_resource_flag resource_flags = ED_resource_flag::NONE
		);
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	public:
		static void initialize_sub_array_srv_with_descriptor_cpu_address(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			F_descriptor_cpu_address descriptor_cpu_address,
			u32 index,
			u32 array_size,
			u32 base_mip_level = 0,
			ED_resource_flag resource_flags = ED_resource_flag::NONE
		);
		static void initialize_sub_array_uav_with_descriptor_cpu_address(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			F_descriptor_cpu_address descriptor_cpu_address,
			u32 index,
			u32 array_size,
			u32 base_mip_level = 0,
			ED_resource_flag resource_flags = ED_resource_flag::NONE
		);
		static void initialize_element_rtv_with_descriptor_cpu_address(
			KPA_valid_texture_2d_array_handle texture_2d_array_p,
			F_descriptor_cpu_address descriptor_cpu_address,
			u32 index,
			u32 target_mip_level = 0,
			ED_resource_flag resource_flags = ED_resource_flag::NONE
		);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

	};

}