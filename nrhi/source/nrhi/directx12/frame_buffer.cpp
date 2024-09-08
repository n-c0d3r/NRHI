#include <nrhi/directx12/frame_buffer.hpp>
#include <nrhi/directx12/resource.hpp>
#include <nrhi/directx12/resource_view.hpp>



namespace nrhi {

	F_directx12_frame_buffer::F_directx12_frame_buffer(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) :
		A_frame_buffer(
			device_p,
			desc
		)
	{
	}
	F_directx12_frame_buffer::F_directx12_frame_buffer(
		TKPA_valid<A_device> device_p,
		TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses,
		F_descriptor_cpu_address depth_stencil_attachment_descriptor_cpu_address
	) :
		A_frame_buffer(
			device_p,
			color_attachment_descriptor_cpu_addresses,
			depth_stencil_attachment_descriptor_cpu_address
		)
	{
	}
	F_directx12_frame_buffer::~F_directx12_frame_buffer()
	{
	}



	F_directx12_managed_frame_buffer::F_directx12_managed_frame_buffer(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) :
		F_directx12_frame_buffer(
			device_p,
			desc
		)
	{
	}
	F_directx12_managed_frame_buffer::~F_directx12_managed_frame_buffer()
	{
		F_directx12_managed_frame_buffer::release_driver_specific_implementation();
	}



	F_directx12_unmanaged_frame_buffer::F_directx12_unmanaged_frame_buffer(
		TKPA_valid<A_device> device_p,
		TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses,
		F_descriptor_cpu_address depth_stencil_attachment_descriptor_cpu_address
	) :
		F_directx12_frame_buffer(
			device_p,
			color_attachment_descriptor_cpu_addresses,
			depth_stencil_attachment_descriptor_cpu_address
		)
	{
	}
	F_directx12_unmanaged_frame_buffer::~F_directx12_unmanaged_frame_buffer()
	{
	}



	TU<A_frame_buffer> HD_directx12_frame_buffer::create(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) {
		return TU<F_directx12_managed_frame_buffer>()(device_p, desc);
	}

	TU<A_frame_buffer> HD_directx12_frame_buffer::create_with_unmanaged_descriptor_cpu_addresses(
		TKPA_valid<A_device> device_p,
		TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses,
		F_descriptor_cpu_address depth_stencil_attachment_descriptor_cpu_address
	)
	{
		return TU<F_directx12_unmanaged_frame_buffer>()(
			device_p,
			color_attachment_descriptor_cpu_addresses,
			depth_stencil_attachment_descriptor_cpu_address
		);
	}
}