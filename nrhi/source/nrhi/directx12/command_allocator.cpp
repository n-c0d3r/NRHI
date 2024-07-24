#include <nrhi/directx12/command_allocator.hpp>
#include <nrhi/directx12/device.hpp>



namespace nrhi {

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

}