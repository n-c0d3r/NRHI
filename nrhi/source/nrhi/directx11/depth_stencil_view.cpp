#include <nrhi/directx11/depth_stencil_view.hpp>
#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

	F_directx11_depth_stencil_view::F_directx11_depth_stencil_view(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		E_resource_view_type overrided_type
	) :
		F_directx11_resource_view(
			device_p,
			desc,
			overrided_type,
			create_d3d11_depth_stencil_view(
				device_p,
				desc
			)
		)
	{
	}
	F_directx11_depth_stencil_view::F_directx11_depth_stencil_view(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		E_resource_view_type overrided_type,
		ID3D11DepthStencilView* d3d11_depth_stencil_view_p
	) :
		F_directx11_resource_view(device_p, desc, overrided_type, d3d11_depth_stencil_view_p)
	{

	}
	F_directx11_depth_stencil_view::~F_directx11_depth_stencil_view() {
	}

	ID3D11DepthStencilView* F_directx11_depth_stencil_view::create_d3d11_depth_stencil_view(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc
	) {

		auto resource_p = desc.resource_p;

		const auto& resource_desc = resource_p->desc();

		NCPP_ASSERT(resource_desc.can_create_view) << "resource can't be used to create view";

		NCPP_ASSERT(
			u32(resource_desc.bind_flags)
			& u32(E_resource_bind_flag::DSV)
		) << "resource bind flag is not conpatible";

		ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();
		ID3D11Resource* d3d11_resource_p = resource_p.T_cast<F_directx11_resource>()->d3d11_resource_p();
		ID3D11View* d3d11_view_p = 0;

		E_resource_type target_resource_type = desc.overrided_resource_type;
		if(target_resource_type == E_resource_type::NONE)
			target_resource_type = resource_desc.type;

		NCPP_ASSERT(target_resource_type == E_resource_type::TEXTURE_2D) << "only support texture 2d";

		E_format target_format = desc.overrided_format;
		if(target_format == E_format::NONE)
			target_format = resource_desc.format;

		D3D11_DEPTH_STENCIL_VIEW_DESC d3d11_dsv_desc;
		memset(&d3d11_dsv_desc, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		d3d11_dsv_desc.Format = DXGI_FORMAT(target_format);
		d3d11_dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		d3d11_dsv_desc.Texture2D.MipSlice = desc.target_mip_level;

		d3d11_device_p->CreateDepthStencilView(
			d3d11_resource_p,
			&d3d11_dsv_desc,
			(ID3D11DepthStencilView**)&d3d11_view_p
		);

		NCPP_ASSERT(d3d11_view_p) << "depth stencil view creation failed";

		return (ID3D11DepthStencilView*)d3d11_view_p;
	}

	void F_directx11_depth_stencil_view::rebuild() {

		if(d3d11_view_p_)
			d3d11_view_p_->Release();
		d3d11_view_p_ = create_d3d11_depth_stencil_view(
			device_p(),
			desc()
		);

		finalize_rebuild();
	}

}