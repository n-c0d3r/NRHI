#include <nrhi/dxgi/adapter.hpp>
#include <nrhi/dxgi/factory.hpp>



namespace nrhi {

    F_dxgi_adapter::F_dxgi_adapter(u32 index, IDXGIAdapter* dxgi_adapter_p) :
        A_adapter(index),
        dxgi_adapter_p_(dxgi_adapter_p)
    {

    }
    F_dxgi_adapter::~F_dxgi_adapter(){

    }



    TG_vector<TU_valid<A_adapter>> HD_dxgi_adapter::unique_adapter_p_vector_;
    TG_vector<TK_valid<A_adapter>> HD_dxgi_adapter::keyed_adapter_p_vector_;



    TG_vector<TK_valid<A_adapter>> HD_dxgi_adapter::enumerate_adapters() {

        return keyed_adapter_p_vector_;
    }



    void HD_dxgi_adapter::initialize_adapters() {

        auto* factory_p = F_dxgi_factory_helper::factory_p();
        IDXGIAdapter* dxgi_adapter_p = 0;

        for (UINT i = 0; factory_p->EnumAdapters(i, &dxgi_adapter_p) != DXGI_ERROR_NOT_FOUND; ++i) {

            auto adapter_p = TU_valid<F_dxgi_adapter>::T_make(i, dxgi_adapter_p);

            keyed_adapter_p_vector_.push_back(adapter_p.keyed());
            unique_adapter_p_vector_.push_back(std::move(adapter_p));
        }

    }
    void HD_dxgi_adapter::release_adapters() {

        for (auto adapter_p : keyed_adapter_p_vector_) {

            adapter_p.reset();
        }

        unique_adapter_p_vector_.clear();
        keyed_adapter_p_vector_.clear();
    }

}