
#include <nrhi/.hpp>

using namespace nrhi;



template<typename F_element__>
auto T_create_buffer(
	TKPA_valid<A_device> device_p,
	const TG_span<F_element__>& elements,
	TKPA_valid<A_command_list> copy_command_list_p,
	TG_vector<TU<A_resource>>& intermediate_resource_p_vector,
	ED_resource_flag flag = ED_resource_flag::NONE,
	ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
) {
	// create buffers
	auto result_p = H_buffer::T_create_committed<F_element__>(
		NCPP_FOH_VALID(device_p),
		elements.size(),
		flag,
		heap_type
	);
	auto intermediate_p = H_buffer::T_create_committed<F_element__>(
		NCPP_FOH_VALID(device_p),
		elements.size(),
		ED_resource_flag::NONE,
		ED_resource_heap_type::GREAD_CWRITE,
		ED_resource_state::_GENERIC_READ
	);

	// upload data to intermediate buffer
	auto mapped_resource = intermediate_p->map(0);
	memcpy(
		mapped_resource.data(),
		elements.data(),
		elements.size() * sizeof(F_element__)
	);
	intermediate_p->unmap(0);

	// copy intermediate buffer to result buffer
	copy_command_list_p->async_copy_resource(
		NCPP_FOH_VALID(result_p),
		NCPP_FOH_VALID(intermediate_p)
	);

	// push + move intermediate resource to intermediate resource pointer vectors
	intermediate_resource_p_vector.push_back(
		std::move(intermediate_p.oref)
	);

	// return result buffer
	return std::move(result_p);
}

template<typename F_element__>
auto T_create_buffer(
	TKPA_valid<A_device> device_p,
	const TG_span<F_element__>& elements,
	ED_format format,
	TKPA_valid<A_command_list> copy_command_list_p,
	TG_vector<TU<A_resource>>& intermediate_resource_p_vector,
	ED_resource_flag flag = ED_resource_flag::NONE,
	ED_resource_heap_type heap_type = ED_resource_heap_type::GREAD_GWRITE
) {
	// create buffers
	auto result_p = H_buffer::T_create_committed<F_element__>(
		NCPP_FOH_VALID(device_p),
		elements.size(),
		format,
		flag,
		heap_type
	);
	auto intermediate_p = H_buffer::T_create_committed<F_element__>(
		NCPP_FOH_VALID(device_p),
		elements.size(),
		format,
		ED_resource_flag::NONE,
		ED_resource_heap_type::GREAD_CWRITE,
		ED_resource_state::_GENERIC_READ
	);

	// upload data to intermediate buffer
	auto mapped_resource = intermediate_p->map(0);
	memcpy(
		mapped_resource.data(),
		elements.data(),
		elements.size() * sizeof(F_element__)
	);
	intermediate_p->unmap(0);

	// copy intermediate buffer to result buffer
	copy_command_list_p->async_copy_resource(
		NCPP_FOH_VALID(result_p),
		NCPP_FOH_VALID(intermediate_p)
	);

	// push + move intermediate resource to intermediate resource pointer vectors
	intermediate_resource_p_vector.push_back(
		std::move(intermediate_p.oref)
	);

	// return result buffer
	return std::move(result_p);
}



