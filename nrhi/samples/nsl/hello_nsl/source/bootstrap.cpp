
#include <nrhi/.hpp>

using namespace nrhi;



int main() {

	auto compiler_p = TU<F_nsl_shader_compiler>()();
	auto compiled_result_opt = compiler_p->compile(
"\n"
"import(nrhi)\n"
"\n"
"semantic POSITION(float3)\n"
"semantic NORMAL(float3)\n"
"\n"
"@input_class(PER_INSTANCE)\n"
"semantic INSTANCE_DATA(float4x4)\n"
"\n"
"@filter(MIN_MAG_MIP_LINEAR)\n"
"@texcoord_address_modes(WRAP WRAP WRAP)\n"
"@lod_offset(0.0)\n"
"@min_lod(0.0)\n"
"@max_lod(1.0e+30)\n"
"sampler_state demo_sampler_state\n"
"\n"
"@slot(0)\n"
"sampler_state demo_sampler_state2\n"
"\n"
"sampler_state demo_sampler_state3\n"
"\n"
"@slot(1)\n"
"sampler_state demo_sampler_state4\n"
"\n"
"@value_type(u32)\n"
"enum E_primitive_type\n"
"(\n"
"	E_PRIMITIVE_TYPE_POINT_LIST(2)\n"
"	E_PRIMITIVE_TYPE_TRIANGLE_LIST(4)\n"
")\n"
"\n"
"@shaders(vs_main ps_main)\n"
"resource demo_texture_2d\n"
"(\n"
"	Texture2D(float4)\n"
")\n"
"\n"
"resource demo_texture_2d_2\n"
"(\n"
"	Texture2D(float4)\n"
")\n"
"\n"
"resource demo_texture_2d_3\n"
"(\n"
"	Texture2D(float4)\n"
")\n"
"\n"
"resource demo_texture_2d_4\n"
"(\n"
"	Texture2D(float4)\n"
")\n"
"\n"
"resource demo_texture_2d_rw\n"
"(\n"
"	RWTexture2D(float4)\n"
")\n"
"\n"
"resource demo_cb\n"
"(\n"
"	ConstantBuffer\n"
")\n"
"\n"
"default_constant_buffer(demo_cb)\n"
"uniform demo_uniform(float4)\n"
"uniform demo_uniform2(float4)\n"
"uniform demo_uniform3(float4)\n"
"uniform demo_uniform4(float4)\n"
"uniform demo_uniform5(float)\n"
"uniform demo_uniform6(float)\n"
"uniform demo_uniform7(float)\n"
"uniform demo_uniform8(float2)\n"
"uniform demo_uniform9(float2)\n"
"uniform demo_uniform10(float3)\n"
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
"struct F_demo_struct\n"
"(\n"
"	position(float3)\n"
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
"vertex_shader vs_main(\n"
"	input(F_vs_input)\n"
"	out output(F_vs_output)\n"
")\n"
"{\n"
"}\n"
"\n"
"pixel_shader ps_main(\n"
"	clip_position(SV_POSITION)\n"
"	out color(SV_TARGET)\n"
")\n"
"{\n"
"}\n"
"\n"
"@thread_group_size(4 4 2)\n"
"compute_shader cs_main(\n"
")\n"
"{\n"
"}\n"
"\n"
"@color_formats(R8G8B8A8_UNORM)\n"
"default_pipeline_state_options()\n"
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
"@primitive_topology(LINE_LIST)\n"
"pipeline_state graphics_pso_main\n"
"(\n"
"	vs_main\n"
"	ps_main\n"
")\n"
"\n",
		E_nsl_output_language::HLSL_4
	);

	if(compiled_result_opt) {

		auto compiled_result = compiled_result_opt.value();

		compiled_result.reflection.sort_sampler_states(
			{
				TG_unordered_set<G_string>({
					"demo_sampler_state",
					"demo_sampler_state3"
				}),
				TG_unordered_set<G_string>({
					"demo_sampler_state2",
					"demo_sampler_state4"
				})
			}
		);

		compiled_result.reflection.sort_resources(
			{
				TG_unordered_set<G_string>({
					"demo_texture_2d_3",
					"demo_texture_2d"
				}),
				TG_unordered_set<G_string>({
					"demo_texture_2d_4",
					"demo_texture_2d_2"
				})
			},
			E_nsl_resource_type_class::SRV
		);

		NCPP_INFO() << compiled_result.build(
			0
		);
	}

	return 0;
}
