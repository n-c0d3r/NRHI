
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
"struct F_demo_node_input(\n"
"	//dispatch_grid(SV_DISPATCH_GRID_3D)\n"
")\n"
"\n"
"@launch(BROADCASTING)\n"
"@max_dispatch_grid(16 1 1)\n"
"@thread_group_size(16 1 1)\n"
"node_shader demo_node(\n"
"	input(DispatchNodeInputRecord(F_demo_node_input))\n"
")\n"
"{\n"
"}\n"
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

	//
	F_state_object_builder state_object_builder(ED_state_object_type::EXECUTABLE);

	F_work_graph_subobject& work_graph_subobject = state_object_builder.add_work_graph();
	work_graph_subobject.include_all_available_nodes();
#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	work_graph_subobject.set_debug_name("demo_work_graph");
#endif

	F_library_subobject& library_subobject = state_object_builder.add_library();
	library_subobject.set_binary({});

	auto root_signature_map = H_nsl_factory::make_owned_root_signature_map(
		NCPP_FOH_VALID(device_p),
		nsl_shader_compiled_result
	);

	auto pipeline_state_p_vector = H_nsl_factory::create_pipeline_states_with_root_signature(
		NCPP_FOH_VALID(device_p),
		nsl_shader_compiled_result,
		root_signature_map.map
	);

	return 0;
}
