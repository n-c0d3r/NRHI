#include <nrhi/directx11/frame_buffer.hpp>



namespace nrhi {

	F_directx11_frame_buffer::F_directx11_frame_buffer(
		TK_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) :
		A_frame_buffer(device_p, desc)
	{
	}
	F_directx11_frame_buffer::~F_directx11_frame_buffer()
	{
	}



	TU<A_frame_buffer> HD_directx11_frame_buffer::create(
		TK_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) {

		return TU<F_directx11_frame_buffer>()(device_p, desc);
	}

}