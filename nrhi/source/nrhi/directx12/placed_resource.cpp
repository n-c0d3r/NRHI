#include <nrhi/directx12/placed_resource.hpp>
#include <nrhi/directx12/resource.hpp>
#include <nrhi/directx12/resource_heap.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

	F_directx12_placed_resource::F_directx12_placed_resource(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset
	) :
		F_directx12_placed_resource(
			device_p,
			desc,
			heap_p,
			heap_offset,
			desc.type
		)
	{
	}
	F_directx12_placed_resource::F_directx12_placed_resource(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
		ED_resource_type overrided_type
	) :
		F_directx12_resource(
			device_p,
			desc,
			overrided_type,
			create_d3d12_placed_resource(
				device_p,
				desc,
				heap_p,
				heap_offset
			)
		)
	{
		set_placed_heap_p_unsafe(heap_p);
		set_placed_heap_offset_unsafe(heap_offset);
	}
	F_directx12_placed_resource::F_directx12_placed_resource(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset,
		ED_resource_type overrided_type,
		ID3D12Resource* d3d12_resource_p
	) :
		F_directx12_resource(device_p, desc, overrided_type, d3d12_resource_p)
	{
		set_placed_heap_p_unsafe(heap_p);
		set_placed_heap_offset_unsafe(heap_offset);
	}
	F_directx12_placed_resource::~F_directx12_placed_resource() {
	}

	ID3D12Resource* F_directx12_placed_resource::create_d3d12_placed_resource(
		TKPA_valid<A_device> device_p,
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset
	)
	{
		ID3D12Device* d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();
		ID3D12Heap* d3d12_resource_heap_p = heap_p.T_cast<F_directx12_resource_heap>()->d3d12_resource_heap_p();

		ID3D12Resource* d3d12_resource_p = 0;

		D3D12_RESOURCE_DESC d3d12_resource_desc;
		d3d12_resource_desc.Dimension = NRHI_DRIVER_DIRECTX_12_MAP___RESOURCE_TYPE___TO___RESOURCE_DIMENSION(desc.type);
		d3d12_resource_desc.Alignment = desc.alignment;
		d3d12_resource_desc.Width = eastl::max<u32>(1, desc.width);
		d3d12_resource_desc.Height = eastl::max<u32>(1, desc.height);
		d3d12_resource_desc.DepthOrArraySize = eastl::max<u32>(1, desc.depth);
		d3d12_resource_desc.MipLevels = eastl::max<u32>(1, desc.mip_level_count);
		d3d12_resource_desc.Format = DXGI_FORMAT(desc.format);
		d3d12_resource_desc.SampleDesc.Count = desc.sample_desc.count;
		d3d12_resource_desc.SampleDesc.Quality = desc.sample_desc.quality;
		d3d12_resource_desc.Layout = D3D12_TEXTURE_LAYOUT(desc.layout);
		d3d12_resource_desc.Flags = NRHI_DRIVER_DIRECTX_12_MAP___RESOURCE_FLAG___TO___RESOURCE_FLAG(desc.flags);

		if(desc.type == ED_resource_type::BUFFER)
		{
			d3d12_resource_desc.Format = DXGI_FORMAT_UNKNOWN;
			d3d12_resource_desc.Width = u64(desc.element_count) * u64(desc.stride);
		}

		D3D12_RESOURCE_STATES d3d12_resource_states = D3D12_RESOURCE_STATES(desc.initial_state);

		if(
			(d3d12_resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
			& !flag_is_has(desc.flags, ED_resource_flag::SHADER_RESOURCE)
		)
			d3d12_resource_desc.Flags &= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

		NCPP_ASSERT(
			(desc.heap_type != ED_resource_heap_type::GREAD_CWRITE)
			|| (
				(desc.heap_type == ED_resource_heap_type::GREAD_CWRITE)
				&& (d3d12_resource_states & D3D12_RESOURCE_STATE_GENERIC_READ)
			)
		) << "GENERIC_READ initial state is required for GREAD-CWRITE resource";

		D3D12_CLEAR_VALUE* d3d12_clear_value_p = 0;

		D3D12_CLEAR_VALUE d3d12_clear_value;
		if(
			flag_is_has(d3d12_resource_desc.Flags, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
			|| flag_is_has(d3d12_resource_desc.Flags, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
		)
		{
			d3d12_clear_value.Format = DXGI_FORMAT(desc.clear_value.format);
			if(d3d12_clear_value.Format == DXGI_FORMAT_UNKNOWN)
				d3d12_clear_value.Format = d3d12_resource_desc.Format;
			d3d12_clear_value.Color[0] = desc.clear_value.color.x;
			d3d12_clear_value.Color[1] = desc.clear_value.color.y;
			d3d12_clear_value.Color[2] = desc.clear_value.color.z;
			d3d12_clear_value.Color[3] = desc.clear_value.color.w;
			d3d12_clear_value.DepthStencil.Depth = desc.clear_value.depth;
			d3d12_clear_value.DepthStencil.Stencil = desc.clear_value.stencil;

			d3d12_clear_value_p = &d3d12_clear_value;
		}

		HRESULT hr = d3d12_device_p->CreatePlacedResource(
			d3d12_resource_heap_p,
			heap_offset,
			&d3d12_resource_desc,
			d3d12_resource_states,
			d3d12_clear_value_p,
			IID_PPV_ARGS(&d3d12_resource_p)
		);

		NCPP_ASSERT(d3d12_resource_p) << "can't create d3d12 placed resource";

		return d3d12_resource_p;
	}

	void F_directx12_placed_resource::rebuild_placed(
		const F_resource_desc& desc,
		TKPA_valid<A_resource_heap> heap_p,
		u64 heap_offset
	) {
		if(d3d12_resource_p_)
			d3d12_resource_p_->Release();
		d3d12_resource_p_ = create_d3d12_placed_resource(
			device_p(),
			desc,
			heap_p,
			heap_offset
		);
		finalize_rebuild_placed(
			desc,
			heap_p,
			heap_offset
		);
	}

	E_resource_management_type F_directx12_placed_resource::management_type() const
	{
		return E_resource_management_type::PLACED;
	}

}