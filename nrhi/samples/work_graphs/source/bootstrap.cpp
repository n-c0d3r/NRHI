
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
"@allow_state_object_additions\n"
"state_object_config\n"
"\n"
"global_root_signature GRS()\n"
"local_root_signature LRS()\n"
"\n"
"struct F_input_record(\n"
"	dispatch_grid(SV_DISPATCH_GRID_3D)\n"
"	___padding_0___(uint)\n"
")\n"
"\n"
"@launch(BROADCASTING)\n"
"@max_dispatch_grid(1 1 1)\n"
"@thread_group_size(1 1 1)\n"
"node_shader demo_node(\n"
"	input(DispatchNodeInputRecord(F_input_record))\n"
")\n"
"{}\n"
"\n"
"@include_all_available_nodes\n"
"work_graph WG()\n"
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
	nsl_shader_compiled_result.finalize_library();

	auto owned_state_object_builder = H_nsl_factory::make_owned_state_object_builder(
		NCPP_FOH_VALID(device_p),
		nsl_shader_compiled_result
	);

	auto state_object_p = owned_state_object_builder.builder.build(
		NCPP_FOH_VALID(device_p)
	);

	return 0;
}
