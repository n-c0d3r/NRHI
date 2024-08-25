#include <nrhi/directx12/frame_buffer.hpp>
#include <nrhi/directx12/resource.hpp>
#include <nrhi/directx12/resource_view.hpp>



namespace nrhi {

	F_directx12_frame_buffer::F_directx12_frame_buffer(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc,
		TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses,
		F_descriptor_cpu_address depth_stencil_attachment_descriptor_cpu_address
	) :
		A_frame_buffer(
			device_p,
			desc,
			color_attachment_descriptor_cpu_addresses,
			depth_stencil_attachment_descriptor_cpu_address
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



	namespace internal
	{
		TG_fixed_vector<F_descriptor_cpu_address, 8, false> extract_color_attachment_descriptor_cpu_addresses_from_frame_buffer_desc(
			const F_frame_buffer_desc& desc
		)
		{
			const auto& color_attachments = desc.color_attachments;
			u32 color_attachment_size = color_attachments.size();

			TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses(color_attachment_size);
			for(u32 i = 0; i < color_attachment_size; ++i)
			{
				color_attachment_descriptor_cpu_addresses[i] = color_attachments[i]->descriptor_handle().cpu_address;
			}

			return color_attachment_descriptor_cpu_addresses;
		}
		F_descriptor_cpu_address extract_depth_stencil_attachment_descriptor_cpu_address_from_frame_buffer_desc(
			const F_frame_buffer_desc& desc
		)
		{
			const auto& depth_stencil_attachment = desc.depth_stencil_attachment;
			if(depth_stencil_attachment)
				return depth_stencil_attachment->descriptor_handle().cpu_address;

			return 0;
		}
	}

	F_directx12_managed_frame_buffer::F_directx12_managed_frame_buffer(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) :
		F_directx12_frame_buffer(
			device_p,
			desc,
			internal::extract_color_attachment_descriptor_cpu_addresses_from_frame_buffer_desc(desc),
			internal::extract_depth_stencil_attachment_descriptor_cpu_address_from_frame_buffer_desc(desc)
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

	E_frame_buffer_management_type F_directx12_unmanaged_frame_buffer::management_type() const
	{
		return E_frame_buffer_management_type::UNMANAGED;
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