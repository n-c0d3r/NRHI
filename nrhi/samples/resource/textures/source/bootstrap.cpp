
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



	// create texture 1d
	auto texture_1d_p = H_texture::create_committed_1d(
		NCPP_FOH_VALID(device_p),
		128
	);
	auto texture_2d_p = H_texture::create_committed_2d(
		NCPP_FOH_VALID(device_p),
		128,
		128
	);
	auto texture_3d_p = H_texture::create_committed_3d(
		NCPP_FOH_VALID(device_p),
		128,
		128,
		128
	);

	return 0;
}
