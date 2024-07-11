#include <nrhi/directx11/factory.hpp>
#include <nrhi/directx11/adapter.hpp>



namespace nrhi {

    IDXGIFactory* F_directx11_factory_helper::factory_p_ = 0;



    void F_directx11_factory_helper::initialize() {

        HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory_p_);

        NCPP_ASSERT(!FAILED(hr)) << "can't create factory";

        HD_directx11_adapter::initialize_adapters();

    }
    void F_directx11_factory_helper::release() {

        HD_directx11_adapter::release_adapters();

        factory_p_->Release();
    }

}