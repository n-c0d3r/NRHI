#include <nrhi/directx11/command_list.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/frame_buffer.hpp>
#include <nrhi/directx11/pipeline_state.hpp>



namespace nrhi {

    F_directx11_command_list::F_directx11_command_list(TKPA_valid<A_device> device_p, const F_command_list_desc& desc) :
        A_command_list(device_p, desc)
    {

        ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

        HRESULT hr = d3d11_device_p->CreateDeferredContext(
            0,
            &d3d11_device_context_p_
        );

        NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 deferred context";

    }
    F_directx11_command_list::~F_directx11_command_list(){

        if(d3d11_device_context_p_)
            d3d11_device_context_p_->Release();
    }



    TU<A_command_list> HD_directx11_command_list::create(TKPA_valid<A_device> device_p, const F_command_list_desc& desc){

        return TU<F_directx11_command_list>()(device_p, desc);
    }

	void HD_directx11_command_list::set_frame_buffer(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_frame_buffer> frame_buffer_p
	) {

	}
	void HD_directx11_command_list::set_graphics_pipeline_state(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_graphics_pipeline_state_handle graphics_pipeline_state_p
	) {

	}
	void HD_directx11_command_list::set_compute_pipeline_state(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_compute_pipeline_state_handle compute_pipeline_state_p
	) {

	}
	void HD_directx11_command_list::draw_indexed(
		TKPA_valid<A_command_list> command_list_p,
		u32 index_count,
		u32 base_index_location
	) {

	}

}