
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
	G_string shader_src_content =
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
"@root_signature(MAIN_ROOT_SIGNATURE)\n"
"pipeline_state graphics_pso_main\n"
"(\n"
"	vs_main\n"
")\n"
"\n";

	auto compiler_p = TU<F_nsl_shader_compiler>()();
	auto nsl_shader_compiled_result_opt = compiler_p->compile(
		shader_src_content,
		"",
		H_nsl_output_language::default_as_enum(
			NCPP_FOH_VALID(device_p)
		)
	);
	NCPP_ASSERT(nsl_shader_compiled_result_opt);

	auto& nsl_shader_compiled_result = nsl_shader_compiled_result_opt.value();
	nsl_shader_compiled_result.finalize();

	auto root_signature_p = H_root_signature::create(
		NCPP_FOH_VALID(device_p),
		{
			.flags = ED_root_signature_flag::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
		}
	);

	auto pipeline_state_p = H_nsl_factory::create_pipeline_states_with_root_signature(
		NCPP_FOH_VALID(device_p),
		nsl_shader_compiled_result,
		{
			{
				"MAIN_ROOT_SIGNATURE",
				root_signature_p
			}
		}
	);

	return 0;
}
