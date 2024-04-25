#include <nrhi/frame_buffer_base.hpp>



namespace nrhi {

	A_frame_buffer::A_frame_buffer(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) :
		device_p_(device_p),
		desc_(desc)
	{
	}
	A_frame_buffer::~A_frame_buffer()
	{
	}

}