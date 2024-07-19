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

		D3D12_SHADER_RESOURCE_VIEW_DESC d3d12_srv_desc = {};
		d3d12_srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
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
		HRESULT hr = d3d12_device_p->GetDeviceRemovedReason();
		NCPP_ASSERT(!FAILED(hr)) << "can't initialize srv descriptor";
	}
	void HD_directx12_descriptor::initialize_uav(
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
				& u32(ED_resource_bind_flag::UAV)
		) << "resource bind flag is not conpatible";

		ID3D12Resource* d3d12_resource_p = resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p();

		ED_resource_type target_resource_type = desc.overrided_resource_type;
		if(target_resource_type == ED_resource_type::NONE)
			target_resource_type = resource_desc.type;

		ED_format target_format = desc.overrided_format;
		if(target_format == ED_format::NONE)
			target_format = resource_desc.format;

		D3D12_UNORDERED_ACCESS_VIEW_DESC d3d12_uav_desc = {};
		d3d12_uav_desc.Format = DXGI_FORMAT(target_format);
		NRHI_ENUM_SWITCH(
			target_resource_type,
			NRHI_ENUM_CASE(
				ED_resource_type::BUFFER,
				d3d12_uav_desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
				d3d12_uav_desc.Buffer.FirstElement = desc.mem_offset / resource_desc.stride;
				d3d12_uav_desc.Buffer.NumElements = resource_desc.size / resource_desc.stride;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::STRUCTURED_BUFFER,
				d3d12_uav_desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
				d3d12_uav_desc.Buffer.FirstElement = desc.mem_offset / resource_desc.stride;
				d3d12_uav_desc.Buffer.NumElements = resource_desc.size / resource_desc.stride;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_1D,
				d3d12_uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
				d3d12_uav_desc.Texture1D.MipSlice = desc.base_mip_level;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D,
				d3d12_uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
				d3d12_uav_desc.Texture2D.MipSlice = desc.base_mip_level;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_3D,
				d3d12_uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
				d3d12_uav_desc.Texture3D.MipSlice = desc.base_mip_level;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D_ARRAY,
				d3d12_uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
				d3d12_uav_desc.Texture2DArray.MipSlice = desc.base_mip_level;
				d3d12_uav_desc.Texture2DArray.FirstArraySlice = desc.index;
				d3d12_uav_desc.Texture2DArray.ArraySize = desc.count;
				NCPP_ASSERT(desc.count) << "texture 2d array size can't be zero";
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_DEFAULT(
				NCPP_ASSERT(false) << "invalid resource type";
				NRHI_ENUM_BREAK;
			)
		);

		d3d12_device_p->CreateUnorderedAccessView(
			d3d12_resource_p,
			0,
			&d3d12_uav_desc,
			D3D12_CPU_DESCRIPTOR_HANDLE(cpu_address)
		);
		HRESULT hr = d3d12_device_p->GetDeviceRemovedReason();
		NCPP_ASSERT(!FAILED(hr)) << "can't initialize uav descriptor";
	}
	void HD_directx12_descriptor::initialize_rtv(
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
				& u32(ED_resource_bind_flag::RTV)
		) << "resource bind flag is not conpatible";

		ID3D12Resource* d3d12_resource_p = resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p();

		ED_resource_type target_resource_type = desc.overrided_resource_type;
		if(target_resource_type == ED_resource_type::NONE)
			target_resource_type = resource_desc.type;

		ED_format target_format = desc.overrided_format;
		if(target_format == ED_format::NONE)
			target_format = resource_desc.format;

		D3D12_RENDER_TARGET_VIEW_DESC d3d12_rtv_desc = {};
		d3d12_rtv_desc.Format = DXGI_FORMAT(target_format);
		NRHI_ENUM_SWITCH(
			target_resource_type,
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D,
				d3d12_rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
				d3d12_rtv_desc.Texture2D.MipSlice = desc.target_mip_level;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D_ARRAY,
				d3d12_rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
				d3d12_rtv_desc.Texture2DArray.MipSlice = desc.base_mip_level;
				d3d12_rtv_desc.Texture2DArray.FirstArraySlice = desc.index;
				d3d12_rtv_desc.Texture2DArray.ArraySize = desc.count;
				NCPP_ASSERT(desc.count) << "texture 2d array size can't be zero";
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_DEFAULT(
				NCPP_ASSERT(false) << "invalid resource type";
				NRHI_ENUM_BREAK;
			)
		);

		d3d12_device_p->CreateRenderTargetView(
			d3d12_resource_p,
			&d3d12_rtv_desc,
			D3D12_CPU_DESCRIPTOR_HANDLE(cpu_address)
		);
		HRESULT hr = d3d12_device_p->GetDeviceRemovedReason();
		NCPP_ASSERT(!FAILED(hr)) << "can't initialize rtv descriptor";
	}
	void HD_directx12_descriptor::initialize_dsv(
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
				& u32(ED_resource_bind_flag::DSV)
		) << "resource bind flag is not conpatible";

		ID3D12Resource* d3d12_resource_p = resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p();

		ED_resource_type target_resource_type = desc.overrided_resource_type;
		if(target_resource_type == ED_resource_type::NONE)
			target_resource_type = resource_desc.type;

		ED_format target_format = desc.overrided_format;
		if(target_format == ED_format::NONE)
			target_format = resource_desc.format;

		D3D12_DEPTH_STENCIL_VIEW_DESC d3d12_dsv_desc = {};
		d3d12_dsv_desc.Format = DXGI_FORMAT(target_format);
		NRHI_ENUM_SWITCH(
			target_resource_type,
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D,
				d3d12_dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				d3d12_dsv_desc.Texture2D.MipSlice = desc.target_mip_level;
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_resource_type::TEXTURE_2D_ARRAY,
				d3d12_dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
				d3d12_dsv_desc.Texture2DArray.MipSlice = desc.base_mip_level;
				d3d12_dsv_desc.Texture2DArray.FirstArraySlice = desc.index;
				d3d12_dsv_desc.Texture2DArray.ArraySize = desc.count;
				NCPP_ASSERT(desc.count) << "texture 2d array size can't be zero";
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_DEFAULT(
				NCPP_ASSERT(false) << "invalid resource type";
				NRHI_ENUM_BREAK;
			)
		);

		d3d12_device_p->CreateDepthStencilView(
			d3d12_resource_p,
			&d3d12_dsv_desc,
			D3D12_CPU_DESCRIPTOR_HANDLE(cpu_address)
		);
		HRESULT hr = d3d12_device_p->GetDeviceRemovedReason();
		NCPP_ASSERT(!FAILED(hr)) << "can't initialize dsv descriptor";
	}

	void HD_directx12_descriptor::initialize_sampler_state(
		TKPA_valid<A_descriptor_heap> heap_p,
		F_descriptor_cpu_address cpu_address,
		const F_sampler_state_desc& desc
	) {
		auto d3d12_descriptor_heap_p = heap_p.T_cast<F_directx12_descriptor_heap>()->d3d12_descriptor_heap_p();
		auto d3d12_device_p = heap_p->device_p().T_cast<F_directx12_device>()->d3d12_device_p();

		D3D12_SAMPLER_DESC d3d12_sampler_state_desc;
		memset(&d3d12_sampler_state_desc, 0, sizeof(D3D12_SAMPLER_DESC));
		d3d12_sampler_state_desc.Filter = D3D12_FILTER(desc.filter);
		d3d12_sampler_state_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE(desc.texcoord_address_modes[0]);
		d3d12_sampler_state_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE(desc.texcoord_address_modes[1]);
		d3d12_sampler_state_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE(desc.texcoord_address_modes[2]);
		d3d12_sampler_state_desc.MipLODBias = desc.lod_offset;
		d3d12_sampler_state_desc.MinLOD = desc.min_lod;
		d3d12_sampler_state_desc.MaxLOD = desc.max_lod;

		d3d12_device_p->CreateSampler(
			&d3d12_sampler_state_desc,
			D3D12_CPU_DESCRIPTOR_HANDLE(cpu_address)
		);
		HRESULT hr = d3d12_device_p->GetDeviceRemovedReason();
		NCPP_ASSERT(!FAILED(hr)) << "can't initialize sampler state descriptor";
	}

	void HD_directx12_descriptor::copy_descriptor_ranges(
		TKPA_valid<A_device> device_p,
		TG_span<F_descriptor_cpu_address> dest_cpu_addresses,
		TG_span<F_descriptor_cpu_address> src_cpu_addresses,
		TG_span<u32> descriptor_counts,
		u32 range_count,
		ED_descriptor_heap_type heap_type
	) {
		auto d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();

		const u32* descriptor_counts_p = descriptor_counts.data();

		d3d12_device_p->CopyDescriptors(
			range_count,
			(const D3D12_CPU_DESCRIPTOR_HANDLE*)(dest_cpu_addresses.data()),
			descriptor_counts_p,
			range_count,
			(const D3D12_CPU_DESCRIPTOR_HANDLE*)(src_cpu_addresses.data()),
			descriptor_counts_p,
			D3D12_DESCRIPTOR_HEAP_TYPE(heap_type)
		);
	}
	void HD_directx12_descriptor::copy_descriptors(
		TKPA_valid<A_device> device_p,
		F_descriptor_cpu_address dest_cpu_address,
		F_descriptor_cpu_address src_cpu_address,
		u32 descriptor_count,
		ED_descriptor_heap_type heap_type
	) {
		auto d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();

		d3d12_device_p->CopyDescriptorsSimple(
			descriptor_count,
			D3D12_CPU_DESCRIPTOR_HANDLE(dest_cpu_address),
			D3D12_CPU_DESCRIPTOR_HANDLE(src_cpu_address),
			D3D12_DESCRIPTOR_HEAP_TYPE(heap_type)
		);
	}

}