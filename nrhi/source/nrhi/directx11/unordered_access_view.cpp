#include <nrhi/directx11/unordered_access_view.hpp>
#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

	F_directx11_unordered_access_view::F_directx11_unordered_access_view(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		E_resource_view_type overrided_type
	) :
		F_directx11_resource_view(
			device_p,
			desc,
			overrided_type,
			create_d3d11_unordered_access_view(
				device_p,
				desc
			)
		)
	{
	}
	F_directx11_unordered_access_view::F_directx11_unordered_access_view(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		E_resource_view_type overrided_type,
		ID3D11UnorderedAccessView* d3d11_unordered_access_view_p
	) :
		F_directx11_resource_view(device_p, desc, overrided_type, d3d11_unordered_access_view_p)
	{

	}
	F_directx11_unordered_access_view::~F_directx11_unordered_access_view() {
	}

	ID3D11UnorderedAccessView* F_directx11_unordered_access_view::create_d3d11_unordered_access_view(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc
	) {

		auto resource_p = desc.resource_p;

		const auto& resource_desc = resource_p->desc();

		NCPP_ASSERT(resource_desc.can_create_view) << "resource can't be used to create view";

		NCPP_ASSERT(
			u32(resource_desc.bind_flags)
				& u32(E_resource_bind_flag::UAV)
		) << "resource bind flag is not conpatible";

		ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();
		ID3D11Resource* d3d11_resource_p = resource_p.T_cast<F_directx11_resource>()->d3d11_resource_p();
		ID3D11View* d3d11_view_p = 0;

		E_resource_type target_resource_type = desc.overrided_resource_type;
		if(target_resource_type == E_resource_type::NONE)
			target_resource_type = resource_desc.type;

		D3D11_UNORDERED_ACCESS_VIEW_DESC d3d11_uav_desc;
		memset(&d3d11_uav_desc, 0, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
		d3d11_uav_desc.Format = DXGI_FORMAT(resource_desc.format);
		switch (target_resource_type) {
		case E_resource_type::BUFFER:
			d3d11_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			d3d11_uav_desc.Buffer.FirstElement = desc.mem_offset / resource_desc.stride;
			d3d11_uav_desc.Buffer.NumElements = resource_desc.size / resource_desc.stride;
			break;
		case E_resource_type::STRUCTURED_BUFFER:
			d3d11_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			d3d11_uav_desc.Buffer.FirstElement = desc.mem_offset / resource_desc.stride;
			d3d11_uav_desc.Buffer.NumElements = resource_desc.size / resource_desc.stride;
			break;
		case E_resource_type::TEXTURE_1D:
			d3d11_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
			d3d11_uav_desc.Texture1D.MipSlice = desc.base_mip_level;
			break;
		case E_resource_type::TEXTURE_2D:
			d3d11_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			d3d11_uav_desc.Texture2D.MipSlice = desc.base_mip_level;
			break;
		case E_resource_type::TEXTURE_3D:
			d3d11_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
			d3d11_uav_desc.Texture3D.MipSlice = desc.base_mip_level;
			break;
		case E_resource_type::TEXTURE_2D_ARRAY:
			d3d11_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
			d3d11_uav_desc.Texture2DArray.MipSlice = desc.base_mip_level;
			d3d11_uav_desc.Texture2DArray.FirstArraySlice = desc.index;
			d3d11_uav_desc.Texture2DArray.ArraySize = desc.count;
			break;
		default:
			NCPP_ASSERT(false) << "invalid resource type";
			break;
		}

		d3d11_device_p->CreateUnorderedAccessView(
			d3d11_resource_p,
			&d3d11_uav_desc,
			(ID3D11UnorderedAccessView**)&d3d11_view_p
		);

		NCPP_ASSERT(d3d11_view_p) << "unordered access view creation failed";

		return (ID3D11UnorderedAccessView*)d3d11_view_p;
	}

	void F_directx11_unordered_access_view::rebuild() {

		if(d3d11_view_p_)
			d3d11_view_p_->Release();
		d3d11_view_p_ = create_d3d11_unordered_access_view(
			device_p(),
			desc()
		);

		finalize_rebuild();
	}

}