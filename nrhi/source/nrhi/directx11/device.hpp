#pragma once

/** @file nrhi/directx11/device.hpp
*
*   Implement directx11 device.
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

#include <nrhi/device_base.hpp>

#pragma endregion



namespace nrhi {

    class A_adapter;



    class NRHI_API F_directx11_device : public A_device {

    private:
        ID3D11Device* d3d11_device_p_ = 0;
        b8 is_d3d11_immediate_ctx_obtained_ = false;

    public:
        NCPP_FORCE_INLINE ID3D11Device* d3d11_device_p() noexcept { return d3d11_device_p_; }
		NCPP_FORCE_INLINE void set_d3d11_device_p_unsafe(ID3D11Device* value) noexcept { d3d11_device_p_ = value; }



    public:
        F_directx11_device(TKPA_valid<A_adapter> adapter_p);
        ~F_directx11_device();

    public:
        ID3D11DeviceContext* pop_d3d11_immediate_ctx();
        void push_d3d11_immediate_ctx();

    };



    class NRHI_API HD_directx11_device {

    public:
        static TU<A_device> create(TKPA_valid<A_adapter> adapter_p);

        static constexpr u32 max_queue_count(A_device*) { return 1; }

    };

}
