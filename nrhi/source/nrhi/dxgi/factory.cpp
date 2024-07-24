#include <nrhi/dxgi/factory.hpp>
#include <nrhi/dxgi/adapter.hpp>



namespace nrhi {

    IDXGIFactory* F_dxgi_factory_helper::factory_p_ = 0;



    void F_dxgi_factory_helper::initialize() {

        HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory_p_);

        NCPP_ASSERT(!FAILED(hr)) << "can't create factory";

        HD_dxgi_adapter::initialize_adapters();

    }
    void F_dxgi_factory_helper::release() {

        HD_dxgi_adapter::release_adapters();

        factory_p_->Release();
    }

}