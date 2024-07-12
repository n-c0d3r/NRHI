
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

	return 0;
}
