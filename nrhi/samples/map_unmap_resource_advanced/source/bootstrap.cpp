
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



	// upload resource
	auto resource_p = H_buffer::create_committed(
		NCPP_FOH_VALID(device_p),
		128,
		1,
		ED_resource_flag::NONE,
		ED_resource_heap_type::GREAD_CWRITE,
		ED_resource_state::_GENERIC_READ
	);



	// map resource
	auto mapped_subresource = resource_p->map(0);

	for(u32 i = 0; i < 128; ++i)
		mapped_subresource[i] = i;

	resource_p->unmap(0);

	return 0;
}
