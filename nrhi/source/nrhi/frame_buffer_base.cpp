#include <nrhi/frame_buffer_base.hpp>



namespace nrhi {

	A_frame_buffer::A_frame_buffer(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc
	) :
		A_device_child(device_p),
		desc_(desc),

		is_has_dsv_(desc.depth_stencil_attachment.is_valid())
	{

		u32 color_attachment_count = desc_.color_attachments.size();
		color_attachment_generations_.resize(color_attachment_count);
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
	// managed
	A_frame_buffer::A_frame_buffer(
		TKPA_valid<A_device> device_p,
		const F_frame_buffer_desc& desc,
		TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses,
		F_descriptor_cpu_address depth_stencil_attachment_descriptor_cpu_address
	) :
		A_frame_buffer(device_p, desc)
	{
		color_attachment_descriptor_cpu_addresses_ = color_attachment_descriptor_cpu_addresses;
		depth_stencil_attachment_descriptor_cpu_address_ = depth_stencil_attachment_descriptor_cpu_address;
	}
	// unmanaged
	A_frame_buffer::A_frame_buffer(
		TKPA_valid<A_device> device_p,
		TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses,
		F_descriptor_cpu_address depth_stencil_attachment_descriptor_cpu_address
	) :
		A_device_child(device_p),
		color_attachment_descriptor_cpu_addresses_(color_attachment_descriptor_cpu_addresses),
		depth_stencil_attachment_descriptor_cpu_address_(depth_stencil_attachment_descriptor_cpu_address)
	{
	}
	A_frame_buffer::~A_frame_buffer()
	{
	}

	void A_frame_buffer::rebuild() {

		finalize_rebuild();
	}
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	void A_frame_buffer::rebuild_with_unmanaged_descriptor_cpu_addresses(
		TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses,
		F_descriptor_cpu_address depth_stencil_attachment_descriptor_cpu_address
	)
	{
		finalize_rebuild_with_unmanaged_descriptor_cpu_addresses(
			color_attachment_descriptor_cpu_addresses,
			depth_stencil_attachment_descriptor_cpu_address
		);
	}
#endif
	void A_frame_buffer::guarantee_generation() {

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		NCPP_ASSERT(management_type() == E_frame_buffer_management_type::MANAGED);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

		if(!is_valid_generation())
			rebuild();
	}

	void A_frame_buffer::finalize_rebuild() {

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		NCPP_ASSERT(management_type() == E_frame_buffer_management_type::MANAGED);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

		u32 color_attachment_count = desc_.color_attachments.size();
		for(u32 i = 0; i < color_attachment_count; ++i) {

			auto& color_attachment = desc_.color_attachments[i];

			color_attachment_generations_[i] = color_attachment->generation();
		}

		if(is_has_dsv_)
			depth_stencil_attachment_generation_ = desc_.depth_stencil_attachment->generation();
	}
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	void A_frame_buffer::finalize_rebuild_with_unmanaged_descriptor_cpu_addresses(
		TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses,
		F_descriptor_cpu_address depth_stencil_attachment_descriptor_cpu_address
	)
	{
		color_attachment_descriptor_cpu_addresses_ = color_attachment_descriptor_cpu_addresses;
		depth_stencil_attachment_descriptor_cpu_address_ = depth_stencil_attachment_descriptor_cpu_address;
	}
#endif

	void A_frame_buffer::release_driver_specific_implementation()
	{
		color_attachment_descriptor_cpu_addresses_.clear();
		depth_stencil_attachment_descriptor_cpu_address_ = 0;
	}

	E_frame_buffer_management_type A_frame_buffer::management_type() const
	{
		return E_frame_buffer_management_type::MANAGED;
	}
}