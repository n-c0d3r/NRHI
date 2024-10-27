#include <nrhi/state_object_base.hpp>



namespace nrhi
{
    A_state_object::A_state_object(TKPA_valid<A_device> device_p) :
        A_device_child(device_p)
    {
    }
}