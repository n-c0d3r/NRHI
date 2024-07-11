#include <nrhi/directx12/factory.hpp>
#include <nrhi/directx12/adapter.hpp>



namespace nrhi {

    IDXGIFactory* F_directx12_factory_helper::factory_p_ = 0;



    void F_directx12_factory_helper::initialize() {

		UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
		// Enable the debug layer (requires the Graphics Tools "optional feature").
		// NOTE: Enabling the debug layer after device creation will invalidate the active device.
		{
			ID3D12Debug* debugController = 0;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();

				// Enable additional debug layers.
				dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}
		}
#endif

        HRESULT hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory_p_));
        NCPP_ASSERT(!FAILED(hr)) << "can't create factory";

        HD_directx12_adapter::initialize_adapters();
    }
    void F_directx12_factory_helper::release() {

        HD_directx12_adapter::release_adapters();

        factory_p_->Release();
    }

}