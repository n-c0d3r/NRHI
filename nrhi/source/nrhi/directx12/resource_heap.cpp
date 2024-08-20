#include <nrhi/directx12/resource_heap.hpp>
#include <nrhi/directx12/device.hpp>



namespace nrhi {

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	void F_directx12_resource_heap::set_debug_name(const F_debug_name& value)
	{
		A_render_object::set_debug_name(value);

		auto wvalue = G_to_wstring(value.c_str());
		d3d12_resource_heap_p_->SetName(wvalue.data());
	}
#endif



	F_directx12_resource_heap::F_directx12_resource_heap(
		TKPA_valid<A_device> device_p,
		const F_resource_heap_desc& desc
	) :
		A_resource_heap(device_p, desc),
		d3d12_resource_heap_p_(
			create_d3d12_resource_heap(
				device_p,
				desc
			)
		)
	{
	}
	F_directx12_resource_heap::F_directx12_resource_heap(
		TKPA_valid<A_device> device_p,
		const F_resource_heap_desc& desc,
		ID3D12Heap* d3d12_resource_heap_p
	) :
		A_resource_heap(device_p, desc),
		d3d12_resource_heap_p_(d3d12_resource_heap_p)
	{
	}
	F_directx12_resource_heap::~F_directx12_resource_heap() {

		if(d3d12_resource_heap_p_)
			d3d12_resource_heap_p_->Release();
	}

	ID3D12Heap* F_directx12_resource_heap::create_d3d12_resource_heap(
		TKPA_valid<A_device> device_p,
		const F_resource_heap_desc& desc
	) {
		ID3D12Device* d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();

		ID3D12Heap* d3d12_resource_heap_p = 0;

		CD3DX12_HEAP_PROPERTIES d3d12_heap_properties(D3D12_HEAP_TYPE(desc.type));

		D3D12_HEAP_DESC d3d12_heap_desc;
		d3d12_heap_desc.SizeInBytes = desc.size;
		d3d12_heap_desc.Properties = d3d12_heap_properties;
		d3d12_heap_desc.Alignment = desc.alignment;
		d3d12_heap_desc.Flags = D3D12_HEAP_FLAGS(desc.flags);

		d3d12_device_p->CreateHeap(
			&d3d12_heap_desc,
			IID_PPV_ARGS(&d3d12_resource_heap_p)
		);
		NCPP_ASSERT(d3d12_resource_heap_p) << "can't create d3d12 resource heap";

		return d3d12_resource_heap_p;
	}

	void F_directx12_resource_heap::rebuild(
		const F_resource_heap_desc& desc
	) {
		if(d3d12_resource_heap_p_)
			d3d12_resource_heap_p_->Release();
		d3d12_resource_heap_p_ = create_d3d12_resource_heap(
			device_p(),
			desc
		);
		finalize_rebuild(desc);
	}

	void F_directx12_resource_heap::release_driver_specific_implementation()
	{
		if(d3d12_resource_heap_p_)
		{
			d3d12_resource_heap_p_->Release();
			d3d12_resource_heap_p_ = 0;
		}
	}



	TU<A_resource_heap> HD_directx12_resource_heap::create(
		TKPA_valid<A_device> device_p,
		const F_resource_heap_desc& desc
	){
		return TU<F_directx12_resource_heap>()(device_p, desc);
	}

}