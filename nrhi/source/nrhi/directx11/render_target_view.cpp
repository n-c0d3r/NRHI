#include <nrhi/directx11/render_target_view.hpp>
#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

    F_directx11_render_target_view::F_directx11_render_target_view(
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc,
        ED_resource_view_type overrided_type
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
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc,
        ED_resource_view_type overrided_type,
        ID3D11RenderTargetView* d3d11_render_target_view_p
    ) :
        F_directx11_resource_view(device_p, desc, overrided_type, d3d11_render_target_view_p)
    {

    }
    F_directx11_render_target_view::~F_directx11_render_target_view() {
    }

    ID3D11RenderTargetView* F_directx11_render_target_view::create_d3d11_render_target_view(
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ) {

        auto resource_p = desc.resource_p;

        const auto& resource_desc = resource_p->desc();

        NCPP_ASSERT(resource_desc.can_create_view) << "resource can't be used to create view";

        NCPP_ASSERT(
			flag_is_has(
				resource_desc.flags,
				ED_resource_flag::RENDER_TARGET
			)
        ) << "resource bind flag is not conpatible";

        ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();
        ID3D11Resource* d3d11_resource_p = resource_p.T_cast<F_directx11_resource>()->d3d11_resource_p();
        ID3D11View* d3d11_view_p = 0;

		ED_resource_type target_resource_type = desc.overrided_resource_type;
		if(target_resource_type == ED_resource_type::NONE)
			target_resource_type = resource_desc.type;

		ED_resource_flag target_resource_flags = desc.overrided_resource_flags;
		if(target_resource_flags == ED_resource_flag::NONE)
			target_resource_flags = resource_desc.flags;

		u32 target_array_size = desc.overrided_array_size;
		if(!target_array_size)
			target_array_size = resource_desc.array_size;

		ED_format target_format = desc.overrided_format;
		if(target_format == ED_format::NONE)
			target_format = resource_desc.format;

        D3D11_RENDER_TARGET_VIEW_DESC d3d11_rtv_desc;
        memset(&d3d11_rtv_desc, 0, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
        d3d11_rtv_desc.Format = DXGI_FORMAT(target_format);
		NRHI_ENUM_SWITCH(
			target_resource_type,
			NRHI_ENUM_CASE(
                ED_resource_type::TEXTURE_2D,
				d3d11_rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				d3d11_rtv_desc.Texture2D.MipSlice = desc.target_mip_level;
				NRHI_ENUM_BREAK;
            )
			NRHI_ENUM_CASE(
                ED_resource_type::TEXTURE_2D_ARRAY,
				d3d11_rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				d3d11_rtv_desc.Texture2DArray.MipSlice = desc.base_mip_level;
				d3d11_rtv_desc.Texture2DArray.FirstArraySlice = desc.first_array_slice;
				d3d11_rtv_desc.Texture2DArray.ArraySize = target_array_size;
				NCPP_ASSERT(target_array_size) << "texture 2d array size can't be zero";
				NRHI_ENUM_BREAK;
            )
			NRHI_ENUM_DEFAULT(
				NCPP_ASSERT(false) << "invalid resource type";
				NRHI_ENUM_BREAK;
            )
		);

        d3d11_device_p->CreateRenderTargetView(
            d3d11_resource_p,
            &d3d11_rtv_desc,
            (ID3D11RenderTargetView**)&d3d11_view_p
        );

        NCPP_ASSERT(d3d11_view_p) << "render target view creation failed";

        return (ID3D11RenderTargetView*)d3d11_view_p;
    }

	void F_directx11_render_target_view::rebuild() {

		if(d3d11_view_p_)
			d3d11_view_p_->Release();
		d3d11_view_p_ = create_d3d11_render_target_view(
			device_p(),
			desc()
		);

		finalize_rebuild();
	}
	void F_directx11_render_target_view::rebuild(
		const F_resource_view_desc& desc
	) {
		if(d3d11_view_p_)
			d3d11_view_p_->Release();
		d3d11_view_p_ = create_d3d11_render_target_view(
			device_p(),
			desc
		);

		finalize_rebuild(
			desc
		);
	}

}