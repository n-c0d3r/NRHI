#include <nrhi/directx12/state_object_properties.hpp>
#include <nrhi/directx12/state_object.hpp>



namespace nrhi
{
    void HD_directx12_state_object_properties::construct_with_state_object(
        void* payload_p,
        TKPA_valid<A_state_object> state_object_p
    )
    {
        ID3D12StateObjectProperties* d3d12_state_object_properties_p;
        HRESULT hr = state_object_p.T_cast<F_directx12_state_object>()->d3d12_state_object_p()->QueryInterface(
            IID_PPV_ARGS(&d3d12_state_object_properties_p)
        );
        NCPP_ASSERT(SUCCEEDED((hr)));

        *((ID3D12StateObjectProperties**)payload_p) = d3d12_state_object_properties_p;
    }
}