
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
	u64 descriptor_increment_size = device_p->descriptor_increment_size(ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS);


	// create descriptor heap 1
	auto descriptor_heap_1_p = H_descriptor_heap::create(
		NCPP_FOH_VALID(device_p),
		{
			.type = ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
			.descriptor_count = 1
		}
	);
	F_descriptor_cpu_address base_cpu_address_1 = descriptor_heap_1_p->base_cpu_address();

	// create buffer
	auto buffer_p = H_buffer::create_committed(
		NCPP_FOH_VALID(device_p),
		128,
		ED_format::R32G32B32A32_FLOAT,
		ED_resource_bind_flag::SHADER_RESOURCE
	);

	// create descriptor
	H_descriptor::initialize_srv(
		NCPP_FOH_VALID(descriptor_heap_1_p),
		base_cpu_address_1,
		{
			.resource_p = NCPP_FOH_VALID(buffer_p)
		}
	);



	// create descriptor heap 2
	auto descriptor_heap_2_p = H_descriptor_heap::create(
		NCPP_FOH_VALID(device_p),
		{
			.type = ED_descriptor_heap_type::SAMPLER,
			.descriptor_count = 1
		}
	);
	F_descriptor_cpu_address base_cpu_address_2 = descriptor_heap_2_p->base_cpu_address();

	// create sampler state
	H_descriptor::initialize_sampler_state(
		NCPP_FOH_VALID(descriptor_heap_2_p),
		base_cpu_address_2,
		{}
	);

	return 0;
}
