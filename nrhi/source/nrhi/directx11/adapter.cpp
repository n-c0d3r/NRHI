#include <nrhi/directx11/adapter.hpp>
#include <nrhi/directx11/factory.hpp>



namespace nrhi {

    F_directx11_adapter::F_directx11_adapter(u32 index, IDXGIAdapter* dxgi_adapter_p) :
        A_adapter(index),
        dxgi_adapter_p_(dxgi_adapter_p)
    {

    }
	F_directx11_adapter::~F_directx11_adapter(){

    }



    TG_vector<TU<A_adapter>> HD_directx11_adapter::unique_adapter_p_vector_;
    TG_vector<TK_valid<A_adapter>> HD_directx11_adapter::keyed_adapter_p_vector_;



    const TG_vector<TK_valid<A_adapter>>& HD_directx11_adapter::adapter_p_vector() {

        return keyed_adapter_p_vector_;
    }



    void HD_directx11_adapter::initialize_adapters() {

        auto* factory_p = F_directx11_factory_helper::factory_p();
        IDXGIAdapter* dxgi_adapter_p = 0;

        for (UINT i = 0; factory_p->EnumAdapters(i, &dxgi_adapter_p) != DXGI_ERROR_NOT_FOUND; ++i) {

            auto adapter_p = TU<F_directx11_adapter>()(i, dxgi_adapter_p);

            keyed_adapter_p_vector_.push_back(
                NCPP_FOREF_VALID(adapter_p)
            );
            unique_adapter_p_vector_.push_back(std::move(adapter_p));
        }

    }
    void HD_directx11_adapter::release_adapters() {

        for (auto adapter_p : keyed_adapter_p_vector_) {

            adapter_p.reset();
        }

        unique_adapter_p_vector_.clear();
        keyed_adapter_p_vector_.clear();
    }

}