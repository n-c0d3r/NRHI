
#include <nrhi/.hpp>

using namespace nrhi;



int main() {

    // initialize nrhi and deferredly release it
    nrhi::initialize_system();
    NCPP_DEFER(nrhi::release_system());



    // get adapter and create device
    const auto& adapter_p_vector = H_adapter::adapter_p_vector();
    NCPP_ASSERT(adapter_p_vector.size());

	return 0;
}
