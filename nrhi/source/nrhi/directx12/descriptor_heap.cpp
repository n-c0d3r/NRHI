#include <nrhi/directx12/descriptor_heap.hpp>
#include <nrhi/directx12/device.hpp>



namespace nrhi {

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	void F_directx12_descriptor_heap::set_debug_name(const F_debug_name& value)
	{
		A_render_object::set_debug_name(value);

		auto wvalue = G_to_wstring(value.c_str());
		d3d12_descriptor_heap_p_->SetName(wvalue.data());
	}
#endif



	F_directx12_descriptor_heap::F_directx12_descriptor_heap(
		TKPA_valid<A_device> device_p,
		const F_descriptor_heap_desc& desc
	) :
		A_descriptor_heap(device_p, desc)
	{
		ID3D12Device* d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();

		D3D12_DESCRIPTOR_HEAP_DESC d3d12_descriptor_heap_desc;
		d3d12_descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE(desc.type);
		d3d12_descriptor_heap_desc.NumDescriptors = desc.descriptor_count;
		d3d12_descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS(desc.flags);
		d3d12_descriptor_heap_desc.NodeMask = 0;

		d3d12_device_p->CreateDescriptorHeap(
			&d3d12_descriptor_heap_desc,
			IID_PPV_ARGS(&d3d12_descriptor_heap_p_)
		);

		NCPP_ASSERT(d3d12_descriptor_heap_p_) << "can't create d3d12 descriptor heap";
	}
	F_directx12_descriptor_heap::F_directx12_descriptor_heap(
		TKPA_valid<A_device> device_p,
		const F_descriptor_heap_desc& desc,
		ID3D12DescriptorHeap* d3d12_descriptor_heap_p
	) :
		A_descriptor_heap(device_p, desc),
		d3d12_descriptor_heap_p_(d3d12_descriptor_heap_p)
	{
	}
	F_directx12_descriptor_heap::~F_directx12_descriptor_heap() {
	}



	TU<A_descriptor_heap> HD_directx12_descriptor_heap::create(
		TKPA_valid<A_device> device_p,
		const F_descriptor_heap_desc& desc
	) {
		return TU<F_directx12_descriptor_heap>()(
			device_p,
			desc
		);
	}

	F_descriptor_cpu_address HD_directx12_descriptor_heap::base_cpu_address(
		TKPA_valid<A_descriptor_heap> heap_p
	) {
		return heap_p.T_cast<F_directx12_descriptor_heap>()->d3d12_descriptor_heap_p()->GetCPUDescriptorHandleForHeapStart().ptr;
	}
	F_descriptor_gpu_address HD_directx12_descriptor_heap::base_gpu_address(
		TKPA_valid<A_descriptor_heap> heap_p
	) {
		return heap_p.T_cast<F_directx12_descriptor_heap>()->d3d12_descriptor_heap_p()->GetGPUDescriptorHandleForHeapStart().ptr;
	}

}