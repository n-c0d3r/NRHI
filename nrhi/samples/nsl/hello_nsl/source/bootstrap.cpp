
#include <nrhi/.hpp>

using namespace nrhi;



int main() {

	auto compiler_p = TU<F_nsl_shader_compiler>()();
	compiler_p->compile(
"\n"
"vertex_shader vs_main(local_position(POSITION) out clip_position(float4 SV_POSITION))\n"
"{\n"
"vertex_shader vs_main2(){}\n"
"}\n"
"\n",
		E_nsl_output_language::HLSL
	);

	return 0;
}
