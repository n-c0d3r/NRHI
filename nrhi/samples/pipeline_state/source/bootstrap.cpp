
#include <nrhi/.hpp>

using namespace nrhi;



int main() {

	// initialize nrhi and deferredly release it
	nrhi::initialize_system();
	NCPP_DEFER(nrhi::release_system());



	// get adapter and create device
	const auto& adapter_p_vector = H_adapter::adapter_p_vector();
	NCPP_ASSERT(adapter_p_vector.size());

	auto device_p = H_device::create(adapter_p_vector[0]);



	// create shader
	auto compiler_p = TU<F_nsl_shader_compiler>()();
	auto nsl_shader_compiled_result_opt = compiler_p->compile(
		"\n"
		"import(nrhi)\n"
		"\n"
		"semantic POSITION(float3)\n"
		"semantic NORMAL(float3)\n"
		"\n"
		"@input_class(PER_INSTANCE)\n"
		"semantic INSTANCE_DATA(float4x4)\n"
		"\n"
		"@value_type(u32)\n"
		"enum E_primitive_type\n"
		"(\n"
		"	E_PRIMITIVE_TYPE_POINT_LIST(2)\n"
		"	E_PRIMITIVE_TYPE_TRIANGLE_LIST(4)\n"
		")\n"
		"\n"
		"struct F_vs_input\n"
		"(\n"
		"	local_position(POSITION)\n"
		"	local_normal(NORMAL)\n"
		"	instance_data(INSTANCE_DATA)\n"
		")\n"
		"\n"
		"struct F_vs_output\n"
		"(\n"
		"	clip_position(SV_POSITION)\n"
		")\n"
		"\n"
		"input_assembler demo_input_assembler\n"
		"(\n"
		"	@buffer(0)\n"
		"	@offset(0)\n"
		"	POSITION\n"
		"	@buffer(1)\n"
		"	@offset(16)\n"
		"	NORMAL\n"
		"	INSTANCE_DATA\n"
		")\n"
		"\n"
		"@input_assembler(demo_input_assembler)\n"
		"vertex_shader vs_main(\n"
		"	input(F_vs_input)\n"
		"	out output(F_vs_output)\n"
		")\n"
		"{\n"
		"F_vs_output vout;\n"
		"vout.clip_position = f32x4(0, 0, 0, 1);\n"
		"\n"
		"output = vout;\n"
		"}\n"
		"\n"
	);
	NCPP_ASSERT(nsl_shader_compiled_result_opt);

	const auto& nsl_shader_compiled_result = nsl_shader_compiled_result_opt.value();
	const auto& nsl_shader_compiled_result_reflection = nsl_shader_compiled_result.reflection;
	const auto& nsl_shader_reflection = nsl_shader_compiled_result_reflection.shaders[0];
	const auto& nsl_input_assembler_reflection = nsl_shader_compiled_result_reflection.input_assemblers[
		nsl_shader_reflection.input_assembler_index
	];

	auto shader_binary = H_shader_compiler::compile_nsl(
		"demo_shader",
		nsl_shader_compiled_result,
		0
	);

	auto shader_p = H_shader::create_vertex_shader(
		NCPP_FOH_VALID(device_p),
		{
			.name = "demo_shader::vs_main",
			.binary = shader_binary,
			.type = nsl_shader_reflection.type,
			.input_assembler_desc = nsl_input_assembler_reflection.desc
		}
	);

	auto pipeline_state_p = H_graphics_pipeline_state::create(
		NCPP_FOH_VALID(device_p),
		{
			.shader_p_vector = {
				NCPP_AOH_VALID(shader_p)
			}
		}
	);

	return 0;
}
