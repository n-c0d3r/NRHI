#include <nrhi/directx12/command_allocator.hpp>
#include <nrhi/directx12/device.hpp>



namespace nrhi {

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	void F_directx12_command_allocator::set_debug_name(const F_debug_name& value)
	{
		A_render_object::set_debug_name(value);

		auto wvalue = G_to_wstring(value.c_str());
		d3d12_command_allocator_p_->SetName(wvalue.data());
	}
#endif



	F_directx12_command_allocator::F_directx12_command_allocator(TKPA_valid<A_device> device_p, const F_command_allocator_desc& desc) :
		A_command_allocator(device_p, desc)
	{
		device_p.T_cast<F_directx12_device>()->d3d12_device_p()->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE(desc.type),
			IID_PPV_ARGS(&d3d12_command_allocator_p_)
		);
		NCPP_ASSERT(d3d12_command_allocator_p_) << "create d3d12 command allocator failed";
	}
	F_directx12_command_allocator::~F_directx12_command_allocator(){
	}



	TU<A_command_allocator> HD_directx12_command_allocator::create(TKPA_valid<A_device> device_p, const F_command_allocator_desc& desc){

		return TU<F_directx12_command_allocator>()(device_p, desc);
	}

	void HD_directx12_command_allocator::flush(TKPA_valid<A_command_allocator> command_allocator_p) {

		command_allocator_p.T_cast<F_directx12_command_allocator>()->d3d12_command_allocator_p()->Reset();
	}

}