
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
	auto command_list_type = ED_command_list_type::BLIT;

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
	TU<A_fence> fence_p;

	fence_p = H_fence::create(
		NCPP_FOREF_VALID(device_p),
		F_fence_desc {}
	);



	// create resources
	auto resource_1_p = H_buffer::T_create_committed<F_vector4_f32>(
		NCPP_FOH_VALID(device_p),
		128
	);
	auto resource_2_p = H_buffer::T_create_committed<F_vector4_f32>(
		NCPP_FOH_VALID(device_p),
		128
	);



	// fill command list
	command_list_p->async_begin(
		NCPP_FOH_VALID(command_allocator_p)
	);
	command_list_p->async_copy_resource(
		NCPP_FOH_VALID(resource_1_p),
		NCPP_FOH_VALID(resource_2_p)
	);
	command_list_p->async_end();



	// submit command list to GPU
	command_queue_p->async_execute_command_list(
		NCPP_FOH_VALID(command_list_p)
	);
	command_queue_p->async_signal(
		NCPP_FOH_VALID(fence_p),
		1
	);
	fence_p->wait(1);

	return 0;
}
