#include <nrhi/directx11/shader_resource_view.hpp>
#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

    F_directx11_shader_resource_view::F_directx11_shader_resource_view(
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc,
        ED_resource_view_type overrided_type
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
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc,
        ED_resource_view_type overrided_type,
        ID3D11ShaderResourceView* d3d11_shader_resource_view_p
    ) :
        F_directx11_resource_view(device_p, desc, overrided_type, d3d11_shader_resource_view_p)
    {

    }
    F_directx11_shader_resource_view::~F_directx11_shader_resource_view() {
    }

    ID3D11ShaderResourceView* F_directx11_shader_resource_view::create_d3d11_shader_resource_view(
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ) {

        auto resource_p = desc.resource_p;

        const auto& resource_desc = resource_p->desc();

        NCPP_ASSERT(resource_desc.can_create_view) << "resource can't be used to create view";

        NCPP_ASSERT(
			flag_is_has(
				resource_desc.flags,
				ED_resource_flag::SHADER_RESOURCE
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

    	sz target_element_count = desc.overrided_element_count;
    	if(target_element_count == 0)
    		target_element_count = resource_desc.element_count;

    	sz target_stride = desc.overrided_stride;
    	if(target_stride == 0)
    		target_stride = resource_desc.stride;

        D3D11_SHADER_RESOURCE_VIEW_DESC d3d11_srv_desc;
        memset(&d3d11_srv_desc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        d3d11_srv_desc.Format = DXGI_FORMAT(target_format);
		NRHI_ENUM_SWITCH(
			target_resource_type,
			NRHI_ENUM_CASE(
				ED_resource_type::BUFFER,
				d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
				d3d11_srv_desc.Buffer.FirstElement = desc.mem_offset / target_stride;
				d3d11_srv_desc.Buffer.NumElements = target_element_count;
				NRHI_ENUM_BREAK;
            )
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_1D,
				d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
				d3d11_srv_desc.Texture1D.MipLevels = resource_desc.mip_level_count;
				d3d11_srv_desc.Texture1D.MostDetailedMip = desc.base_mip_level;
				NRHI_ENUM_BREAK;
            )
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D,
				d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				d3d11_srv_desc.Texture2D.MipLevels = resource_desc.mip_level_count;
				d3d11_srv_desc.Texture2D.MostDetailedMip = desc.base_mip_level;
				NRHI_ENUM_BREAK;
            )
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_3D,
				d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
				d3d11_srv_desc.Texture3D.MipLevels = resource_desc.mip_level_count;
				d3d11_srv_desc.Texture3D.MostDetailedMip = desc.base_mip_level;
				NRHI_ENUM_BREAK;
            )
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D_ARRAY,
				if(
					flag_is_has(
						target_resource_flags,
						ED_resource_flag::TEXTURE_CUBE
					)
				) {
					d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
					d3d11_srv_desc.TextureCube.MipLevels = resource_desc.mip_level_count;
					d3d11_srv_desc.TextureCube.MostDetailedMip = desc.base_mip_level;
				}
				else {
					d3d11_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
					d3d11_srv_desc.Texture2DArray.MipLevels = resource_desc.mip_level_count;
					d3d11_srv_desc.Texture2DArray.FirstArraySlice = desc.first_array_slice;
					d3d11_srv_desc.Texture2DArray.ArraySize = target_array_size;
					d3d11_srv_desc.Texture2DArray.MostDetailedMip = desc.base_mip_level;
					NCPP_ASSERT(target_array_size) << "texture 2d array size can't be zero";
				}
				NRHI_ENUM_BREAK;
            )
			NRHI_ENUM_DEFAULT(
				NCPP_ASSERT(false) << "invalid resource type";
				NRHI_ENUM_BREAK;
			)
		);

        d3d11_device_p->CreateShaderResourceView(
            d3d11_resource_p,
            &d3d11_srv_desc,
            (ID3D11ShaderResourceView**)&d3d11_view_p
        );

        NCPP_ASSERT(d3d11_view_p) << "shader resource view creation failed";

        return (ID3D11ShaderResourceView*)d3d11_view_p;
    }

	void F_directx11_shader_resource_view::rebuild() {

		if(d3d11_view_p_)
			d3d11_view_p_->Release();
		d3d11_view_p_ = create_d3d11_shader_resource_view(
			device_p(),
			desc()
		);

		finalize_rebuild();
	}
	void F_directx11_shader_resource_view::rebuild(
		const F_resource_view_desc& desc
	) {
		if(d3d11_view_p_)
			d3d11_view_p_->Release();
		d3d11_view_p_ = create_d3d11_shader_resource_view(
			device_p(),
			desc
		);

		finalize_rebuild(
			desc
		);
	}

}