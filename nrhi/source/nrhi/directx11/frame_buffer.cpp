#include <nrhi/directx11/frame_buffer.hpp>
#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/resource_view.hpp>



namespace nrhi {

	F_directx11_frame_buffer::F_directx11_frame_buffer(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) :
		A_frame_buffer(device_p, desc)
	{

		update_d3d11_viewport();
	}
	F_directx11_frame_buffer::~F_directx11_frame_buffer()
	{
	}

	void F_directx11_frame_buffer::update_d3d11_viewport() {

		const auto& desc_ = desc();

		const auto& color_attachments = desc_.color_attachments;

		if(color_attachments.size()) {

			const auto& texture_desc = color_attachments[0]->desc().resource_p->desc();

			d3d11_viewport_.TopLeftX = 0;
			d3d11_viewport_.TopLeftY = 0;
			d3d11_viewport_.Width = (f32)(texture_desc.width);
			d3d11_viewport_.Height = (f32)(texture_desc.height);
			d3d11_viewport_.MinDepth = 0.0f;
			d3d11_viewport_.MaxDepth = 1.0f;
		}
		else {

			if(is_has_dsv()) {

				const auto& depth_stencil_attachment = desc_.depth_stencil_attachment;

				const auto& texture_desc = depth_stencil_attachment->desc().resource_p->desc();

				d3d11_viewport_.TopLeftX = 0;
				d3d11_viewport_.TopLeftY = 0;
				d3d11_viewport_.Width = (f32)(texture_desc.width);
				d3d11_viewport_.Height = (f32)(texture_desc.height);
				d3d11_viewport_.MinDepth = 0.0f;
				d3d11_viewport_.MaxDepth = 1.0f;
			}
		}
	}

	void F_directx11_frame_buffer::rebuild() {

		update_d3d11_viewport();

		finalize_rebuild();
	}



	TU<A_frame_buffer> HD_directx11_frame_buffer::create(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) {

		return TU<F_directx11_frame_buffer>()(device_p, desc);
	}

	void HD_directx11_frame_buffer::update_viewport(TK_valid<A_frame_buffer> frame_bufer_p) {

		frame_bufer_p.T_cast<F_directx11_frame_buffer>()->update_d3d11_viewport();
	}

}