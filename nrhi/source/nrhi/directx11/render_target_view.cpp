#include <nrhi/directx11/render_target_view.hpp>
#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

    F_directx11_render_target_view::F_directx11_render_target_view(
        TK_valid<A_device> device_p,
        const F_resource_view_desc& desc,
        E_resource_view_type overrided_type
    ) :
        F_directx11_resource_view(
            device_p,
            desc,
            overrided_type,
            create_d3d11_render_target_view(
                device_p,
                desc
            )
        )
    {
    }
    F_directx11_render_target_view::F_directx11_render_target_view(
        TK_valid<A_device> device_p,
        const F_resource_view_desc& desc,
        E_resource_view_type overrided_type,
        ID3D11RenderTargetView* d3d11_render_target_view_p
    ) :
        F_directx11_resource_view(device_p, desc, overrided_type, d3d11_render_target_view_p)
    {

    }
    F_directx11_render_target_view::~F_directx11_render_target_view() {
    }

    ID3D11RenderTargetView* F_directx11_render_target_view::create_d3d11_render_target_view(
        TK_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ) {

        auto resource_p = desc.resource_p;

        const auto& resource_desc = resource_p->desc();

        NCPP_ASSERT(resource_desc.can_create_view) << "resource can't be used to create view";

        NCPP_ASSERT(
            u32(resource_desc.bind_flags)
            & u32(E_resource_bind_flag::RTV)
        ) << "resource bind flag is not conpatible";

        ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();
        ID3D11Resource* d3d11_resource_p = resource_p.T_cast<F_directx11_resource>()->d3d11_resource_p();
        ID3D11View* d3d11_view_p = 0;

        NCPP_ASSERT(resource_desc.type == E_resource_type::TEXTURE_2D) << "only support texture 2d";

        D3D11_RENDER_TARGET_VIEW_DESC d3d11_rtv_desc;
        memset(&d3d11_rtv_desc, 0, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
        d3d11_rtv_desc.Format = DXGI_FORMAT(resource_desc.format);
        d3d11_rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        d3d11_rtv_desc.Texture2D.MipSlice = desc.target_mip_level;

        d3d11_device_p->CreateRenderTargetView(
            d3d11_resource_p,
            &d3d11_rtv_desc,
            (ID3D11RenderTargetView**)&d3d11_view_p
        );

        NCPP_ASSERT(d3d11_view_p) << "render target view creation failed";

        return (ID3D11RenderTargetView*)d3d11_view_p;
    }

}