#pragma once

/** @file nrhi/directx11/resource_view.hpp
*
*   Implement directx11 resource view.
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

#include <nrhi/resource_view_base.hpp>
#include <nrhi/texture.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;



    class NRHI_API F_directx11_resource_view : public A_resource_view {

    private:
        ID3D11View* d3d11_view_p_ = 0;

    public:
        NCPP_FORCE_INLINE ID3D11View* d3d11_view_p() noexcept { return d3d11_view_p_; }
        NCPP_FORCE_INLINE void set_d3d11_view_p(ID3D11View* value) noexcept {
            d3d11_view_p_ = value;
        }

    public:
        F_directx11_resource_view(
            TK_valid<A_device> device_p,
            const F_resource_view_desc& desc,
            E_resource_view_type overrided_type
        );
        F_directx11_resource_view(
            TK_valid<A_device> device_p,
            const F_resource_view_desc& desc,
            E_resource_view_type overrided_type,
            ID3D11View* d3d11_view_p
        );
        ~F_directx11_resource_view();

    };



    class NRHI_API HD_directx11_resource_view {

    public:
        static TU<A_resource_view> create(
            TK_valid<A_device> device_p,
            const F_resource_view_desc& desc
        );

    public:
        static U_srv_handle create_srv(
            TK_valid<A_device> device_p,
            const F_resource_view_desc& desc
        );
        static U_uav_handle create_uav(
            TK_valid<A_device> device_p,
            const F_resource_view_desc& desc
        );
        static U_rtv_handle create_rtv(
            TK_valid<A_device> device_p,
            const F_resource_view_desc& desc
        );
        static U_dsv_handle create_dsv(
            TK_valid<A_device> device_p,
            const F_resource_view_desc& desc
        );
        static U_sampler_handle create_sampler(
            TK_valid<A_device> device_p,
            const F_resource_view_desc& desc
        );

    };

}