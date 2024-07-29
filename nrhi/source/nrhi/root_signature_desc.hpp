#pragma once

/** @file nrhi/root_signature_desc.hpp
*
*   Implement root_signature_desc.
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

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#include <nrhi/root_param_type.hpp>
#include <nrhi/root_signature_flag.hpp>
#include <nrhi/descriptor_base.hpp>
#include <nrhi/descriptor_range_type.hpp>
#include <nrhi/sampler_state_desc.hpp>
#include <nrhi/shader_visibility.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

#pragma endregion



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
namespace nrhi {

	static constexpr u32 offset_in_descriptors_from_table_start_append = NCPP_U32_MAX;

	struct F_descriptor_range_desc {

		ED_descriptor_range_type type;

		u32 descriptor_count = 0;
		u32 offset_in_descriptors_from_table_start = offset_in_descriptors_from_table_start_append;

		u32 base_register = 0;
		u32 register_space = 0;

	};
	struct F_root_descriptor_table_desc {

		TG_vector<F_descriptor_range_desc> range_descs;

	};

	struct F_root_descriptor_desc {

		u32 base_register = 0;
		u32 register_space = 0;

	};

	struct F_root_constants_desc {

		u32 base_register = 0;
		u32 register_space = 0;
		u32 constant_count = 0;

	};

	struct F_root_param_desc {

		ED_root_param_type type;
		ED_shader_visibility shader_visibility = ED_shader_visibility::ALL;

		// due to use of TG_vector, we can't put descriptor_table_desc into union
		F_root_descriptor_table_desc descriptor_table_desc;

		// these two structs just have primitive data types, so it's ok to use union
		union {
			F_root_descriptor_desc descriptor_desc;
			F_root_constants_desc constants_desc;
		};

		NCPP_FORCE_INLINE F_root_param_desc() noexcept :
			shader_visibility(ED_shader_visibility::ALL),
			descriptor_table_desc(),
			constants_desc()
		{
		}
		NCPP_FORCE_INLINE F_root_param_desc(
			const F_root_descriptor_table_desc& descriptor_table_desc,
			ED_shader_visibility shader_visibility = ED_shader_visibility::ALL
		) noexcept :
			type(ED_root_param_type::DESCRIPTOR_TABLE),
			shader_visibility(shader_visibility),
			descriptor_table_desc(descriptor_table_desc)
		{
		}
		NCPP_FORCE_INLINE F_root_param_desc(
			F_root_descriptor_table_desc&& descriptor_table_desc,
			ED_shader_visibility shader_visibility = ED_shader_visibility::ALL
		) noexcept :
			type(ED_root_param_type::DESCRIPTOR_TABLE),
			shader_visibility(shader_visibility),
			descriptor_table_desc(std::move(descriptor_table_desc))
		{
		}
		NCPP_FORCE_INLINE F_root_param_desc(
			ED_root_param_type type,
			const F_root_descriptor_desc& descriptor_desc,
			ED_shader_visibility shader_visibility = ED_shader_visibility::ALL
		) noexcept :
			type(type),
			shader_visibility(shader_visibility),
			descriptor_desc(descriptor_desc)
		{
		}
		NCPP_FORCE_INLINE F_root_param_desc(
			const F_root_constants_desc& constants_desc,
			ED_shader_visibility shader_visibility = ED_shader_visibility::ALL
		) noexcept :
			type(ED_root_param_type::CONSTANT_32BITS),
			shader_visibility(shader_visibility),
			constants_desc(constants_desc)
		{
		}
		NCPP_FORCE_INLINE F_root_param_desc(const F_root_param_desc& x) noexcept :
			type(x.type),
			shader_visibility(x.shader_visibility),
			descriptor_table_desc(x.descriptor_table_desc),
			constants_desc(x.constants_desc)
		{
		}
		NCPP_FORCE_INLINE F_root_param_desc& operator = (const F_root_param_desc& x) noexcept
		{
			type = x.type;
			shader_visibility = x.shader_visibility;
			descriptor_table_desc = x.descriptor_table_desc;
			constants_desc = x.constants_desc;

			return *this;
		}
		NCPP_FORCE_INLINE F_root_param_desc(F_root_param_desc&& x) noexcept :
			type(x.type),
			shader_visibility(x.shader_visibility),
			descriptor_table_desc(std::move(x.descriptor_table_desc)),
			constants_desc(x.constants_desc)
		{
		}
		NCPP_FORCE_INLINE F_root_param_desc& operator = (F_root_param_desc&& x) noexcept
		{
			type = x.type;
			shader_visibility = x.shader_visibility;
			descriptor_table_desc = std::move(x.descriptor_table_desc);
			constants_desc = x.constants_desc;

			return *this;
		}

	};

	struct F_static_sampler_state_desc {

		u32 base_register = 0;
		u32 register_space = 0;
		F_sampler_state_desc sampler_state_desc;
		ED_shader_visibility shader_visibility = ED_shader_visibility::ALL;

	};

	struct F_root_signature_desc {

		TG_vector<F_root_param_desc> param_descs;
		TG_vector<F_static_sampler_state_desc> static_sampler_state_descs;
		ED_root_signature_flag flags = ED_root_signature_flag::NONE;

	};
}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING