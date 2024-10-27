#include <nrhi/directx12/state_object_builder.hpp>
#include <nrhi/directx12/state_object.hpp>
#include <nrhi/directx12/device.hpp>



namespace nrhi
{
    TU<A_state_object> HD_directx12_state_object_builder::build(
        void* payload_p,
        TKPA_valid<A_device> device_p
    )
    {
        ID3D12Device5* d3d12_device_5_p;
        HRESULT hr = device_p.T_cast<F_directx12_device>()->d3d12_device_p()->QueryInterface(IID_PPV_ARGS(&d3d12_device_5_p));
        NCPP_ASSERT(SUCCEEDED(hr));

        ID3D12StateObject* d3d12_state_object_p;
        hr = d3d12_device_5_p->CreateStateObject(
            *((CD3DX12_STATE_OBJECT_DESC*)payload_p),
            IID_PPV_ARGS(&d3d12_state_object_p)
        );
        NCPP_ASSERT(SUCCEEDED(hr)) << "can't create state object";

        return TU<F_directx12_state_object>()(
            device_p,
            d3d12_state_object_p
        );
    }
    TU<A_state_object> HD_directx12_state_object_builder::add_to(
        void* payload_p,
        TKPA_valid<A_state_object> src_state_object_p
    )
    {
        ID3D12StateObject* d3d12_src_state_object_p = src_state_object_p.T_cast<F_directx12_state_object>()->d3d12_state_object_p();

        auto device_p = src_state_object_p->device_p();

        ID3D12Device7* d3d12_device_7_p;
        HRESULT hr = device_p.T_cast<F_directx12_device>()->d3d12_device_p()->QueryInterface(IID_PPV_ARGS(&d3d12_device_7_p));
        NCPP_ASSERT(SUCCEEDED(hr));

        ID3D12StateObject* d3d12_state_object_p;
        hr = d3d12_device_7_p->AddToStateObject(
            *((CD3DX12_STATE_OBJECT_DESC*)payload_p),
            d3d12_src_state_object_p,
            IID_PPV_ARGS(&d3d12_state_object_p)
        );
        NCPP_ASSERT(SUCCEEDED(hr)) << "can't add to state object";

        return TU<F_directx12_state_object>()(
            device_p,
            d3d12_state_object_p
        );
    }
}