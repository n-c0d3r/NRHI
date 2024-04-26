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

		const auto& color_attachment_p_vector = desc.color_attachment_p_vector;

		if(color_attachment_p_vector.size()) {

			const auto& texture_desc = color_attachment_p_vector[0]->desc().resource_p->desc();

			d3d11_viewport_.TopLeftX = 0;
			d3d11_viewport_.TopLeftY = 0;
			d3d11_viewport_.Width = texture_desc.width;
			d3d11_viewport_.Height = texture_desc.height;
			d3d11_viewport_.MinDepth = 0.0f;
			d3d11_viewport_.MaxDepth = 1.0f;
		}
		else {

			if(is_has_dsv()) {

				const auto& depth_stencil_attachment_p = desc.depth_stencil_attachment_p;

				const auto& texture_desc = depth_stencil_attachment_p->desc().resource_p->desc();

				d3d11_viewport_.TopLeftX = 0;
				d3d11_viewport_.TopLeftY = 0;
				d3d11_viewport_.Width = texture_desc.width;
				d3d11_viewport_.Height = texture_desc.height;
				d3d11_viewport_.MinDepth = 0.0f;
				d3d11_viewport_.MaxDepth = 1.0f;
			}
		}
	}
	F_directx11_frame_buffer::~F_directx11_frame_buffer()
	{
	}



	TU<A_frame_buffer> HD_directx11_frame_buffer::create(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) {

		return TU<F_directx11_frame_buffer>()(device_p, desc);
	}

}