#pragma once

/** @file nrhi/pipeline_state_base.hpp
*
*   Implement pipeline_state base class.
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

#include <nrhi/pipeline_state_desc.hpp>
#include <nrhi/pipeline_state_type.hpp>

#pragma endregion



namespace nrhi {

	class A_device;
	class A_pipeline_state;



	class NRHI_API A_pipeline_state {

	private:
		TK_valid<A_device> device_p_;
		F_pipeline_state_desc desc_;

		u32 vertex_buffer_count_ = 0;
		u32 instance_buffer_count_ = 0;

	public:
		NCPP_FORCE_INLINE TK_valid<A_device> device_p() noexcept { return device_p_; }
		NCPP_FORCE_INLINE const F_pipeline_state_desc& desc() const noexcept { return desc_; }

		NCPP_FORCE_INLINE u32 vertex_buffer_count() const noexcept { return vertex_buffer_count_; }
		NCPP_FORCE_INLINE u32 instance_buffer_count() const noexcept { return instance_buffer_count_; }



	protected:
		A_pipeline_state(
			TKPA_valid<A_device> device_p,
			const F_pipeline_state_desc& desc,
			E_pipeline_state_type overrided_type
		);

	public:
		virtual ~A_pipeline_state();

	public:
		NCPP_OBJECT(A_pipeline_state);

	};

}
