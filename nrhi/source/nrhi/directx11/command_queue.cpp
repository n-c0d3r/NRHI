#include <nrhi/directx11/command_queue.hpp>
#include <nrhi/directx11/command_list.hpp>
#include <nrhi/directx11/device.hpp>



namespace nrhi {

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	void F_directx11_command_queue::set_debug_name(const F_debug_name& value)
	{
		A_render_object::set_debug_name(value);

		d3d11_device_context_p_->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			value.size() - 1,
			value.data()
		);
	}
#endif



    F_directx11_command_queue::F_directx11_command_queue(TKPA_valid<A_device> device_p, const F_command_queue_desc& desc) :
        A_command_queue(device_p, desc),
        d3d11_device_context_p_(
            device_p
            .T_cast<F_directx11_device>()
            ->pop_d3d11_immediate_ctx()
        )
    {

    }
    F_directx11_command_queue::~F_directx11_command_queue(){

        if(d3d11_device_context_p_)
            device_p()
            .T_cast<F_directx11_device>()
            ->push_d3d11_immediate_ctx();

    }



    TU<A_command_queue> HD_directx11_command_queue::create(TKPA_valid<A_device> device_p, const F_command_queue_desc& desc){

        return TU<F_directx11_command_queue>()(device_p, desc);
    }

    void HD_directx11_command_queue::execute_command_lists(
        TKPA_valid<A_command_queue> command_queue_p,
        TG_span<TK_valid<A_command_list>> command_list_p_span
    ) {
        sz command_list_count = command_list_p_span.size();

        TG_array<ID3D11CommandList*, NRHI_MAX_COMMAND_LIST_COUNT_PER_BATCH> d3d11_command_list_p_array;

        for(u32 i = 0; i < command_list_count; ++i) {

            TK_valid<A_command_list> command_list_p = command_list_p_span[i];

			NCPP_ASSERT(command_queue_p->is_compatible(command_list_p)) << "non-compatible command list";

            ID3D11DeviceContext* d3d11_deferred_ctx_p = command_list_p.T_cast<F_directx11_command_list>()->d3d11_device_context_p();

            d3d11_deferred_ctx_p->FinishCommandList(false, &(d3d11_command_list_p_array[i]));
        }

        for(u32 i = 0; i < command_list_count; ++i) {

            command_queue_p
				.T_cast<F_directx11_command_queue>()
				->d3d11_device_context_p()
				->ExecuteCommandList(
					d3d11_command_list_p_array[i],
					false
				);
        }

        for(u32 i = 0; i < command_list_count; ++i) {

            d3d11_command_list_p_array[i]->Release();
        }
    }
	void HD_directx11_command_queue::execute_command_list(
		TKPA_valid<A_command_queue> command_queue_p,
		TKPA_valid<A_command_list> command_list_p
	) {
		NCPP_ASSERT(command_queue_p->is_compatible(command_list_p)) << "non-compatible command list";

		ID3D11CommandList* d3d11_command_list_p = 0;

		ID3D11DeviceContext* d3d11_deferred_ctx_p = command_list_p.T_cast<F_directx11_command_list>()->d3d11_device_context_p();

		d3d11_deferred_ctx_p->FinishCommandList(false, &d3d11_command_list_p);

		command_queue_p
			.T_cast<F_directx11_command_queue>()
			->d3d11_device_context_p()
			->ExecuteCommandList(
				d3d11_command_list_p,
				false
			);

		d3d11_command_list_p->Release();
	}

}