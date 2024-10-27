#pragma once

#include <nrhi/state_object_base.hpp>


namespace nrhi
{
    class NRHI_API F_directx12_state_object : public A_state_object
    {
    private:
        ID3D12StateObject* d3d12_state_object_p_ = 0;

    public:
        NCPP_FORCE_INLINE ID3D12StateObject* d3d12_state_object_p() const noexcept { return d3d12_state_object_p_; }



    public:
        F_directx12_state_object(
            TKPA_valid<A_device> device_p,
            ID3D12StateObject* d3d12_state_object_p
        );
    };



    class NRHI_API HD_directx12_state_object
    {
    };
}
