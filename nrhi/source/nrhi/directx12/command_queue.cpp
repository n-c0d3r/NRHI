#include <nrhi/directx12/command_queue.hpp>
#include <nrhi/directx12/command_list.hpp>
#include <nrhi/directx12/device.hpp>



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

	void HD_directx12_command_queue::execute_command_lists(
		TKPA_valid<A_command_queue> command_queue_p,
		TG_span<TK_valid<A_command_list>> command_list_p_span
	) {
//		sz command_list_count = command_list_p_span.size();
//
//		TG_vector<ID3D12CommandList*> d3d12_command_list_p_vector(command_list_count);
//
//		for(u32 i = 0; i < command_list_count; ++i) {
//
//			TK_valid<A_command_list> command_list_p = command_list_p_span[i];
//
//			NCPP_ASSERT(command_queue_p->is_compatible(command_list_p)) << "non-compatible command list";
//
//			ID3D12DeviceContext* d3d12_deferred_ctx_p = command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_queue_p();
//
//			d3d12_deferred_ctx_p->FinishCommandList(false, &(d3d12_command_list_p_vector[i]));
//		}
//
//
//		for(u32 i = 0; i < command_list_count; ++i) {
//
//			command_queue_p
//				.T_cast<F_directx12_command_queue>()
//				->d3d12_command_queue_p()
//				->ExecuteCommandList(
//					d3d12_command_list_p_vector[i],
//					false
//				);
//		}
//
//		for(u32 i = 0; i < command_list_count; ++i) {
//
//			d3d12_command_list_p_vector[i]->Release();
//		}
	}
	void HD_directx12_command_queue::execute_command_list(
		TKPA_valid<A_command_queue> command_queue_p,
		TKPA_valid<A_command_list> command_list_p
	) {
		NCPP_ASSERT(command_queue_p->is_compatible(command_list_p)) << "non-compatible command list";

//		ID3D12CommandList* d3d12_command_list_p = 0;
//
//		ID3D12DeviceContext* d3d12_deferred_ctx_p = command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_queue_p();
//
//		d3d12_deferred_ctx_p->FinishCommandList(false, &d3d12_command_list_p);
//
//		command_queue_p
//			.T_cast<F_directx12_command_queue>()
//			->d3d12_command_queue_p()
//			->ExecuteCommandList(
//				d3d12_command_list_p,
//				false
//			);
//
//		d3d12_command_list_p->Release();
	}

}