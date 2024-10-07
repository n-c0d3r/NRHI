#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/adapter.hpp>



namespace nrhi {

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	void F_directx12_device::set_debug_name(const F_debug_name& value)
	{
		A_render_object::set_debug_name(value);

		auto wvalue = G_to_wstring(value.c_str());
		d3d12_device_p_->SetName(wvalue.data());
	}
#endif



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

	E_resource_heap_tier HD_directx12_device::resource_heap_tier(
		TKPA_valid<A_device> device_p
	) {
		D3D12_FEATURE_DATA_D3D12_OPTIONS d3d12_feature_data_d3d12_options;

		device_p.T_cast<F_directx12_device>()->d3d12_device_p()->CheckFeatureSupport(
			D3D12_FEATURE_D3D12_OPTIONS,
			&d3d12_feature_data_d3d12_options,
			sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS)
		);

		switch (d3d12_feature_data_d3d12_options.ResourceHeapTier)
		{
		case D3D12_RESOURCE_HEAP_TIER_1:
			return E_resource_heap_tier::A;
		case D3D12_RESOURCE_HEAP_TIER_2:
			return E_resource_heap_tier::B;
		}
	}

	struct eastl::pair<u32, u32> HD_directx12_device::hlsl_highest_shader_model(
		TKPA_valid<A_device> device_p
	)
	{
		auto d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();

		{
			D3D12_FEATURE_DATA_SHADER_MODEL shader_model = {};
			shader_model.HighestShaderModel = D3D_SHADER_MODEL_6_7;
			if(
				SUCCEEDED(
					d3d12_device_p->CheckFeatureSupport(
						D3D12_FEATURE_SHADER_MODEL,
						&shader_model,
						sizeof(shader_model)
					)
				)
			)
			{
				return { 6, 7 };
			}
		}

		{
			D3D12_FEATURE_DATA_SHADER_MODEL shader_model = {};
			shader_model.HighestShaderModel = D3D_SHADER_MODEL_6_6;
			if(
				SUCCEEDED(
					d3d12_device_p->CheckFeatureSupport(
						D3D12_FEATURE_SHADER_MODEL,
						&shader_model,
						sizeof(shader_model)
					)
				)
			)
			{
				return { 6, 6 };
			}
		}

		{
			D3D12_FEATURE_DATA_SHADER_MODEL shader_model = {};
			shader_model.HighestShaderModel = D3D_SHADER_MODEL_6_5;
			if(
				SUCCEEDED(
					d3d12_device_p->CheckFeatureSupport(
						D3D12_FEATURE_SHADER_MODEL,
						&shader_model,
						sizeof(shader_model)
					)
				)
			)
			{
				return { 6, 5 };
			}
		}

		{
			D3D12_FEATURE_DATA_SHADER_MODEL shader_model = {};
			shader_model.HighestShaderModel = D3D_SHADER_MODEL_6_4;
			if(
				SUCCEEDED(
					d3d12_device_p->CheckFeatureSupport(
						D3D12_FEATURE_SHADER_MODEL,
						&shader_model,
						sizeof(shader_model)
					)
				)
			)
			{
				return { 6, 4 };
			}
		}

		{
			D3D12_FEATURE_DATA_SHADER_MODEL shader_model = {};
			shader_model.HighestShaderModel = D3D_SHADER_MODEL_6_3;
			if(
				SUCCEEDED(
					d3d12_device_p->CheckFeatureSupport(
						D3D12_FEATURE_SHADER_MODEL,
						&shader_model,
						sizeof(shader_model)
					)
				)
			)
			{
				return { 6, 3 };
			}
		}

		{
			D3D12_FEATURE_DATA_SHADER_MODEL shader_model = {};
			shader_model.HighestShaderModel = D3D_SHADER_MODEL_6_2;
			if(
				SUCCEEDED(
					d3d12_device_p->CheckFeatureSupport(
						D3D12_FEATURE_SHADER_MODEL,
						&shader_model,
						sizeof(shader_model)
					)
				)
			)
			{
				return { 6, 2 };
			}
		}

		{
			D3D12_FEATURE_DATA_SHADER_MODEL shader_model = {};
			shader_model.HighestShaderModel = D3D_SHADER_MODEL_6_1;
			if(
				SUCCEEDED(
					d3d12_device_p->CheckFeatureSupport(
						D3D12_FEATURE_SHADER_MODEL,
						&shader_model,
						sizeof(shader_model)
					)
				)
			)
			{
				return { 6, 1 };
			}
		}

		{
			D3D12_FEATURE_DATA_SHADER_MODEL shader_model = {};
			shader_model.HighestShaderModel = D3D_SHADER_MODEL_6_0;
			if(
				SUCCEEDED(
					d3d12_device_p->CheckFeatureSupport(
						D3D12_FEATURE_SHADER_MODEL,
						&shader_model,
						sizeof(shader_model)
					)
				)
			)
			{
				return { 6, 0 };
			}
		}

		{
			D3D12_FEATURE_DATA_SHADER_MODEL shader_model = {};
			shader_model.HighestShaderModel = D3D_SHADER_MODEL_5_1;
			if(
				SUCCEEDED(
					d3d12_device_p->CheckFeatureSupport(
						D3D12_FEATURE_SHADER_MODEL,
						&shader_model,
						sizeof(shader_model)
					)
				)
			)
			{
				return { 5, 1 };
			}
		}

		return { 0, 0 };
	}
	u32 HD_directx12_device::wave_size(
        TKPA_valid<A_device> device_p
    )
    {
		auto d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();

	    D3D12_FEATURE_DATA_D3D12_OPTIONS1 options;
		d3d12_device_p->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &options, sizeof(options));

		return options.BaselineWaveSizeInThreads;
    }
}