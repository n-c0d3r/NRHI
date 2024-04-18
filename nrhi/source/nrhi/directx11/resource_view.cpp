#include <nrhi/directx11/resource_view.hpp>
#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>



namespace nrhi {

    F_directx11_resource_view::F_directx11_resource_view(
        TK_valid<A_device> device_p,
        TK_valid<A_resource> resource_p,
        E_resource_view_type type,
        const F_resource_view_desc& desc,
        ID3D11View* d3d11_view_p
    ) :
        A_resource_view(device_p, resource_p, type, desc),
        d3d11_view_p_(d3d11_view_p)
    {

    }
    F_directx11_resource_view::~F_directx11_resource_view() {

        if(d3d11_view_p_)
            d3d11_view_p_->Release();
    }



    U_srv_handle HD_directx11_resource_view::create_srv(
        TK_valid<A_device> device_p,
        TK_valid<A_resource> resource_p,
        const F_resource_view_desc& desc
    ){

        const auto& resource_desc = resource_p->desc();

        NCPP_ASSERT(
            u32(resource_desc.bind_flags)
            & u32(E_resource_bind_flag::SRV)
        ) << "resource bind flag is not conpatible";

        ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();
        ID3D11Resource* d3d11_resource_p = resource_p.T_cast<F_directx11_resource>()->d3d11_resource_p();
        ID3D11View* d3d11_view_p = 0;

        D3D11_SHADER_RESOURCE_VIEW_DESC d3d11_srv_desc;
        switch (resource_desc.type) {
            case E_resource_type::BUFFER:
                if(resource_desc.stride) {
                    d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
                    d3d11_srv_desc.BufferEx.FirstElement = desc.mem_offset / resource_desc.stride;
                    d3d11_srv_desc.BufferEx.NumElements = resource_desc.width / resource_desc.stride;
                    d3d11_srv_desc.BufferEx.Flags = 0;
                }
                else{
                    d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
                    d3d11_srv_desc.Buffer.ElementOffset = 0;
                    d3d11_srv_desc.Buffer.ElementWidth = resource_desc.width;
                }
                d3d11_srv_desc.Format = DXGI_FORMAT_UNKNOWN;
                break;
            case E_resource_type::TEXTURE_1D:
                d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
                d3d11_srv_desc.Texture1D.MipLevels = resource_desc.mip_level_count;
                d3d11_srv_desc.Texture1D.MostDetailedMip = desc.base_mip_level;
                d3d11_srv_desc.Format = DXGI_FORMAT(resource_desc.format);
                break;
            case E_resource_type::TEXTURE_2D:
                d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                d3d11_srv_desc.Texture2D.MipLevels = resource_desc.mip_level_count;
                d3d11_srv_desc.Texture2D.MostDetailedMip = desc.base_mip_level;
                d3d11_srv_desc.Format = DXGI_FORMAT(resource_desc.format);
                break;
            case E_resource_type::TEXTURE_3D:
                d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
                d3d11_srv_desc.Texture3D.MipLevels = resource_desc.mip_level_count;
                d3d11_srv_desc.Texture3D.MostDetailedMip = desc.base_mip_level;
                d3d11_srv_desc.Format = DXGI_FORMAT(resource_desc.format);
                break;
        }

        d3d11_device_p->CreateShaderResourceView(
            d3d11_resource_p,
            &d3d11_srv_desc,
            (ID3D11ShaderResourceView**)&d3d11_view_p
        );

        auto result = TU<F_directx11_resource_view>()(
            device_p,
            resource_p,
            E_resource_view_type::SRV,
            desc,
            d3d11_view_p
        );

        return { std::move(result) };
    }
    U_uav_handle HD_directx11_resource_view::create_uav(
        TK_valid<A_device> device_p,
        TK_valid<A_resource> resource_p,
        const F_resource_view_desc& desc
    ){

        return {};
    }
    U_rtv_handle HD_directx11_resource_view::create_rtv(
        TK_valid<A_device> device_p,
        TK_valid<A_resource> resource_p,
        const F_resource_view_desc& desc
    ){

        return {};
    }
    U_dsv_handle HD_directx11_resource_view::create_dsv(
        TK_valid<A_device> device_p,
        TK_valid<A_resource> resource_p,
        const F_resource_view_desc& desc
    ){

        return {};
    }
    U_sampler_handle HD_directx11_resource_view::create_sampler(
        TK_valid<A_device> device_p,
        TK_valid<A_resource> resource_p,
        const F_resource_view_desc& desc
    ){

        return {};
    }

}