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
	}
	F_directx11_frame_buffer::~F_directx11_frame_buffer()
	{
	}

	void F_directx11_frame_buffer::rebuild() {

		finalize_rebuild();
	}



	TU<A_frame_buffer> HD_directx11_frame_buffer::create(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) {
		return TU<F_directx11_frame_buffer>()(device_p, desc);
	}
}