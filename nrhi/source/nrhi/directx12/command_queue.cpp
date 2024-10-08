#include <nrhi/directx12/command_queue.hpp>
#include <nrhi/directx12/command_list.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/fence.hpp>
#include <nrhi/command_queue.hpp>



namespace nrhi {

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	void F_directx12_command_queue::set_debug_name(const F_debug_name& value)
	{
		A_render_object::set_debug_name(value);

		auto wvalue = G_to_wstring(value.c_str());
		d3d12_command_queue_p_->SetName(wvalue.data());
	}
#endif



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

	void HD_directx12_command_queue::async_execute_command_lists(
		TKPA_valid<A_command_queue> command_queue_p,
		TG_span<TK_valid<A_command_list>> command_list_p_span
	) {
		sz command_list_count = command_list_p_span.size();

		TG_array<ID3D12CommandList*, NRHI_MAX_COMMAND_LIST_COUNT_PER_BATCH> d3d12_command_list_p_array;

		for(u32 i = 0; i < command_list_count; ++i) {

			TK_valid<A_command_list> command_list_p = command_list_p_span[i];

			NCPP_ASSERT(command_queue_p->is_compatible(command_list_p)) << "non-compatible command list";

			auto d3d12_command_list_p = command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p();
			d3d12_command_list_p_array[i] = d3d12_command_list_p;
		}

		command_queue_p
			.T_cast<F_directx12_command_queue>()
			->d3d12_command_queue_p()
			->ExecuteCommandLists(
				command_list_count,
				d3d12_command_list_p_array.data()
			);

		NCPP_ASSERT(
			SUCCEEDED(
				command_queue_p->device_p().T_cast<F_directx12_device>()->d3d12_device_p()->GetDeviceRemovedReason()
			)
		) << "can't execute command lists";
	}
	void HD_directx12_command_queue::async_execute_command_list(
		TKPA_valid<A_command_queue> command_queue_p,
		TKPA_valid<A_command_list> command_list_p
	) {
		NCPP_ASSERT(command_queue_p->is_compatible(command_list_p)) << "non-compatible command list";

		ID3D12GraphicsCommandList* d3d12_command_list_p = command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p();

		command_queue_p
			.T_cast<F_directx12_command_queue>()
			->d3d12_command_queue_p()
			->ExecuteCommandLists(
				1,
				(ID3D12CommandList**)&d3d12_command_list_p
			);

		NCPP_ASSERT(
			SUCCEEDED(
				command_queue_p->device_p().T_cast<F_directx12_device>()->d3d12_device_p()->GetDeviceRemovedReason()
			)
		) << "can't execute command list";
	}



#pragma region Alternative Functions
#ifdef NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
	void HD_directx12_command_queue::execute_command_lists(
		TKPA_valid<A_command_queue> command_queue_p,
		TG_span<TK_valid<A_command_list>> command_list_p_span
	) {
		H_command_queue::ALTERNATIVE::execute_command_lists(
			command_queue_p,
			command_list_p_span
		);
	}
	void HD_directx12_command_queue::execute_command_list(
		TKPA_valid<A_command_queue> command_queue_p,
		TKPA_valid<A_command_list> command_list_p
	) {
		H_command_queue::ALTERNATIVE::execute_command_list(
			command_queue_p,
			command_list_p
		);
	}
#endif // NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
#pragma endregion

}