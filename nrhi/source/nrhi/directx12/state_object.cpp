#include <nrhi/directx12/state_object.hpp>



namespace nrhi
{
    F_directx12_state_object::F_directx12_state_object(
        TKPA_valid<A_device> device_p,
        ID3D12StateObject* d3d12_state_object_p
    ) :
        A_state_object(device_p),
        d3d12_state_object_p_(d3d12_state_object_p)
    {
    }
}