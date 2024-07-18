#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/adapter.hpp>



namespace nrhi {

    F_directx12_device::F_directx12_device(TK_valid<A_adapter> adapter_p) :
        A_device(adapter_p)
    {
		HRESULT hr;

		D3D_FEATURE_LEVEL feature_levels[] = {
			D3D_FEATURE_LEVEL_12_2,
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0
		};
		UINT num_feature_levels = ARRAYSIZE(feature_levels);

		for (UINT feature_level_index = 0; feature_level_index < num_feature_levels;)
		{
			hr = D3D12CreateDevice(
				adapter_p.T_cast<F_directx12_adapter>()->dxgi_adapter_p(),
				feature_levels[feature_level_index],
				IID_PPV_ARGS(&d3d12_device_p_)
			);
			if (SUCCEEDED(hr))
				break;
			++feature_level_index;
		}

		NCPP_ASSERT(SUCCEEDED(hr)) << "can't create d3d12 device";
    }
    F_directx12_device::~F_directx12_device(){
    }



    TU<A_device> HD_directx12_device::create(TKPA_valid<A_adapter> adapter_p){

        return TU<F_directx12_device>()(adapter_p);
    }

	u64 HD_directx12_device::descriptor_increment_size(TKPA_valid<A_device> device_p, ED_descriptor_heap_type descriptor_heap_type) {

		return device_p.T_cast<F_directx12_device>()->d3d12_device_p()->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE(descriptor_heap_type)
		);
	}

}