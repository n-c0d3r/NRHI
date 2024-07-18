#pragma once

/** @file nrhi/directx11/render_target_view.hpp
*
*   Implement directx11 render_target_view.
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

#include <nrhi/directx11/resource_view.hpp>

#pragma endregion



namespace nrhi {

    class A_device;



    class NRHI_API F_directx11_render_target_view : public F_directx11_resource_view {

    public:
        F_directx11_render_target_view(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc,
            ED_resource_view_type overrided_type = ED_resource_view_type::RTV
        );
        F_directx11_render_target_view(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc,
            ED_resource_view_type overrided_type,
            ID3D11RenderTargetView* d3d11_render_target_view_p
        );
        virtual ~F_directx11_render_target_view();

    private:
        static ID3D11RenderTargetView* create_d3d11_render_target_view(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc
        );

	public:
		virtual void rebuild() override;
		virtual void rebuild(
			const F_resource_view_desc& desc
		) override;

    };

}
