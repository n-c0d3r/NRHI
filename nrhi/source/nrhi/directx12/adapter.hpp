#pragma once

/** @file nrhi/dxgi/adapter.hpp
*
*   Implement dxgi adapter.
*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#pragma region Includes

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/prerequisites.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/adapter_base.hpp>
#include <nrhi/gpu_memory_info.hpp>

#pragma endregion



namespace nrhi {

    namespace internal {

        extern NRHI_API IDXGIFactory* dxgi_factory_p;

    }



    class NRHI_API F_directx12_adapter : public A_adapter {

    public:
        friend class HD_directx12_adapter;



    private:
        IDXGIAdapter* dxgi_adapter_p_;

    public:
        NCPP_FORCE_INLINE IDXGIAdapter* dxgi_adapter_p() noexcept { return dxgi_adapter_p_; }
		NCPP_FORCE_INLINE void set_dxgi_adapter_p_unsafe(IDXGIAdapter* value) noexcept { dxgi_adapter_p_ = value; }



    public:
        F_directx12_adapter(u32 index, IDXGIAdapter* dxgi_adapter_p);
        ~F_directx12_adapter();

    };



    class NRHI_API HD_directx12_adapter {

    public:
        friend class F_directx12_factory_helper;



    private:
        static TG_vector<TU<A_adapter>> unique_adapter_p_vector_;
        static TG_vector<TK_valid<A_adapter>> keyed_adapter_p_vector_;



    public:
        static const TG_vector<TK_valid<A_adapter>>& adapter_p_vector();



    private:
        static void initialize_adapters();
        static void release_adapters();

    public:
        static F_gpu_memory_info gpu_memory_info(
            TKPA_valid<A_adapter> adapter_p
        );
    };
}
