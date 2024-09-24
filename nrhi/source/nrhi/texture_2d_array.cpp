#include <nrhi/texture_2d_array.hpp>
#include <nrhi/resource.hpp>
#include <nrhi/resource_view.hpp>

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#include <nrhi/descriptor.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING



namespace nrhi {

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
	U_srv_handle H_texture_2d_array::create_sub_array_srv(
		KPA_valid_texture_2d_array_handle texture_2d_array_p,
		u32 index,
		u32 array_size,
		u32 base_mip_level,
		ED_resource_flag resource_flags
	) {
		return H_resource_view::create_srv(
			texture_2d_array_p->device_p(),
			{
				.overrided_resource_type = ED_resource_type::TEXTURE_2D_ARRAY,
				.overrided_resource_flags = resource_flags,
				.resource_p = texture_2d_array_p.oref.no_requirements(),
				.base_mip_level = base_mip_level,
				.index = index,
				.overrided_array_size = array_size
			}
		);
	}
	U_uav_handle H_texture_2d_array::create_sub_array_uav(
		KPA_valid_texture_2d_array_handle texture_2d_array_p,
		u32 index,
		u32 array_size,
		u32 base_mip_level,
		ED_resource_flag resource_flags
	) {
		return H_resource_view::create_uav(
			texture_2d_array_p->device_p(),
			{
				.overrided_resource_type = ED_resource_type::TEXTURE_2D_ARRAY,
				.overrided_resource_flags = resource_flags,
				.resource_p = texture_2d_array_p.oref.no_requirements(),
				.base_mip_level = base_mip_level,
				.index = index,
				.overrided_array_size = array_size
			}
		);
	}
	U_rtv_handle H_texture_2d_array::create_element_rtv(
		KPA_valid_texture_2d_array_handle texture_2d_array_p,
		u32 index,
		u32 target_mip_level,
		ED_resource_flag resource_flags
	) {
		return H_resource_view::create_rtv(
			texture_2d_array_p->device_p(),
			{
				.overrided_resource_type = ED_resource_type::TEXTURE_2D_ARRAY,
				.overrided_resource_flags = resource_flags,
				.resource_p = texture_2d_array_p.oref.no_requirements(),
				.base_mip_level = target_mip_level,
				.index = index,
				.overrided_array_size = 1
			}
		);
	}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	void H_texture_2d_array::initialize_sub_array_srv_with_descriptor_cpu_address(
		KPA_valid_texture_2d_array_handle texture_2d_array_p,
		F_descriptor_cpu_address descriptor_cpu_address,
		u32 index,
		u32 array_size,
		u32 base_mip_level,
		ED_resource_flag resource_flags
	) {
		H_descriptor::initialize_srv(
			texture_2d_array_p->device_p(),
			descriptor_cpu_address,
			{
				.overrided_resource_type = ED_resource_type::TEXTURE_2D_ARRAY,
				.overrided_resource_flags = resource_flags,
				.resource_p = texture_2d_array_p.oref.no_requirements(),
				.base_mip_level = base_mip_level,
				.index = index,
				.overrided_array_size = array_size
			}
		);
	}
	void H_texture_2d_array::initialize_sub_array_uav_with_descriptor_cpu_address(
		KPA_valid_texture_2d_array_handle texture_2d_array_p,
		F_descriptor_cpu_address descriptor_cpu_address,
		u32 index,
		u32 array_size,
		u32 base_mip_level,
		ED_resource_flag resource_flags
	) {
		H_descriptor::initialize_uav(
			texture_2d_array_p->device_p(),
			descriptor_cpu_address,
			{
				.overrided_resource_type = ED_resource_type::TEXTURE_2D_ARRAY,
				.overrided_resource_flags = resource_flags,
				.resource_p = texture_2d_array_p.oref.no_requirements(),
				.base_mip_level = base_mip_level,
				.index = index,
				.overrided_array_size = array_size
			}
		);
	}
	void H_texture_2d_array::initialize_element_rtv_with_descriptor_cpu_address(
		KPA_valid_texture_2d_array_handle texture_2d_array_p,
		F_descriptor_cpu_address descriptor_cpu_address,
		u32 index,
		u32 target_mip_level,
		ED_resource_flag resource_flags
	) {
		H_descriptor::initialize_rtv(
			texture_2d_array_p->device_p(),
			descriptor_cpu_address,
			{
				.overrided_resource_type = ED_resource_type::TEXTURE_2D_ARRAY,
				.overrided_resource_flags = resource_flags,
				.resource_p = texture_2d_array_p.oref.no_requirements(),
				.base_mip_level = target_mip_level,
				.index = index,
				.overrided_array_size = 1
			}
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

}