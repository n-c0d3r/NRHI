#pragma once

/** @file nrhi/frame_buffer_base.hpp
*
*   Implement frame buffer base class.
*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#pragma region Includes

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/prerequisites.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/device_child.hpp>
#include <nrhi/frame_buffer_desc.hpp>
#include <nrhi/texture.hpp>
#include <nrhi/resource_view_base.hpp>

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#include <nrhi/descriptor_base.hpp>
#endif

#pragma endregion



namespace nrhi {

	class A_device;
	class A_frame_buffer;



	class NRHI_API A_frame_buffer : public A_device_child {

	private:
		F_frame_buffer_desc desc_;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		E_frame_buffer_management_type management_type_ = E_frame_buffer_management_type::MANAGED;
#endif

		b8 is_has_dsv_;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		TG_fixed_vector<F_descriptor_cpu_address, 8, false> unmanaged_color_attachment_descriptor_cpu_addresses_;
		F_descriptor_cpu_address unmanaged_depth_stencil_attachment_descriptor_cpu_address_ = 0;
#endif

	protected:
		TG_fixed_vector<u64, 8, false> color_attachment_generations_;
		u64 depth_stencil_attachment_generation_ = 0xFFFFFFFFFFFFFFFF;

	public:
		NCPP_FORCE_INLINE const F_frame_buffer_desc& desc() const noexcept { return desc_; }

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		NCPP_FORCE_INLINE E_frame_buffer_management_type management_type() const noexcept { return management_type_; }
#endif

		NCPP_FORCE_INLINE b8 is_has_dsv() const noexcept { return is_has_dsv_; }

		NCPP_FORCE_INLINE const TG_fixed_vector<u64, 8, false>& color_attachment_generations() const noexcept { return color_attachment_generations_; }
		NCPP_FORCE_INLINE void set_color_attachment_generations_unsafe(const TG_fixed_vector<u64, 8, false>& value) noexcept { color_attachment_generations_ = value; }
		NCPP_FORCE_INLINE u32 depth_stencil_attachment_generation() const noexcept { return depth_stencil_attachment_generation_; }
		NCPP_FORCE_INLINE void set_depth_stencil_attachment_generation_unsafe(u64 value) noexcept { depth_stencil_attachment_generation_ = value; }
		NCPP_FORCE_INLINE b8 is_valid_generation() const noexcept {

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
			if(management_type() == E_frame_buffer_management_type::UNMANAGED)
				return true;
#endif

			u32 color_attachment_count = desc_.color_attachments.size();
			for(u32 i = 0; i < color_attachment_count; ++i) {

				const auto& color_attachment = desc_.color_attachments[i];

				if(
					color_attachment->generation()
					!= color_attachment_generations_[i]
				)
					return false;
			}

			if(is_has_dsv_)
				return (
					desc_.depth_stencil_attachment->generation()
					== depth_stencil_attachment_generation_
				);

			return true;
		}

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		NCPP_FORCE_INLINE const auto& unmanaged_color_attachment_descriptor_cpu_addresses() const noexcept { return unmanaged_color_attachment_descriptor_cpu_addresses_; }
		NCPP_FORCE_INLINE const auto& unmanaged_depth_stencil_attachment_descriptor_cpu_address() const noexcept { return unmanaged_depth_stencil_attachment_descriptor_cpu_address_; }
		NCPP_FORCE_INLINE void set_color_attachment_descriptor_cpu_addresses_unsafe(
			const TG_fixed_vector<F_descriptor_cpu_address, 8, false>& value
		) noexcept
		{
			unmanaged_color_attachment_descriptor_cpu_addresses_ = value;
		}
		NCPP_FORCE_INLINE void set_depth_stencil_attachment_descriptor_cpu_address_unsafe(F_descriptor_cpu_address value) noexcept
		{
			unmanaged_depth_stencil_attachment_descriptor_cpu_address_ = value;
		}
#endif



	protected:
		A_frame_buffer(
			TKPA_valid<A_device> device_p,
			const F_frame_buffer_desc& desc
		);
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		// unmanaged
		A_frame_buffer(
			TKPA_valid<A_device> device_p,
			TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses,
			F_descriptor_cpu_address depth_stencil_attachment_descriptor_cpu_address
		);
#endif

	public:
		virtual ~A_frame_buffer();

	public:
		NCPP_OBJECT(A_frame_buffer);

	public:
		virtual void rebuild();
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		virtual void rebuild_with_unmanaged_descriptor_cpu_addresses(
			TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses,
			F_descriptor_cpu_address depth_stencil_attachment_descriptor_cpu_address
		);
#endif
		void guarantee_generation();

	protected:
		void finalize_rebuild();
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		void finalize_rebuild_with_unmanaged_descriptor_cpu_addresses(
			TG_fixed_vector<F_descriptor_cpu_address, 8, false> color_attachment_descriptor_cpu_addresses,
			F_descriptor_cpu_address depth_stencil_attachment_descriptor_cpu_address
		);
#endif

	public:
		virtual void release_driver_specific_implementation();
	};

}
