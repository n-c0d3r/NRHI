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

#pragma endregion



namespace nrhi {

	class A_device;
	class A_frame_buffer;



	class NRHI_API A_frame_buffer : public A_device_child {

	private:
		F_frame_buffer_desc desc_;

		b8 is_has_dsv_;

	protected:
		TG_fixed_vector<u64, 8, false> color_attachment_generations_;
		u64 depth_stencil_attachment_generation_ = 0xFFFFFFFFFFFFFFFF;

	public:
		NCPP_FORCE_INLINE const F_frame_buffer_desc& desc() const noexcept { return desc_; }

		NCPP_FORCE_INLINE b8 is_has_dsv() const noexcept { return is_has_dsv_; }

		NCPP_FORCE_INLINE const TG_fixed_vector<u64, 8, false>& color_attachment_generations() const noexcept { return color_attachment_generations_; }
		NCPP_FORCE_INLINE void set_color_attachment_generations_unsafe(const TG_fixed_vector<u64, 8, false>& value) noexcept { color_attachment_generations_ = value; }
		NCPP_FORCE_INLINE u32 depth_stencil_attachment_generation() const noexcept { return depth_stencil_attachment_generation_; }
		NCPP_FORCE_INLINE void set_depth_stencil_attachment_generation_unsafe(u64 value) noexcept { depth_stencil_attachment_generation_ = value; }
		NCPP_FORCE_INLINE b8 is_valid_generation() const noexcept {

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



	protected:
		A_frame_buffer(
			TKPA_valid<A_device> device_p,
			const F_frame_buffer_desc& desc
		);

	public:
		virtual ~A_frame_buffer();

	public:
		NCPP_OBJECT(A_frame_buffer);

	public:
		void update_viewport();

	public:
		virtual void rebuild();
		void guarantee_generation();

	protected:
		void finalize_rebuild();

	};

}
