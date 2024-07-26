#pragma once

/** @file nrhi/pipeline_state_desc.hpp
*
*   Implement pipeline_state desc.
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

#include <nrhi/pipeline_state_type.hpp>
#include <nrhi/format.hpp>
#include <nrhi/frame_buffer_desc.hpp>
#include <nrhi/cull_mode.hpp>
#include <nrhi/fill_mode.hpp>
#include <nrhi/depth_comparison_func.hpp>
#include <nrhi/primitive_topology.hpp>
#include <nrhi/shader_desc.hpp>
#include <nrhi/blend_factor.hpp>
#include <nrhi/blend_operation.hpp>
#include <nrhi/color_write_mode.hpp>
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#include <nrhi/logic_operation.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#include <nrhi/utilities/platform_object_pool.hpp>

#pragma endregion



namespace nrhi {

	class A_device;
	class A_pipeline_state;
	class A_frame_buffer;
	class A_shader;
	class A_root_signature;



	struct F_rasterizer_desc {

		ED_cull_mode cull_mode = ED_cull_mode::BACK;
		ED_fill_mode fill_mode = ED_fill_mode::SOLID;

		b8 front_counter_clock_wise = false;

		NCPP_FORCE_INLINE b8 operator == (const F_rasterizer_desc& b) const noexcept {

			return (
				(cull_mode == b.cull_mode)
				&& (fill_mode == b.fill_mode)
				&& (front_counter_clock_wise == b.front_counter_clock_wise)
			);
		}

	};

	struct F_depth_stencil_desc {

		b8 enable_depth_test = false;
		ED_format format = ED_format::D32_FLOAT;
		ED_depth_comparison_func depth_comparison_func = ED_depth_comparison_func::LESS;
		b8 depth_buffer_write = true;

		NCPP_FORCE_INLINE b8 operator == (const F_depth_stencil_desc& b) const noexcept {

			return (
				(enable_depth_test == b.enable_depth_test)
				&& (format == b.format)
				&& (depth_comparison_func == b.depth_comparison_func)
				&& (depth_buffer_write == b.depth_buffer_write)
			);
		}

	};

	struct F_blend_render_target_desc {

		b8 enable_blend = false;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		b8 enable_logic_operation = false;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

		ED_blend_factor src_blend_factor;
		ED_blend_factor dst_blend_factor;
		ED_blend_operation blend_operation;

		ED_blend_factor src_alpha_blend_factor;
		ED_blend_factor dst_alpha_blend_factor;
		ED_blend_operation alpha_blend_operation;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		ED_logic_operation logic_operation;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

		ED_color_write_mode write_mode;

		NCPP_FORCE_INLINE b8 operator == (const F_blend_render_target_desc& b) const noexcept {

			return (
				(enable_blend == b.enable_blend)

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
				&& (enable_logic_operation == b.enable_logic_operation)
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

				&& (src_blend_factor == b.src_blend_factor)
				&& (dst_blend_factor == b.dst_blend_factor)
				&& (blend_operation == b.blend_operation)

				&& (src_alpha_blend_factor == b.src_alpha_blend_factor)
				&& (dst_alpha_blend_factor == b.dst_alpha_blend_factor)
				&& (alpha_blend_operation == b.alpha_blend_operation)

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
				&& (logic_operation == b.logic_operation)
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

				&& (write_mode == b.write_mode)
			);
		}

	};
	struct F_blend_desc {

		b8 enable_alpha_to_coverage = false;
		b8 enable_independent_blend = false;

		TG_array<F_blend_render_target_desc, 8> render_targets;

		NCPP_FORCE_INLINE b8 operator == (const F_blend_desc& b) const noexcept {

			if (
				(enable_alpha_to_coverage != b.enable_alpha_to_coverage)
				|| (enable_independent_blend != b.enable_independent_blend)
			)
				return false;

			for(u32 i = 0; i < 8; ++i)
				if(render_targets[i] != b.render_targets[i])
					return false;

			return true;
		}

	};

	struct F_pipeline_state_desc {

		ED_pipeline_state_type type = ED_pipeline_state_type::NONE;

		TG_vector<ED_format> color_formats = {
			ED_format::R8G8B8A8_UNORM
		};
		F_depth_stencil_desc depth_stencil_desc;

		F_rasterizer_desc rasterizer_desc;

		F_blend_desc blend_desc;

		ED_primitive_topology primitive_topology = ED_primitive_topology::TRIANGLE_LIST;

		TG_vector<TK_valid<A_shader>> shader_p_vector;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		TG_vector<F_shader_desc> direct_shader_descs;
		TK<A_root_signature> root_signature_p;
#endif // NRHI_SUPPORT_DRIVER_ADVANCED_RESOURCE_BINDING

	};

	class NRHI_API H_pipeline_state_desc {
	};

}
