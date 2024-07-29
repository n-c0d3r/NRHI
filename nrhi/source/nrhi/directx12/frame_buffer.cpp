#include <nrhi/directx12/frame_buffer.hpp>
#include <nrhi/directx12/resource.hpp>
#include <nrhi/directx12/resource_view.hpp>



namespace nrhi {

	F_directx12_frame_buffer::F_directx12_frame_buffer(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) :
		A_frame_buffer(device_p, desc)
	{
		update_d3d12_viewport();
	}
	F_directx12_frame_buffer::~F_directx12_frame_buffer()
	{
	}

	void F_directx12_frame_buffer::update_d3d12_viewport() {

		const auto& desc_ = desc();

		const auto& color_attachments = desc_.color_attachments;

		if(color_attachments.size()) {

			const auto& texture_desc = color_attachments[0]->desc().resource_p->desc();

			d3d12_viewport_.TopLeftX = 0;
			d3d12_viewport_.TopLeftY = 0;
			d3d12_viewport_.Width = (f32)(texture_desc.width);
			d3d12_viewport_.Height = (f32)(texture_desc.height);
			d3d12_viewport_.MinDepth = 0.0f;
			d3d12_viewport_.MaxDepth = 1.0f;

			d3d12_scissor_rect_ = {
				0,
				0,
				(LONG)(texture_desc.width),
				(LONG)(texture_desc.height)
			};
		}
		else {

			if(is_has_dsv()) {

				const auto& depth_stencil_attachment = desc_.depth_stencil_attachment;

				const auto& texture_desc = depth_stencil_attachment->desc().resource_p->desc();

				d3d12_viewport_.TopLeftX = 0;
				d3d12_viewport_.TopLeftY = 0;
				d3d12_viewport_.Width = (f32)(texture_desc.width);
				d3d12_viewport_.Height = (f32)(texture_desc.height);
				d3d12_viewport_.MinDepth = 0.0f;
				d3d12_viewport_.MaxDepth = 1.0f;

				d3d12_scissor_rect_ = {
					0,
					0,
					(LONG)(texture_desc.width),
					(LONG)(texture_desc.height)
				};
			}
		}
	}

	void F_directx12_frame_buffer::rebuild() {

		update_d3d12_viewport();

		finalize_rebuild();
	}



	TU<A_frame_buffer> HD_directx12_frame_buffer::create(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) {

		return TU<F_directx12_frame_buffer>()(device_p, desc);
	}

	void HD_directx12_frame_buffer::update_viewport(TK_valid<A_frame_buffer> frame_bufer_p) {

		frame_bufer_p.T_cast<F_directx12_frame_buffer>()->update_d3d12_viewport();
	}

}