#include <nrhi/directx12/descriptor.hpp>
#include <nrhi/directx12/descriptor_heap.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/resource.hpp>



namespace nrhi {

	void HD_directx12_descriptor::initialize_resource_view(
		TKPA_valid<A_descriptor_heap> heap_p,
		F_descriptor_cpu_address cpu_address,
		const F_resource_view_desc& desc
	) {
		NRHI_ENUM_SWITCH(
			desc.type,
			NRHI_ENUM_CASE(
				ED_resource_view_type::SRV,
				initialize_srv(
					heap_p,
					cpu_address,
					desc
				);
			)
			NRHI_ENUM_CASE(
				ED_resource_view_type::UAV,
				initialize_uav(
					heap_p,
					cpu_address,
					desc
				);
			)
			NRHI_ENUM_CASE(
				ED_resource_view_type::RTV,
				initialize_rtv(
					heap_p,
					cpu_address,
					desc
				);
			)
			NRHI_ENUM_CASE(
				ED_resource_view_type::DSV,
				initialize_dsv(
					heap_p,
					cpu_address,
					desc
				);
			)
		);
	}

	void HD_directx12_descriptor::initialize_srv(
		TKPA_valid<A_descriptor_heap> heap_p,
		F_descriptor_cpu_address cpu_address,
		const F_resource_view_desc& desc
	) {
		auto d3d12_descriptor_heap_p = heap_p.T_cast<F_directx12_descriptor_heap>()->d3d12_descriptor_heap_p();
		auto d3d12_device_p = heap_p->device_p().T_cast<F_directx12_device>()->d3d12_device_p();

		auto resource_p = desc.resource_p;

		const auto& resource_desc = resource_p->desc();

		NCPP_ASSERT(resource_desc.can_create_view) << "resource can't be used to create view";

		NCPP_ASSERT(
			u32(resource_desc.bind_flags)
			& u32(ED_resource_bind_flag::SRV)
		) << "resource bind flag is not conpatible";

		ID3D12Resource* d3d12_resource_p = resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p();

		ED_resource_type target_resource_type = desc.overrided_resource_type;
		if(target_resource_type == ED_resource_type::NONE)
			target_resource_type = resource_desc.type;

		ED_format target_format = desc.overrided_format;
		if(target_format == ED_format::NONE)
			target_format = resource_desc.format;

		D3D12_SHADER_RESOURCE_VIEW_DESC d3d12_srv_desc;
		memset(&d3d12_srv_desc, 0, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));
		d3d12_srv_desc.Format = DXGI_FORMAT(target_format);
		NRHI_ENUM_SWITCH(
			target_resource_type,
			NRHI_ENUM_CASE(
				ED_resource_type::BUFFER,
				d3d12_srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
				d3d12_srv_desc.Buffer.FirstElement = desc.mem_offset / resource_desc.stride;
				d3d12_srv_desc.Buffer.NumElements = resource_desc.size / resource_desc.stride;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::STRUCTURED_BUFFER,
				d3d12_srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
				d3d12_srv_desc.Buffer.FirstElement = desc.mem_offset / resource_desc.stride;
				d3d12_srv_desc.Buffer.NumElements = resource_desc.size / resource_desc.stride;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_1D,
				d3d12_srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
				d3d12_srv_desc.Texture1D.MipLevels = resource_desc.mip_level_count;
				d3d12_srv_desc.Texture1D.MostDetailedMip = desc.base_mip_level;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D,
				d3d12_srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				d3d12_srv_desc.Texture2D.MipLevels = resource_desc.mip_level_count;
				d3d12_srv_desc.Texture2D.MostDetailedMip = desc.base_mip_level;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_3D,
				d3d12_srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
				d3d12_srv_desc.Texture3D.MipLevels = resource_desc.mip_level_count;
				d3d12_srv_desc.Texture3D.MostDetailedMip = desc.base_mip_level;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D_ARRAY,
				d3d12_srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				d3d12_srv_desc.Texture2DArray.MipLevels = resource_desc.mip_level_count;
				d3d12_srv_desc.Texture2DArray.FirstArraySlice = desc.index;
				d3d12_srv_desc.Texture2DArray.ArraySize = desc.count;
				d3d12_srv_desc.Texture2DArray.MostDetailedMip = desc.base_mip_level;
				NCPP_ASSERT(desc.count) << "texture 2d array size can't be zero";
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_CUBE,
				d3d12_srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
				d3d12_srv_desc.TextureCube.MipLevels = resource_desc.mip_level_count;
				d3d12_srv_desc.TextureCube.MostDetailedMip = desc.base_mip_level;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_DEFAULT(
				NCPP_ASSERT(false) << "invalid resource type";
				NRHI_ENUM_BREAK;
			)
		);

		d3d12_device_p->CreateShaderResourceView(
			d3d12_resource_p,
			&d3d12_srv_desc,
			D3D12_CPU_DESCRIPTOR_HANDLE(cpu_address)
		);
	}
	void HD_directx12_descriptor::initialize_uav(
		TKPA_valid<A_descriptor_heap> heap_p,
		F_descriptor_cpu_address cpu_address,
		const F_resource_view_desc& desc
	) {
	}
	void HD_directx12_descriptor::initialize_rtv(
		TKPA_valid<A_descriptor_heap> heap_p,
		F_descriptor_cpu_address cpu_address,
		const F_resource_view_desc& desc
	) {
	}
	void HD_directx12_descriptor::initialize_dsv(
		TKPA_valid<A_descriptor_heap> heap_p,
		F_descriptor_cpu_address cpu_address,
		const F_resource_view_desc& desc
	) {
	}

	void HD_directx12_descriptor::initialize_sampler_state(
		TKPA_valid<A_descriptor_heap> heap_p,
		F_descriptor_cpu_address cpu_address,
		const F_resource_view_desc& desc
	) {
	}

}