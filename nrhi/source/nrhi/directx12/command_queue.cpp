#include <nrhi/directx12/command_queue.hpp>
#include <nrhi/directx12/command_list.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/fence.hpp>



namespace nrhi {

	F_directx12_command_queue::F_directx12_command_queue(TKPA_valid<A_device> device_p, const F_command_queue_desc& desc) :
		A_command_queue(device_p, desc)
	{
		D3D12_COMMAND_QUEUE_DESC d3d12_queue_desc = {};
		d3d12_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		d3d12_queue_desc.Type = D3D12_COMMAND_LIST_TYPE(desc.type);

		device_p.T_cast<F_directx12_device>()->d3d12_device_p()->CreateCommandQueue(
			&d3d12_queue_desc,
			IID_PPV_ARGS(&d3d12_command_queue_p_)
		);
		NCPP_ASSERT(d3d12_command_queue_p_) << "create d3d12 command queue failed";
	}
	F_directx12_command_queue::~F_directx12_command_queue(){

		if(d3d12_command_queue_p_)
			d3d12_command_queue_p_->Release();
	}



	TU<A_command_queue> HD_directx12_command_queue::create(TKPA_valid<A_device> device_p, const F_command_queue_desc& desc){

		return TU<F_directx12_command_queue>()(device_p, desc);
	}

	void HD_directx12_command_queue::async_signal(
		TKPA_valid<A_command_queue> command_queue_p,
		TKPA_valid<A_fence> fence_p,
		u64 new_value
	) {
		command_queue_p.T_cast<F_directx12_command_queue>()->d3d12_command_queue_p()->Signal(
			fence_p.T_cast<F_directx12_fence>()->d3d12_fence_p(),
			new_value
		);
	}
	void HD_directx12_command_queue::async_wait(
		TKPA_valid<A_command_queue> command_queue_p,
		TKPA_valid<A_fence> fence_p,
		u64 value
	) {
		command_queue_p.T_cast<F_directx12_command_queue>()->d3d12_command_queue_p()->Wait(
			fence_p.T_cast<F_directx12_fence>()->d3d12_fence_p(),
			value
		);
	}

}