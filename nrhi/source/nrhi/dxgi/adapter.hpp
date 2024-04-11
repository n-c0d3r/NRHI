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

#pragma endregion



namespace nrhi {

    namespace internal {

        extern NRHI_API IDXGIFactory* dxgi_factory_p;

    }



    class NRHI_API F_dxgi_adapter : public A_adapter {

    public:
        friend class HD_dxgi_adapter;



    private:
        IDXGIAdapter* dxgi_adapter_p_;

    public:
        NCPP_FORCE_INLINE IDXGIAdapter* dxgi_adapter_p() noexcept { return dxgi_adapter_p_; }



    public:
        F_dxgi_adapter(u32 index, IDXGIAdapter* dxgi_adapter_p);
        ~F_dxgi_adapter();

    };



    class NRHI_API HD_dxgi_adapter {

    public:
        friend class F_dxgi_factory_helper;



    private:
        static TG_vector<TU<A_adapter>> unique_adapter_p_vector_;
        static TG_vector<TK_valid<A_adapter>> keyed_adapter_p_vector_;



    public:
        static TG_vector<TK_valid<A_adapter>> adapters();



    private:
        static void initialize_adapters();
        static void release_adapters();

    };

}
