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

		NCPP_ASSERT(
			desc.color_attachments.size()
			|| is_has_dsv_
		) << "invalid frame buffer, require at least 1 attachment";
	}
	A_frame_buffer::~A_frame_buffer()
	{
	}

}