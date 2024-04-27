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

#include <nrhi/frame_buffer_desc.hpp>
#include <nrhi/texture.hpp>
#include <nrhi/resource_view_base.hpp>

#pragma endregion



namespace nrhi {

	class A_device;
	class A_frame_buffer;



	class NRHI_API A_frame_buffer {

	private:
		TK_valid<A_device> device_p_;
		F_frame_buffer_desc desc_;

		b8 is_has_dsv_;

	public:
		NCPP_FORCE_INLINE TK_valid<A_device> device_p() noexcept { return device_p_; }
		NCPP_FORCE_INLINE const F_frame_buffer_desc& desc() const noexcept { return desc_; }

		NCPP_FORCE_INLINE b8 is_has_dsv() const noexcept { return is_has_dsv_; }



	protected:
		A_frame_buffer(
			TKPA_valid<A_device> device_p,
			const F_frame_buffer_desc& desc
		);

	public:
		virtual ~A_frame_buffer();

	public:
		void update_viewport();

	};

}
