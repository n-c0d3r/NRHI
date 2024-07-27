#include <nrhi/pipeline_state_compiler.hpp>
#include <nrhi/shader_compiler.hpp>



namespace nrhi {

	F_graphics_pipeline_state_options H_pipeline_state_compiler::compile_graphics_nsl(
		const F_nsl_compiled_result& nsl_compiled_result,
		F_graphics_pipeline_state_shader_binaries& shader_binaries,
		u32 pipeline_state_index
	) {
		const auto& nsl_reflection = nsl_compiled_result.reflection;

		NCPP_ASSERT(pipeline_state_index < nsl_reflection.pipeline_states.size()) << "pipeline state index is out of bound";
		const auto& nsl_pso_reflection = nsl_reflection.pipeline_states[pipeline_state_index];
		u32 shader_count = nsl_pso_reflection.shader_indices.size();

		for(u32 i = 0; i < shader_count; ++i) {

			u32 shader_index = nsl_pso_reflection.shader_indices[i];

			const auto& nsl_shader_reflection = nsl_reflection.shaders[shader_index];

			auto shader_binary = H_shader_compiler::compile_nsl(
				nsl_pso_reflection.name,
				nsl_compiled_result,
				shader_index
			);

			NRHI_ENUM_SWITCH(
				nsl_shader_reflection.type,
				NRHI_ENUM_CASE(
					ED_shader_type::VERTEX,
					shader_binaries.vertex = shader_binary;
				)
				NRHI_ENUM_CASE(
					ED_shader_type::PIXEL,
					shader_binaries.pixel = shader_binary;
				)
			);
		}

		F_graphics_pipeline_state_options result = nsl_pso_reflection.options.graphics;
		result.shader_binaries = shader_binaries;

		return std::move(result);
	}
	F_compute_pipeline_state_options H_pipeline_state_compiler::compile_compute_nsl(
		const F_nsl_compiled_result& nsl_compiled_result,
		F_compute_pipeline_state_shader_binaries& shader_binaries,
		u32 pipeline_state_index
	) {
		const auto& nsl_reflection = nsl_compiled_result.reflection;

		NCPP_ASSERT(pipeline_state_index < nsl_reflection.pipeline_states.size()) << "pipeline state index is out of bound";
		const auto& nsl_pso_reflection = nsl_reflection.pipeline_states[pipeline_state_index];
		u32 shader_count = nsl_pso_reflection.shader_indices.size();

		for(u32 i = 0; i < shader_count; ++i) {

			u32 shader_index = nsl_pso_reflection.shader_indices[i];

			const auto& nsl_shader_reflection = nsl_reflection.shaders[shader_index];

			auto shader_binary = H_shader_compiler::compile_nsl(
				nsl_pso_reflection.name,
				nsl_compiled_result,
				shader_index
			);

			NRHI_ENUM_SWITCH(
				nsl_shader_reflection.type,
				NRHI_ENUM_CASE(
					ED_shader_type::COMPUTE,
					shader_binaries.compute = shader_binary;
				)
			);
		}

		F_compute_pipeline_state_options result = nsl_pso_reflection.options.compute;
		result.shader_binaries = shader_binaries;

		return std::move(result);
	}

}