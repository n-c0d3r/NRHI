#include <nrhi/directx11/device.hpp>



namespace nrhi {

    F_directx11_device::F_directx11_device(TK_valid<A_adapter> adapter_p) :
        A_device(adapter_p)
    {

        HRESULT hr;

        D3D_DRIVER_TYPE driver_types[] = {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE
        };
        UINT num_driver_types = ARRAYSIZE(driver_types);

        D3D_FEATURE_LEVEL feature_levels[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_11_1
        };
        UINT num_feature_levels = ARRAYSIZE(feature_levels);

        for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
        {
            hr = D3D11CreateDevice(
                NULL,
                driver_types[driver_type_index],
                NULL,
                NULL,
                feature_levels,
                num_feature_levels,
                D3D11_SDK_VERSION,
                &d3d11_device_p_,
                0,
                0
            );
            if (SUCCEEDED(hr))
                break;
            ++driver_type_index;
        }

        NCPP_ASSERT(SUCCEEDED(hr)) << "can't create d3d11 device";
    }
    F_directx11_device::~F_directx11_device(){

        d3d11_device_p_->Release();
    }

    ID3D11DeviceContext* F_directx11_device::pop_d3d11_immediate_ctx(){

        NCPP_ASSERT(!is_d3d11_immediate_ctx_obtained_) << "d3d11 immediate ctx was already obtained";

        ID3D11DeviceContext* result = 0;
        d3d11_device_p_->GetImmediateContext(&result);

        return result;
    }
    void F_directx11_device::push_d3d11_immediate_ctx() {

        is_d3d11_immediate_ctx_obtained_ = false;
    }



    TU<A_device> HD_directx11_device::create(TK_valid<A_adapter> adapter_p){

        return TU<F_directx11_device>()(adapter_p);
    }

}