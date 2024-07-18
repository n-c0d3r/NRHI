
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



	// create descriptor heap
	auto descriptor_heap_p = H_descriptor_heap::create(
		NCPP_FOH_VALID(device_p),
		{
			.type = ED_descriptor_heap_type::CBV_SRV_UAV,
			.descriptor_count = 1
		}
	);
	F_descriptor_cpu_address base_cpu_address = descriptor_heap_p->base_cpu_address();
	u64 descriptor_increment_size = device_p->descriptor_increment_size(ED_descriptor_heap_type::CBV_SRV_UAV);

	// create buffer
	auto buffer_p = H_buffer::create_committed(
		NCPP_FOH_VALID(device_p),
		128,
		4,
		ED_resource_bind_flag::SRV
	);

	// create descriptor
	H_descriptor::initialize_srv(
		NCPP_FOH_VALID(descriptor_heap_p),
		base_cpu_address,
		{
			.resource_p = NCPP_FOH_VALID(buffer_p)
		}
	);

	return 0;
}
