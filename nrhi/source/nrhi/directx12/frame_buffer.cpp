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
		const auto& color_attachments = desc.color_attachments;
		u32 color_attachment_size = color_attachments.size();
		color_attachment_descriptors_.resize(color_attachment_size);
		for(u32 i = 0; i < color_attachment_size; ++i)
		{
			color_attachment_descriptors_[i] = color_attachments[i]->descriptor();
		}

		const auto& depth_stencil_attachment = desc.depth_stencil_attachment;
		if(depth_stencil_attachment)
			depth_stencil_attachment_descriptor_ = depth_stencil_attachment->descriptor();
	}
	F_directx12_frame_buffer::~F_directx12_frame_buffer()
	{
	}

	void F_directx12_frame_buffer::rebuild() {

		finalize_rebuild();
	}

	void F_directx12_frame_buffer::release_driver_specific_implementation()
	{
	}



	TU<A_frame_buffer> HD_directx12_frame_buffer::create(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) {
		return TU<F_directx12_frame_buffer>()(device_p, desc);
	}
}