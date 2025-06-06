
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
    auto command_list_p = H_command_list::create(
        NCPP_FOREF_VALID(device_p),
        F_command_list_desc {
            command_list_type
        }
    );
	command_list_p->end();



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
	U_buffer_handle instance_buffer_p = H_buffer::T_create<F_vector4>(
		NCPP_FOREF_VALID(device_p),
		instances,
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
    U_buffer_handle ibuffer_p = H_buffer::T_create<u32>(
        NCPP_FOREF_VALID(device_p),
        indices,
        ED_format::R32_UINT,
        ED_resource_flag::INDEX_BUFFER
    );

    F_vector4 output_color = { 0.5f, 0.5f, 0.5f, 1.0f };
    U_buffer_handle cbuffer_p = H_buffer::T_create<F_vector4>(
        NCPP_FOREF_VALID(device_p),
        NCPP_INIL_SPAN(output_color),
        ED_resource_flag::CONSTANT_BUFFER,
        ED_resource_heap_type::GREAD_CWRITE
    );

    U_texture_2d_handle texture_2d_p = H_texture::create_2d(
        NCPP_FOREF_VALID(device_p),
        {},
        1024,
        1024,
        ED_format::R8G8B8A8_UNORM,
        1,
        {},
        flag_combine(
            ED_resource_flag::SHADER_RESOURCE,
            ED_resource_flag::RENDER_TARGET
        )
    );
    U_srv_handle texture_2d_srv_p = H_resource_view::create_srv(
        NCPP_FOREF_VALID(device_p),
        {
            .resource_p = texture_2d_p.oref
        }
    );
    U_rtv_handle texture_2d_rtv_p = H_resource_view::create_rtv(
        NCPP_FOREF_VALID(device_p),
        {
            .resource_p = texture_2d_p.oref
        }
    );

	// input assembler desc
	F_input_assembler_desc input_assembler_desc = {
		.attribute_groups = {
			{
				{
					{
						.name = "VERTEX_POSITION",
						.format = ED_format::R32G32B32A32_FLOAT
					}
				}
			},
			{
				{
					{
						.name = "INSTANCE_POSITION",
						.format = ED_format::R32G32B32A32_FLOAT,
						.classification = ED_input_classification::PER_INSTANCE_DATA
					}
				}
			}
		}
	};

	// demo shader src
	G_string demo_shader_src = ""
		"cbuffer uniform_data : register(b0) { float4 output_color; };"
	   	"float4 vmain(float4 vertex_pos : VERTEX_POSITION, float4 instance_pos : INSTANCE_POSITION) : SV_POSITION"
	   	"{ return float4(instance_pos.xyz + vertex_pos.xyz, 1); }"
	   	"float4 pmain(float4 pos : SV_POSITION) : SV_TARGET"
	   	"{ return output_color; }";

	// create vertex shader
	auto vshader_binary = H_shader_compiler::compile_hlsl_from_src_content(
		"DemoShaderClass",
		"vmain",
		"vmain",
		demo_shader_src,
		"",
		5,
		0,
		ED_shader_type::VERTEX
	);

	// create pixel shader
	auto pshader_binary = H_shader_compiler::compile_hlsl_from_src_content(
		"DemoShaderClass",
		"pmain",
		"pmain",
		demo_shader_src,
		"",
		5,
		0,
		ED_shader_type::PIXEL
	);

	// create frame buffer
	auto frame_buffer_p = H_frame_buffer::create(
		NCPP_FOREF_VALID(device_p),
		{
			.color_attachments = {
				swapchain_p->back_rtv_p()
			}
		}
	);

	// resize frame buffer when surface is sized
	surface_p->T_get_event<F_surface_resize_event>().T_push_back_listener([&](auto& e){

	  	frame_buffer_p->rebuild();
	});

	// create graphics pipeline state
	auto graphics_pipeline_state_p = H_graphics_pipeline_state::create(
		NCPP_FOREF_VALID(device_p),
		{
			.input_assembler_desc = input_assembler_desc,
			.shader_binaries = {
				.vertex = (F_shader_binary_temp)vshader_binary,
				.pixel = eastl::optional<F_shader_binary_temp>(pshader_binary)
			}
		}
	);

	f32 delta_time = 0.0f;
	auto start_time = std::chrono::high_resolution_clock::now();

    // run app
    surface_manager.T_run([&](F_surface_manager& surface_manager){

		if(swapchain_p.is_valid()) {

			command_list_p->begin();

			//
			frame_buffer_p->guarantee_generation();

			// clear main render target view to cyan (r: 0, g: 255, b:255, a: 1.0)
			command_list_p->clear_rtv(
				NCPP_FHANDLE_VALID(back_rtv_p),
				{ 0.0f, 1.0f, 1.0f, 1.0f }
			);

			// update uniform data
			{
				static f64 t = 0.0;
				t += delta_time * 0.2f;

				output_color = lerp(
					F_vector4 { 0.2f, 0.2f, 0.2f, 1.0f },
					F_vector4 { 0.2f, 0.5f, 0.5f, 1.0f },
					sin(t * 15.0) * 0.5f + 0.5f
				);
				command_list_p->update_resource_data(
					NCPP_FHANDLE_VALID(cbuffer_p),
					&output_color,
					sizeof(F_vector4)
				);
			}

			// draw triangle
			{
				command_list_p->clear_state();

				command_list_p->ZG_bind_pipeline_state(
					NCPP_FHANDLE_VALID(graphics_pipeline_state_p)
				);

				command_list_p->ZIA_bind_index_buffer(
					NCPP_FHANDLE_VALID(ibuffer_p),
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
				command_list_p->ZPS_bind_constant_buffer(
					NCPP_FHANDLE_VALID(cbuffer_p),
					0
				);
				command_list_p->ZRS_bind_viewport(
					{
						.max_xy = surface_p->desc().size
					}
				);
				command_list_p->ZOM_bind_frame_buffer(
					NCPP_FOREF_VALID(frame_buffer_p)
				);
				command_list_p->draw_indexed_instanced(
					indices.size(),
					instances.size(),
					0,
					0,
					0
				);
			}

			command_list_p->end();

			// submit command lists to GPU
			command_queue_p->execute_command_list(
				NCPP_FOREF_VALID(command_list_p)
			);

			// finalize rendering, swap back buffer and front buffer
			swapchain_p->present();
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