int main() {

    NCPP_INFO()
        << "Hello "
        << E_log_color::V_FOREGROUND_BRIGHT_MAGNETA
        << "NRHI "
        << E_log_color::V_FOREGROUND_BRIGHT_BLUE
        << NRHI_VERSION_STR
        << E_log_color::RESET;



    // initialize nrhi and deferredly release it
    nrhi::initialize_system();
    NCPP_DEFER(nrhi::release_system());



    // get adapter and create device
    const auto& adapter_p_vector = H_adapter::adapter_p_vector();
    NCPP_ASSERT(adapter_p_vector.size());

    auto device_p = H_device::create(adapter_p_vector[0]);



    // create command queue and command list
    auto command_list_type = ED_command_list_type::DIRECT;

    auto command_queue_p = H_command_queue::create(
        NCPP_FOREF_VALID(device_p),
        F_command_queue_desc {
            command_list_type
        }
    );

	auto command_allocator_p = H_command_allocator::create(
		NCPP_FOREF_VALID(device_p),
		F_command_allocator_desc {
			command_list_type
		}
	);
	auto command_list_p = H_command_list::create_with_command_allocator(
		NCPP_FOREF_VALID(device_p),
		F_command_list_desc {
			command_list_type,
			command_allocator_p.keyed()
		}
	);
	command_list_p->async_end();



	// create fence
	TU<A_fence> fence_p = H_fence::create(
		NCPP_FOREF_VALID(device_p),
		F_fence_desc {}
	);
	u32 frame_index = 0;



	// create copy command queue and copy command list
	auto copy_command_list_type = ED_command_list_type::BLIT;

	auto copy_command_queue_p = H_command_queue::create(
		NCPP_FOREF_VALID(device_p),
		F_command_queue_desc {
			command_list_type
		}
	);

	auto copy_command_allocator_p = H_command_allocator::create(
		NCPP_FOREF_VALID(device_p),
		F_command_allocator_desc {
			command_list_type
		}
	);
	auto copy_command_list_p = H_command_list::create_with_command_allocator(
		NCPP_FOREF_VALID(device_p),
		F_command_list_desc {
			command_list_type,
			copy_command_allocator_p.keyed()
		}
	);
	copy_command_list_p->async_end();



	// create a vector to store intermediate resources
	TG_vector<TU<A_resource>> intermediate_resource_p_vector;



	// create fence
	TU<A_fence> copy_fence_p = H_fence::create(
		NCPP_FOREF_VALID(device_p),
		F_fence_desc {}
	);
	u32 copy_index = 0;



    // create surface manager and surface
    F_surface_manager surface_manager;
    auto surface_p = surface_manager.create_surface({
        .title = G_wstring(L"Hello NRHI (advanced) ") + TEXT(NRHI_VERSION_STR),
        .size = { 700, 700 }
    });



    //create swapchain
    auto swapchain_p = H_swapchain::create(
        NCPP_FOREF_VALID(command_queue_p),
        surface_p,
        F_swapchain_desc {
        }
    );
    auto back_rtv_p = swapchain_p->back_rtv_p();



    // delete swapchain when surface is destroyed
    surface_p->T_get_event<F_surface_destroy_event>().T_push_back_listener([&](auto& e){

        swapchain_p.reset();
    });



	// begin copy command list for resource uploading
	copy_command_list_p->async_begin(
		NCPP_FOH_VALID(copy_command_allocator_p)
	);



	// create resources
    TG_vector<F_vector4> vertices = {
		{ 0.05f, 0.0f, 0.5f, 1.0f },
		{ 0.0f, 0.0f, 0.5f, 1.0f },
		{ 0.0f, 0.05f, 0.5f, 1.0f },
		{ 0.05f, 0.05f, 0.5f, 1.0f }
	};
    U_buffer_handle vbuffer_p = T_create_buffer<F_vector4>(
        NCPP_FOH_VALID(device_p),
		vertices,
		NCPP_FOH_VALID(copy_command_list_p),
		intermediate_resource_p_vector,
        ED_resource_flag::INPUT_BUFFER
    );

	TG_vector<F_vector4> instances = {
		{ -0.35f, -0.1f, 0.0f, 1.0f },
		{ -0.35f, -0.05f, 0.0f, 1.0f },
		{ -0.35f, 0.00f, 0.0f, 1.0f },
		{ -0.35f, 0.05f, 0.0f, 1.0f },
		{ -0.35f, 0.1f, 0.0f, 1.0f },
		{ -0.35f, 0.15f, 0.0f, 1.0f },

		{ -0.25f, 0.15f, 0.0f, 1.0f },
		{ -0.30f, 0.15f, 0.0f, 1.0f },

		{ -0.20f, -0.1f, 0.0f, 1.0f },
		{ -0.20f, -0.05f, 0.0f, 1.0f },
		{ -0.20f, 0.00f, 0.0f, 1.0f },
		{ -0.20f, 0.05f, 0.0f, 1.0f },
		{ -0.20f, 0.1f, 0.0f, 1.0f },

		{ -0.10f, -0.1f, 0.0f, 1.0f },
		{ -0.10f, -0.05f, 0.0f, 1.0f },
		{ -0.10f, 0.00f, 0.0f, 1.0f },
		{ -0.10f, 0.05f, 0.0f, 1.0f },
		{ -0.10f, 0.1f, 0.0f, 1.0f },
		{ -0.10f, 0.15f, 0.0f, 1.0f },

		{ 0.00f, 0.15f, 0.0f, 1.0f },
		{ -0.05f, 0.10f, 0.0f, 1.0f },

		{ 0.10f, -0.1f, 0.0f, 1.0f },
		{ 0.10f, -0.05f, 0.0f, 1.0f },
		{ 0.10f, 0.00f, 0.0f, 1.0f },
		{ 0.10f, 0.05f, 0.0f, 1.0f },
		{ 0.10f, 0.1f, 0.0f, 1.0f },
		{ 0.10f, 0.15f, 0.0f, 1.0f },

		{ 0.20f, 0.05f, 0.0f, 1.0f },
		{ 0.15f, 0.05f, 0.0f, 1.0f },

		{ 0.25f, -0.1f, 0.0f, 1.0f },
		{ 0.25f, -0.05f, 0.0f, 1.0f },
		{ 0.25f, 0.00f, 0.0f, 1.0f },
		{ 0.25f, 0.05f, 0.0f, 1.0f },
		{ 0.25f, 0.1f, 0.0f, 1.0f },
		{ 0.25f, 0.15f, 0.0f, 1.0f },

		{ 0.35f, -0.1f, 0.0f, 1.0f },
		{ 0.35f, -0.05f, 0.0f, 1.0f },
		{ 0.35f, 0.00f, 0.0f, 1.0f },
		{ 0.35f, 0.05f, 0.0f, 1.0f },
		{ 0.35f, 0.1f, 0.0f, 1.0f },
		{ 0.35f, 0.15f, 0.0f, 1.0f }
	};
	U_buffer_handle instance_buffer_p = T_create_buffer<F_vector4>(
		NCPP_FOH_VALID(device_p),
		instances,
		NCPP_FOH_VALID(copy_command_list_p),
		intermediate_resource_p_vector,
		ED_resource_flag::INPUT_BUFFER
	);

    TG_vector<u32> indices = {
		0,
		1,
		2,
		0,
		2,
		3
	};
    U_buffer_handle ibuffer_p = T_create_buffer<u32>(
		NCPP_FOH_VALID(device_p),
        indices,
        ED_format::R32_UINT,
		NCPP_FOH_VALID(copy_command_list_p),
		intermediate_resource_p_vector,
        ED_resource_flag::INDEX_BUFFER
    );

    F_vector4 output_color = { 0.2f, 0.2f, 0.2f, 1.0f };
    U_buffer_handle cbuffer_p = T_create_buffer<F_vector4>(
		NCPP_FOH_VALID(device_p),
        NCPP_INIL_SPAN(output_color),
		NCPP_FOH_VALID(copy_command_list_p),
		intermediate_resource_p_vector,
        ED_resource_flag::CONSTANT_BUFFER
    );



	// submit copy commands to GPU and wait it done
	copy_command_list_p->async_end();

	++copy_index;
	copy_command_queue_p->async_execute_command_list(
		NCPP_FOH_VALID(copy_command_list_p)
	);
	copy_command_queue_p->async_signal(
		NCPP_FOH_VALID(copy_fence_p),
		copy_index
	);
	copy_fence_p->wait(copy_index);

	//
	copy_command_allocator_p->flush();



	// clear intermediate resources to save memory
	intermediate_resource_p_vector.clear();




	// create and compile pipeline state
	G_string shader_src_content = "\n"
"import(nrhi)\n"
"\n"
"@shaders(ps_main)\n"
"resource cb\n"
"(\n"
"	ConstantBuffer\n"
")\n"
"default_constant_buffer(cb)\n"
"uniform text_color(float4)\n"
"\n"
"semantic VERTEX_POSITION(float4)\n"
"\n"
"semantic INSTANCE_POSITION(float4)\n"
"\n"
"vertex_shader vs_main\n"
"(\n"
"	vertex_position(VERTEX_POSITION)\n"
"	instance_position(INSTANCE_POSITION)\n"
"	out clip_position(SV_POSITION)\n"
")\n"
"{\n"
"	clip_position = float4(instance_position.xyz + vertex_position.xyz, 1);\n"
"}\n"
"pixel_shader ps_main\n"
"(\n"
"	out color(SV_TARGET)\n"
")\n"
"{\n"
"	color = text_color;\n"
"}\n"
"\n"
"@color_formats(R8G8B8A8_UNORM)\n"
"@rasterizer\n"
"(\n"
"	cull_mode(NONE)\n"
"	fill_mode(SOLID)\n"
")\n"
"@input_assembler\n"
"(\n"
"	VERTEX_POSITION\n"
"	@buffer(1)\n"
"	@classification(PER_INSTANCE_DATA)\n"
"	INSTANCE_POSITION\n"
")\n"
"@primitive_topology(TRIANGLE_LIST)\n"
"pipeline_state graphics_pso_main\n"
"(\n"
"	vs_main\n"
"	ps_main\n"
")\n"
"\n";

	auto compiler_p = TU<F_nsl_shader_compiler>()();
	auto nsl_shader_compiled_result_opt = compiler_p->compile(shader_src_content);
	NCPP_ASSERT(nsl_shader_compiled_result_opt);

	auto& nsl_shader_compiled_result = nsl_shader_compiled_result_opt.value();
	nsl_shader_compiled_result.finalize();

	F_root_param_desc cbv_root_param_desc(
		ED_root_param_type::CONSTANT_BUFFER,
		F_root_descriptor_desc {
			0,
			0
		},
		ED_shader_visibility::PIXEL
	);

	auto root_signature_p = H_root_signature::create(
		NCPP_FOH_VALID(device_p),
		{
			.param_descs = {
				cbv_root_param_desc
			},
			.flags = ED_root_signature_flag::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
		}
	);

	auto pipeline_state_p_vector = H_nsl_factory::create_pipeline_states_with_root_signature(
		NCPP_FOH_VALID(device_p),
		nsl_shader_compiled_result,
		NCPP_INIL_SPAN(
			NCPP_FOH_VALID(root_signature_p)
		)
	);

	U_graphics_pipeline_state_handle pipeline_state_p = {
		std::move(pipeline_state_p_vector[0])
	};

	// create frame buffer
	auto frame_buffer_p = H_frame_buffer::create(
		NCPP_FOREF_VALID(device_p),
		{
			.color_attachments = {
				swapchain_p->back_rtv_p()
			}
		}
	);

	// release some objects when surface destroyed
	surface_p->T_get_event<F_surface_destroy_event>().T_push_back_listener([&](auto& e){

		frame_buffer_p.reset();
		swapchain_p.reset();
	});

	// time counting variables
	f32 delta_time = 0.0f;
	auto start_time = std::chrono::high_resolution_clock::now();

    // run app
    surface_manager.T_run([&](F_surface_manager& surface_manager){

		if(swapchain_p) {

			//
			frame_buffer_p->guarantee_generation();

			// fill command list
			{
				NRHI_COMMAND_LIST_ASYNC_SCOPE(
					NCPP_FOH_VALID(command_list_p),
					NCPP_FOH_VALID(command_allocator_p)
				);

				// transition back buffer state to RENDER_TARGET
				command_list_p->async_resource_barrier(
					H_resource_barrier::transition({
						.resource_p = swapchain_p->back_buffer_p().no_requirements(),
						.state_before = ED_resource_state::PRESENT,
						.state_after = ED_resource_state::RENDER_TARGET
					})
				);

				// clear rtv
				command_list_p->async_clear_rtv(
					swapchain_p->back_rtv_p(),
					F_vector4_f32 { 0, 0.84f, 0.96f, 1.0f }
				);

				//
				command_list_p->ZG_bind_root_signature(
					NCPP_FOH_VALID(root_signature_p)
				);

				// render text
				command_list_p->ZG_bind_pipeline_state(
					NCPP_FOH_VALID(pipeline_state_p)
				);
				command_list_p->ZG_bind_root_cbv_with_resource(
					0,
					NCPP_AOH_VALID(cbuffer_p)
				);
				command_list_p->ZIA_bind_index_buffer(
					NCPP_FOH_VALID(ibuffer_p),
					0
				);
				command_list_p->ZIA_bind_input_buffers(
					NCPP_INIL_SPAN(
						NCPP_FOH_VALID(vbuffer_p),
						NCPP_FOH_VALID(instance_buffer_p)
					),
					NCPP_INIL_SPAN(
						u32(0),
						u32(0)
					),
					0
				);
				command_list_p->ZOM_bind_frame_buffer(
					NCPP_FOH_VALID(frame_buffer_p)
				);
				command_list_p->async_draw_indexed_instanced(
					indices.size(),
					instances.size(),
					0,
					0,
					0
				);

				// transition back buffer state back to PRESENT
				command_list_p->async_resource_barrier(
					H_resource_barrier::transition({
						.resource_p = swapchain_p->back_buffer_p().no_requirements(),
						.state_before = ED_resource_state::RENDER_TARGET,
						.state_after = ED_resource_state::PRESENT
					})
				);
			}

			// submit command list and present swapchain
			++frame_index;
			command_queue_p->async_execute_command_list(
				NCPP_FOH_VALID(command_list_p)
			);
			swapchain_p->async_present();
			command_queue_p->async_signal(
				NCPP_FOH_VALID(fence_p),
				frame_index
			);
			fence_p->wait(frame_index);

			swapchain_p->update_back_rtv();

			//
			command_allocator_p->flush();
		}

		auto end_time = std::chrono::high_resolution_clock::now();

		u64 nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
		delta_time = (((f32)nanoseconds) * 0.000000001f);

	  	start_time = end_time;

		f32 fps = 1.0f / delta_time;

		NCPP_INFO() << "FPS: " << T_cout_value(fps);
	});

	return 0;
}
