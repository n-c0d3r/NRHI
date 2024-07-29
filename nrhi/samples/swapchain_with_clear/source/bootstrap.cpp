
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



	// create command queue
	auto command_queue_p = H_command_queue::create(
		NCPP_FOREF_VALID(device_p),
		F_command_queue_desc {
			ED_command_list_type::DIRECT
		}
	);



	// create command list
	TU<A_command_allocator> command_allocator_p;
	TU<A_command_list> command_list_p;
	NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_WORK_SUBMISSION(
		command_allocator_p = H_command_allocator::create(
			NCPP_FOREF_VALID(device_p),
			F_command_allocator_desc {
				ED_command_list_type::DIRECT
			}
		);
		command_list_p = H_command_list::create_with_command_allocator(
			NCPP_FOREF_VALID(device_p),
			F_command_list_desc {
				ED_command_list_type::DIRECT,
				command_allocator_p.keyed()
			}
		);
		command_list_p->async_end();
	)
	else NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_WORK_SUBMISSION(
		command_list_p = H_command_list::create(
			NCPP_FOREF_VALID(device_p),
			F_command_list_desc {
				ED_command_list_type::DIRECT
			}
		);
	);



	// create fence
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
	TU<A_fence> fence_p;

	NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_WORK_SUBMISSION(
		fence_p = H_fence::create(
			NCPP_FOREF_VALID(device_p),
			F_fence_desc {}
		);
	);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION



	// create surface manager and surface
	F_surface_manager surface_manager;
	auto surface_p = surface_manager.create_surface({
		.title = L"Swapchain ",
		.size = { 700, 700 }
	});



	//create swapchain
	auto swapchain_p = H_swapchain::create(
		NCPP_FOREF_VALID(command_queue_p),
		surface_p,
		F_swapchain_desc {
		}
	);



	// delete swapchain when surface is destroyed
	surface_p->T_get_event<F_surface_destroy_event>().T_push_back_listener([&](auto& e){

	  	swapchain_p.reset();
	});



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
	// frame counter
	u64 frame_counter = 0;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION



	// run app
	surface_manager.T_run([&](F_surface_manager& surface_manager){

		if(!swapchain_p)
			return;

	  	auto back_rtv_p = swapchain_p->back_rtv_p();
	  	auto back_buffer_p = swapchain_p->back_buffer_p();

	  	NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_WORK_SUBMISSION(
			command_list_p->clear_rtv(
				back_rtv_p,
				F_vector4::forward()
			);

			command_queue_p->execute_command_list(
				NCPP_FOH_VALID(command_list_p)
			);
	  	)
	  	else NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_WORK_SUBMISSION(
		  	command_allocator_p->flush();

			command_list_p->async_begin(
				NCPP_FOH_VALID(command_allocator_p)
			);

			command_list_p->async_resource_barrier(
				H_resource_barrier::transition({
					.resource_p = back_buffer_p.no_requirements(),
					.state_before = ED_resource_state::PRESENT,
					.state_after = ED_resource_state::RENDER_TARGET
				})
			);

			command_list_p->async_clear_rtv_with_descriptor(
				back_rtv_p->descriptor().handle.cpu_address,
				F_vector4::forward()
			);

			command_list_p->async_resource_barrier(
				H_resource_barrier::transition({
					.resource_p = back_buffer_p.no_requirements(),
				  	.state_before = ED_resource_state::RENDER_TARGET,
					.state_after = ED_resource_state::PRESENT
				})
			);

			command_list_p->async_end();

			command_queue_p->async_execute_command_list(
				NCPP_FOH_VALID(command_list_p)
			);
	  	);

	  	// present swapchain
		NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_WORK_SUBMISSION(
	  		swapchain_p->present();
	 	)
		else NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_WORK_SUBMISSION(
			swapchain_p->async_present();

			u64 target_fence_value = frame_counter;
			command_queue_p->async_signal(
				NCPP_FOH_VALID(fence_p),
				target_fence_value
			);
			fence_p->wait(target_fence_value);

			swapchain_p->update_back_rtv();

			++frame_counter;
		);
	});

	return 0;
}
