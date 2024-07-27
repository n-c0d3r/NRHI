
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
		"vertex_shader vs_main(\n"
		"	input(F_vs_input)\n"
		"	out output(F_vs_output)\n"
		")\n"
		"{\n"
		"	F_vs_output vout;\n"
		"	vout.clip_position = f32x4(0, 0, 0, 1);\n"
		"	\n"
		"	output = vout;\n"
		"}\n"
		"\n"
		"@color_formats(R8G8B8A8_UNORM)\n"
		"default_pipeline_state()\n"
		"\n"
		"@rasterizer\n"
		"(\n"
		"	cull_mode(BACK)\n"
		"	fill_mode(WIREFRAME)\n"
		")\n"
		"@input_assembler\n"
		"(\n"
		"	@buffer(0)\n"
		"	@offset(0)\n"
		"	POSITION\n"
		"	@buffer(1)\n"
		"	@offset(16)\n"
		"	NORMAL\n"
		"	INSTANCE_DATA\n"
		")\n"
		"@primitive_topology(TRIANGLE_LIST)\n"
		"pipeline_state graphics_pso_main\n"
		"(\n"
		"	vs_main\n"
		")\n"
		"\n"
	);
	NCPP_ASSERT(nsl_shader_compiled_result_opt);

	const auto& nsl_shader_compiled_result = nsl_shader_compiled_result_opt.value();

	F_graphics_pipeline_state_shader_binaries shader_binaries;

	auto pipeline_state_p = H_graphics_pipeline_state::create(
		NCPP_FOH_VALID(device_p),
		H_pipeline_state_compiler::compile_graphics_nsl(
			nsl_shader_compiled_result,
			shader_binaries,
			0
		)
	);

	return 0;
}
