
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

	//
	F_state_object_builder state_object_builder(ED_state_object_type::EXECUTABLE);

	F_global_root_signature_subobject& global_root_signature_subobject = state_object_builder.add_global_root_signature_subobject();

	F_local_root_signature_subobject& local_root_signature_subobject = state_object_builder.add_local_root_signature_subobject();

	F_state_object_config_subobject& state_object_config_subobject = state_object_builder.add_state_object_config();
	state_object_config_subobject.set_flags(ED_state_object_flag::ALLOW_STATE_OBJECT_ADDITIONS);

	F_library_subobject& library_subobject = state_object_builder.add_library();
	library_subobject.set_binary(nsl_shader_compiled_result.library_binary);

	F_work_graph_subobject& work_graph_subobject = state_object_builder.add_work_graph();
	work_graph_subobject.include_all_available_nodes();
	work_graph_subobject.set_name("demo_work_graph");

	auto state_object1_p = state_object_builder.build(
		NCPP_FOH_VALID(device_p)
	);

	auto state_object2_p = state_object_builder.add_to(
		NCPP_FOH_VALID(state_object1_p)
	);

	F_state_object_properties state_object_properties = NCPP_FOH_VALID(state_object1_p);
	F_work_graph_properties work_graph_properties = NCPP_FOH_VALID(state_object1_p);

	F_program_id work_graph_program_id = state_object_properties.program_id("demo_work_graph");

	auto root_signature_map = H_nsl_factory::make_owned_root_signature_map(
		NCPP_FOH_VALID(device_p),
		nsl_shader_compiled_result
	);

	return 0;
}
