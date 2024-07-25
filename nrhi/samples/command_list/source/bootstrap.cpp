
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
	NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_WORK_SUBMISSION(
		auto command_allocator_p = H_command_allocator::create(
			NCPP_FOREF_VALID(device_p),
			F_command_allocator_desc {
				ED_command_list_type::DIRECT
			}
		);
		auto command_list_p = H_command_list::create_with_command_allocator(
			NCPP_FOREF_VALID(device_p),
			F_command_list_desc {
				ED_command_list_type::DIRECT,
				command_allocator_p.keyed()
			}
		);
		command_list_p->async_end();
	)
	else NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_WORK_SUBMISSION(
		auto command_list_p = H_command_list::create(
			NCPP_FOREF_VALID(device_p),
			F_command_list_desc {
				ED_command_list_type::DIRECT
			}
		);
	);

	return 0;
}
