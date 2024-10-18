#include <nrhi/directx12/adapter.hpp>
#include <nrhi/directx12/factory.hpp>



namespace nrhi {

    F_directx12_adapter::F_directx12_adapter(u32 index, IDXGIAdapter* dxgi_adapter_p) :
        A_adapter(index),
        dxgi_adapter_p_(dxgi_adapter_p)
    {

    }
	F_directx12_adapter::~F_directx12_adapter(){

    }



    TG_vector<TU<A_adapter>> HD_directx12_adapter::unique_adapter_p_vector_;
    TG_vector<TK_valid<A_adapter>> HD_directx12_adapter::keyed_adapter_p_vector_;



    const TG_vector<TK_valid<A_adapter>>& HD_directx12_adapter::adapter_p_vector() {

        return keyed_adapter_p_vector_;
    }



    void HD_directx12_adapter::initialize_adapters() {

        auto* factory_p = F_directx12_factory_helper::factory_p();
        IDXGIAdapter* dxgi_adapter_p = 0;

		for (UINT i = 0; factory_p->EnumAdapters(i, &dxgi_adapter_p) != DXGI_ERROR_NOT_FOUND; ++i) {

            auto adapter_p = TU<F_directx12_adapter>()(i, dxgi_adapter_p);

            keyed_adapter_p_vector_.push_back(
                NCPP_FOREF_VALID(adapter_p)
            );
            unique_adapter_p_vector_.push_back(std::move(adapter_p));
        }
    }
    void HD_directx12_adapter::release_adapters() {

        for (auto adapter_p : keyed_adapter_p_vector_) {

            adapter_p.reset();
        }

        unique_adapter_p_vector_.clear();
        keyed_adapter_p_vector_.clear();
    }

    F_gpu_memory_info HD_directx12_adapter::gpu_memory_info(
        TKPA_valid<A_adapter> adapter_p
    )
    {
        auto dxgi_adapter_p = adapter_p.T_cast<F_directx12_adapter>()->dxgi_adapter_p();

        IDXGIAdapter3* dxgi_adapter_3_p = 0;
        dxgi_adapter_p->QueryInterface(IID_PPV_ARGS(&dxgi_adapter_3_p));
        NCPP_ASSERT(dxgi_adapter_3_p) << "not supported";

        DXGI_QUERY_VIDEO_MEMORY_INFO dxgi_gpu_memory_info;
        HRESULT hr = dxgi_adapter_3_p->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &dxgi_gpu_memory_info);
        NCPP_ASSERT(SUCCEEDED(hr)) << "can't query gpu memory info";

        return {
            .budget = dxgi_gpu_memory_info.Budget,
            .current_usage = dxgi_gpu_memory_info.CurrentUsage
        };
    }
}