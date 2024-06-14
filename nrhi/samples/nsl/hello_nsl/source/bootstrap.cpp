
#include <nrhi/.hpp>

using namespace nrhi;



int main() {

	auto compiler_p = TU<F_nsl_shader_compiler>()();
	compiler_p->compile(
"\n"
"semantic POSITION(float3)\n"
"semantic NORMAL(float3)\n"
"\n"
"@input_class(PER_INSTANCE)\n"
"semantic INSTANCE_DATA(float4x4)\n"
"\n"
"vertex_shader vs_main(\n"
"	@buffer(0)\n"
"	local_position(POSITION)\n"
"	@buffer(1)\n"
"	@count(4)\n"
"	local_normal(NORMAL)\n"
"	out clip_position(SV_POSITION)\n"
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
"\n",
		E_nsl_output_language::HLSL
	);

	return 0;
}
