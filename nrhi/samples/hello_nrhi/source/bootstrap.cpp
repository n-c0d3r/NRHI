
#include <nrhi/.hpp>

using namespace nrhi;



int main() {

    NCPP_INFO()
        << "Hello "
        << E_log_color::V_FOREGROUND_BRIGHT_MAGNETA
        << "NRHI "
        << E_log_color::V_FOREGROUND_BRIGHT_BLUE
        << NRHI_VERSION_STR
        << E_log_color::RESET;



    // initialize nrhi and deferrered release it
    nrhi::initialize_system();
    NCPP_DEFER(nrhi::release_system());



    // get adapter and create device
    const auto& adapter_p_vector = H_adapter::adapter_p_vector();
    NCPP_ASSERT(adapter_p_vector.size());

    auto device_p = H_device::create(adapter_p_vector[0]);



    // create command queue and command list
    auto command_list_type = E_command_list_type::DIRECT;

    auto command_queue_p = H_command_queue::create(
        NCPP_FOREF_VALID(device_p),
        F_command_queue_desc {
            command_list_type
        }
    );
    auto command_list_p = H_command_list::create(
        NCPP_FOREF_VALID(device_p),
        F_command_list_desc {
            command_list_type
        }
    );



    // create surface manager and surface
    F_surface_manager surface_manager;
    auto surface_p = surface_manager.create_surface({
        .title = G_wstring(L"Hello NRHI ") + TEXT(NRHI_VERSION_STR),
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



    TG_vector<F_vector4> vertices = {
		{ 0.05f, 0.0f, 0.5f, 1.0f },
		{ 0.0f, 0.0f, 0.5f, 1.0f },
		{ 0.0f, 0.05f, 0.5f, 1.0f },
		{ 0.05f, 0.05f, 0.5f, 1.0f }
	};
    U_buffer_handle vbuffer_p = H_buffer::T_create<F_vector4>(
        NCPP_FOREF_VALID(device_p),
        vertices,
        E_resource_bind_flag::VBV
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
	U_buffer_handle instance_buffer_p = H_buffer::T_create<F_vector4>(
		NCPP_FOREF_VALID(device_p),
		instances,
		E_resource_bind_flag::INSTBV
	);

    TG_vector<u32> indices = {
		0,
		1,
		2,
		0,
		2,
		3
	};
    U_buffer_handle ibuffer_p = H_buffer::T_create<u32>(
        NCPP_FOREF_VALID(device_p),
        indices,
        E_format::R32_UINT,
        E_resource_bind_flag::IBV
    );

    TG_vector<F_matrix4x4> buffer2_data(128);
    U_structured_buffer_handle buffer2_p = H_buffer::T_create_structured<F_matrix4x4>(
        NCPP_FOREF_VALID(device_p),
        buffer2_data,
        E_resource_bind_flag::SRV
    );
    U_srv_handle buffer2_srv_p = H_resource_view::create_srv(
        NCPP_FOREF_VALID(device_p),
        {
            .resource_p = NCPP_FHANDLE_VALID_AS_OREF(buffer2_p)
        }
    );

    F_vector4 cdata;
    U_single_elemented_buffer_handle cbuffer_p = H_buffer::T_create_single_elemented<F_vector4>(
        NCPP_FOREF_VALID(device_p),
        &cdata,
        E_resource_bind_flag::CBV
    );

    U_texture_2d_handle texture_2d_p = H_texture::create_2d(
        NCPP_FOREF_VALID(device_p),
        {},
        1024,
        1024,
        E_format::R8G8B8A8_UNORM,
        1,
        {},
        flag_combine(
            E_resource_bind_flag::SRV,
            E_resource_bind_flag::RTV
        )
    );
    U_srv_handle texture_2d_srv_p = H_resource_view::create_srv(
        NCPP_FOREF_VALID(device_p),
        {
            .resource_p = NCPP_FHANDLE_VALID_AS_OREF(texture_2d_p)
        }
    );
    U_rtv_handle texture_2d_rtv_p = H_resource_view::create_rtv(
        NCPP_FOREF_VALID(device_p),
        {
            .resource_p = NCPP_FHANDLE_VALID_AS_OREF(texture_2d_p)
        }
    );

	// create demo shader class
	auto shader_class_p = H_shader_compiler::compile_hlsl(
		// shader class name
		"DemoShaderClass",
		// shader class source content
		"float4 vmain(float4 vertex_pos : VERTEX_POSITION, float4 instance_pos : INSTANCE_POSITION) : SV_POSITION"
		"{ return float4(instance_pos.xyz + vertex_pos.xyz, 1); }"
		"float4 pmain(float4 pos : SV_POSITION) : SV_TARGET"
		"{ return float4(0.2, 0.2, 0.2, 1.0); }",
		// shader kernel descriptors (each kernel has 1 entry point function and is compiled to 1 shader blob)
		NCPP_INIL_SPAN(
			F_shader_kernel_desc {
				.blob_desc = {
					.name = "vmain",
					.type = E_shader_type::VERTEX,
					.vertex_attribute_group_desc_vector = {
						{
							.attribute_desc_vector = {
								{
									.name = "VERTEX_POSITION",
									.format = E_format::R32G32B32A32_FLOAT
								}
							}
						}
					},
					.instance_attribute_group_desc_vector = {
						{
							.attribute_desc_vector = {
								{
									.name = "INSTANCE_POSITION",
									.format = E_format::R32G32B32A32_FLOAT
								}
							}
						}
					}
				}
			},
			F_shader_kernel_desc {
				.blob_desc = {
					.name = "pmain",
					.type = E_shader_type::PIXEL,
					.render_target_view_count = 1
				}
			}
		)
	);

	// get vertex shader blob (the object storing compiled shader binary from hlsl)
	auto vshader_blob_p = shader_class_p->shader_blob_p("vmain");

	// create vertex shader from vertex shader blob
	auto vshader_p = H_vertex_shader::create(
		NCPP_FOREF_VALID(device_p),
		{
			.blob_p = NCPP_FOREF_VALID(vshader_blob_p)
		}
	);

	// get pixel shader blob (the object storing compiled shader binary from hlsl)
	auto pshader_blob_p = shader_class_p->shader_blob_p("pmain");

	// create pixel shader from pixel shader blob
	auto pshader_p = H_pixel_shader::create(
		NCPP_FOREF_VALID(device_p),
		{
			.blob_p = NCPP_FOREF_VALID(pshader_blob_p)
		}
	);

	// create frame buffer
	auto frame_buffer_p = H_frame_buffer::create(
		NCPP_FOREF_VALID(device_p),
		{
			.color_attachment_p_vector = {
				swapchain_p->back_rtv_p()
			}
		}
	);

	// create graphics pipeline state
	auto graphics_pipeline_state_p = H_graphics_pipeline_state::create(
		NCPP_FOREF_VALID(device_p),
		{
			.primitive_topology = E_primitive_topology::TRIANGLE_LIST,
			.shader_p_vector = {
				NCPP_FHANDLE_VALID_AS_OREF(vshader_p),
				NCPP_FHANDLE_VALID_AS_OREF(pshader_p)
			}
		}
	);

    // run app
    surface_manager.T_run([&](F_surface_manager& surface_manager){

		auto start_time = std::chrono::high_resolution_clock::now();

		if(swapchain_p.is_valid()) {

			// update frame buffer
			frame_buffer_p->update_viewport();

			// clear main render target view to cyan (r: 0, g: 255, b:255, a: 1.0)
			command_list_p->clear_rtv(
				NCPP_FHANDLE_VALID(back_rtv_p),
				{ 0.0f, 1.0f, 1.0f, 1.0f }
			);

			// draw triangle
			{
				command_list_p->clear_state();
				command_list_p->bind_frame_buffer(
					NCPP_FOREF_VALID(frame_buffer_p)
				);
				command_list_p->bind_graphics_pipeline_state(
					NCPP_FHANDLE_VALID(graphics_pipeline_state_p)
				);
				command_list_p->ZIA_bind_index_buffer(
					NCPP_FHANDLE_VALID(ibuffer_p),
					0
				);
				command_list_p->ZIA_bind_vertex_buffer(
					NCPP_FHANDLE_VALID(vbuffer_p),
					0,
					0
				);
				command_list_p->ZIA_bind_instance_buffer(
					NCPP_FHANDLE_VALID(instance_buffer_p),
					0,
					0
				);
				command_list_p->draw_indexed_instanced(
					indices.size(),
					instances.size(),
					0,
					0,
					0
				);
			}

			// submit command lists to GPU
			command_queue_p->execute_command_lists(
				NCPP_INIL_SPAN(
					NCPP_FOREF_VALID(command_list_p)
				)
			);

			// finalize rendering, swap back buffer and front buffer
			swapchain_p->present();
		}

		auto end_time = std::chrono::high_resolution_clock::now();

		u64 nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

		f32 fps = 1.0f / (((f32)nanoseconds) * 0.000000001f);

		NCPP_INFO() << "FPS: " << T_cout_value(fps);
	});

	return 0;
}
