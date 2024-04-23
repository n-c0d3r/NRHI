#include <nrhi/directx11/shader_resource_view.hpp>
#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

    F_directx11_shader_resource_view::F_directx11_shader_resource_view(
        TK_valid<A_device> device_p,
        const F_resource_view_desc& desc,
        E_resource_view_type overrided_type
    ) :
        F_directx11_resource_view(
            device_p,
            desc,
            overrided_type,
            create_d3d11_shader_resource_view(
                device_p,
                desc
            )
        )
    {
    }
    F_directx11_shader_resource_view::F_directx11_shader_resource_view(
        TK_valid<A_device> device_p,
        const F_resource_view_desc& desc,
        E_resource_view_type overrided_type,
        ID3D11ShaderResourceView* d3d11_shader_resource_view_p
    ) :
        F_directx11_resource_view(device_p, desc, overrided_type, d3d11_shader_resource_view_p)
    {

    }
    F_directx11_shader_resource_view::~F_directx11_shader_resource_view() {
    }

    ID3D11ShaderResourceView* F_directx11_shader_resource_view::create_d3d11_shader_resource_view(
        TK_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ) {

        auto resource_p = desc.resource_p;

        const auto& resource_desc = resource_p->desc();

        NCPP_ASSERT(resource_desc.can_create_view) << "resource can't be used to create view";

        NCPP_ASSERT(
            u32(resource_desc.bind_flags)
            & u32(E_resource_bind_flag::SRV)
        ) << "resource bind flag is not conpatible";

        ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();
        ID3D11Resource* d3d11_resource_p = resource_p.T_cast<F_directx11_resource>()->d3d11_resource_p();
        ID3D11View* d3d11_view_p = 0;

        D3D11_SHADER_RESOURCE_VIEW_DESC d3d11_srv_desc;
        memset(&d3d11_srv_desc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        d3d11_srv_desc.Format = DXGI_FORMAT(resource_desc.format);
        switch (resource_desc.type) {
            case E_resource_type::BUFFER:
                d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
                d3d11_srv_desc.Buffer.FirstElement = desc.mem_offset / resource_desc.stride;
                d3d11_srv_desc.Buffer.NumElements = resource_desc.width / resource_desc.stride;
                break;
            case E_resource_type::STRUCTURED_BUFFER:
                d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
                d3d11_srv_desc.Buffer.FirstElement = desc.mem_offset / resource_desc.stride;
                d3d11_srv_desc.Buffer.NumElements = resource_desc.width / resource_desc.stride;
                break;
            case E_resource_type::TEXTURE_1D:
                d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
                d3d11_srv_desc.Texture1D.MipLevels = resource_desc.mip_level_count;
                d3d11_srv_desc.Texture1D.MostDetailedMip = desc.base_mip_level;
                break;
            case E_resource_type::TEXTURE_2D:
                d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                d3d11_srv_desc.Texture2D.MipLevels = resource_desc.mip_level_count;
                d3d11_srv_desc.Texture2D.MostDetailedMip = desc.base_mip_level;
                break;
            case E_resource_type::TEXTURE_3D:
                d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
                d3d11_srv_desc.Texture3D.MipLevels = resource_desc.mip_level_count;
                d3d11_srv_desc.Texture3D.MostDetailedMip = desc.base_mip_level;
                break;
        }

        d3d11_device_p->CreateShaderResourceView(
            d3d11_resource_p,
            &d3d11_srv_desc,
            (ID3D11ShaderResourceView**)&d3d11_view_p
        );

        NCPP_ASSERT(d3d11_view_p) << "shader resource view creation failed";

        return (ID3D11ShaderResourceView*)d3d11_view_p;
    }

}