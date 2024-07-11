#include <nrhi/descriptor_base.hpp>



#ifdef NRHI_DRIVER_SUPPORT_DESCRIPTOR_MANAGEMENT
namespace nrhi {

    A_descriptor_heap::A_descriptor_heap(TK_valid<A_device> device_p, const F_descriptor_heap_desc& desc) :
        device_p_(device_p),
        desc_(desc)
    {
    }
    A_descriptor_heap::~A_descriptor_heap(){
    }

}
#endif // NRHI_DRIVER_SUPPORT_DESCRIPTOR_MANAGEMENT