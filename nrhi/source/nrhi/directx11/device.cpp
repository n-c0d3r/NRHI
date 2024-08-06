#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/adapter.hpp>



namespace nrhi {

    F_directx11_device::F_directx11_device(TKPA_valid<A_adapter> adapter_p) :
        A_device(adapter_p)
    {
        HRESULT hr;

        D3D_FEATURE_LEVEL feature_levels[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_11_1
        };
        UINT num_feature_levels = ARRAYSIZE(feature_levels);

		UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef NCPP_ENABLE_ASSERT
		creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		hr = D3D11CreateDevice(
			adapter_p.T_cast<F_directx11_adapter>()->dxgi_adapter_p(),
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			creation_flags,
			feature_levels,
			num_feature_levels,
			D3D11_SDK_VERSION,
			&d3d11_device_p_,
			0,
			0
		);

        NCPP_ASSERT(SUCCEEDED(hr)) << "can't create d3d11 device";
    }
    F_directx11_device::~F_directx11_device(){

        if(d3d11_device_p_)
            d3d11_device_p_->Release();
    }

	ID3D11DeviceContext* F_directx11_device::d3d11_immediate_ctx_p_unsafe(){

		ID3D11DeviceContext* result = 0;
		d3d11_device_p_->GetImmediateContext(&result);

		return result;
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



    TU<A_device> HD_directx11_device::create(TKPA_valid<A_adapter> adapter_p){

        return TU<F_directx11_device>()(adapter_p);
    }

}