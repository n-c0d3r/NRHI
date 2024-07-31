#include <nrhi/nsl_factory.hpp>
#include <nrhi/driver.hpp>
#include <nrhi/root_signature.hpp>
#include <nrhi/pipeline_state.hpp>
#include <nrhi/graphics_pipeline_state.hpp>
#include <nrhi/compute_pipeline_state.hpp>



namespace nrhi {

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
	TG_vector<TU<A_pipeline_state>> H_nsl_factory::create_pipeline_states(
		TKPA_valid<A_device> device_p,
		const F_nsl_compiled_result& compiled_result
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_BINDING(
			TG_vector<TU<A_pipeline_state>> result;

			const auto& nsl_reflection = compiled_result.reflection;
			const auto& nsl_shader_reflections = nsl_reflection.shaders;
			const auto& nsl_pipeline_state_reflections = nsl_reflection.pipeline_states;

			u32 pipeline_state_count = nsl_pipeline_state_reflections.size();

			const auto& shader_binaries = compiled_result.shader_binaries;

			// create pipeline states
			auto& pipeline_state_p_vectors = result;
			pipeline_state_p_vectors.resize(pipeline_state_count);
			for(u32 i = 0; i < pipeline_state_count; ++i) {

				const auto& nsl_pipeline_state_reflection = nsl_pipeline_state_reflections[i];
				auto& pipeline_state_p = pipeline_state_p_vectors[i];

				const auto& general_pipeline_state_options = nsl_pipeline_state_reflection.options;
				auto pipeline_state_type = nsl_pipeline_state_reflection.type;

				// create graphics pipeline state
				if(pipeline_state_type == ED_pipeline_state_type::GRAPHICS) {

					F_graphics_pipeline_state_options options = general_pipeline_state_options.graphics;

					// setup shader binaries for options
					u32 shader_count = nsl_pipeline_state_reflection.shader_indices.size();
					for(u32 i = 0; i < shader_count; ++i) {

						u32 shader_index = nsl_pipeline_state_reflection.shader_indices[i];
						const auto& shader_binary = shader_binaries[shader_index];
						const auto& nsl_shader_reflection = nsl_shader_reflections[shader_index];

						NRHI_ENUM_SWITCH(
							nsl_shader_reflection.type,
							NRHI_ENUM_CASE(
								ED_shader_type::VERTEX,
								options.shader_binaries.vertex = (F_shader_binary&)shader_binary;
							)
							NRHI_ENUM_CASE(
								ED_shader_type::PIXEL,
								options.shader_binaries.pixel = (F_shader_binary_temp)(F_shader_binary&)shader_binary;
							)
						);
					}

					// create pipeline state object
					pipeline_state_p = H_pipeline_state::create_graphics_pipeline_state(
						device_p,
						options
					).oref;
				}
					// create compute pipeline state
				else if(pipeline_state_type == ED_pipeline_state_type::COMPUTE) {

					F_compute_pipeline_state_options options = general_pipeline_state_options.compute;

					// setup shader binaries for options
					options.shader_binaries.compute = (F_shader_binary&)shader_binaries[
						nsl_pipeline_state_reflection.shader_indices[0]
					];

					// create pipeline state object
					pipeline_state_p = H_pipeline_state::create_compute_pipeline_state(
						device_p,
						options
					).oref;
				}
			}

			return std::move(result);
		);
	}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	TG_vector<TU<A_pipeline_state>> H_nsl_factory::create_pipeline_states_with_root_signature(
		TKPA_valid<A_device> device_p,
		const F_nsl_compiled_result& compiled_result,
		const TG_span<TK_valid<A_root_signature>>& root_signature_p_vector
	) {
		NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
			TG_vector<TU<A_pipeline_state>> result;

			const auto& nsl_reflection = compiled_result.reflection;
			const auto& nsl_shader_reflections = nsl_reflection.shaders;
			const auto& nsl_pipeline_state_reflections = nsl_reflection.pipeline_states;

			u32 pipeline_state_count = nsl_pipeline_state_reflections.size();

			const auto& shader_binaries = compiled_result.shader_binaries;

			// create pipeline states
			auto& pipeline_state_p_vectors = result;
			pipeline_state_p_vectors.resize(pipeline_state_count);
			for(u32 i = 0; i < pipeline_state_count; ++i) {

				const auto& nsl_pipeline_state_reflection = nsl_pipeline_state_reflections[i];
				auto& pipeline_state_p = pipeline_state_p_vectors[i];

				const auto& general_pipeline_state_options = nsl_pipeline_state_reflection.options;
				auto pipeline_state_type = nsl_pipeline_state_reflection.type;

				// create graphics pipeline state
				if(pipeline_state_type == ED_pipeline_state_type::GRAPHICS) {

					F_graphics_pipeline_state_options options = general_pipeline_state_options.graphics;

					// setup shader binaries for options
					u32 shader_count = nsl_pipeline_state_reflection.shader_indices.size();
					for(u32 i = 0; i < shader_count; ++i) {

						u32 shader_index = nsl_pipeline_state_reflection.shader_indices[i];
						const auto& shader_binary = shader_binaries[shader_index];
						const auto& nsl_shader_reflection = nsl_shader_reflections[shader_index];

						NRHI_ENUM_SWITCH(
							nsl_shader_reflection.type,
							NRHI_ENUM_CASE(
								ED_shader_type::VERTEX,
								options.shader_binaries.vertex = (F_shader_binary&)shader_binary;
							)
							NRHI_ENUM_CASE(
								ED_shader_type::PIXEL,
								options.shader_binaries.pixel = (F_shader_binary_temp)(F_shader_binary&)shader_binary;
							)
						);
					}

					// find root signature
					auto root_signature_p = root_signature_p_vector[
						nsl_pipeline_state_reflection.root_signature
					];

					// create pipeline state object
					pipeline_state_p = H_pipeline_state::create_graphics_pipeline_state_with_root_signature(
						device_p,
						options,
						root_signature_p
					).oref;
				}
				// create compute pipeline state
				else if(pipeline_state_type == ED_pipeline_state_type::COMPUTE) {

					F_compute_pipeline_state_options options = general_pipeline_state_options.compute;

					// setup shader binaries for options
					options.shader_binaries.compute = (F_shader_binary&)shader_binaries[
						nsl_pipeline_state_reflection.shader_indices[0]
					];

					// find root signature
					auto root_signature_p = root_signature_p_vector[
						nsl_pipeline_state_reflection.root_signature
					];

					// create pipeline state object
					pipeline_state_p = H_pipeline_state::create_compute_pipeline_state_with_root_signature(
						device_p,
						options,
						root_signature_p
					).oref;
				}
			}

			return std::move(result);
		);
	}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

}