#include <nrhi/frame_buffer_base.hpp>



namespace nrhi {

	A_frame_buffer::A_frame_buffer(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) :
		device_p_(device_p),
		desc_(desc),

		is_has_dsv_(desc.depth_stencil_attachment.is_valid())
	{

		u32 color_attachment_count = desc_.color_attachments.size();
		for(u32 i = 0; i < color_attachment_count; ++i) {

			auto& color_attachment = desc_.color_attachments[i];

			color_attachment_generations_[i] = color_attachment->generation();
		}

		if(is_has_dsv_)
			depth_stencil_attachment_generation_ = desc_.depth_stencil_attachment->generation();

		NCPP_ASSERT(
			desc.color_attachments.size()
			|| is_has_dsv_
		) << "invalid frame buffer, require at least 1 attachment";
	}
	A_frame_buffer::~A_frame_buffer()
	{
	}

	void A_frame_buffer::rebuild() {

		finalize_rebuild();
	}
	void A_frame_buffer::finalize_rebuild() {

		u32 color_attachment_count = desc_.color_attachments.size();
		for(u32 i = 0; i < color_attachment_count; ++i) {

			auto& color_attachment = desc_.color_attachments[i];

			color_attachment_generations_[i] = color_attachment->generation();
		}

		if(is_has_dsv_)
			depth_stencil_attachment_generation_ = desc_.depth_stencil_attachment->generation();
	}

}