#include <nrhi/directx11/command_list.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/frame_buffer.hpp>
#include <nrhi/directx11/pipeline_state.hpp>
#include <nrhi/directx11/render_target_view.hpp>



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
	void HD_directx11_command_list::clear_rtv(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_rtv_handle rtv_p,
		PA_vector4 color
	) {

		ID3D11DeviceContext* d3d11_device_context_p = command_list_p.T_cast<F_directx11_command_list>()->d3d11_device_context_p();

		if constexpr (std::is_same_v<F_vector4, F_vector4_f32>)
			d3d11_device_context_p->ClearRenderTargetView(
				(ID3D11RenderTargetView*)(rtv_p.T_cast<F_directx11_render_target_view>()->d3d11_view_p()),
				(const f32*)&color
			);
		else {
			f32 color_f32[4] = {
				color.x,
				color.y,
				color.z,
				color.w
			};
			d3d11_device_context_p->ClearRenderTargetView(
				(ID3D11RenderTargetView*)(rtv_p.T_cast<F_directx11_render_target_view>()->d3d11_view_p()),
				color_f32
			);
		}
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