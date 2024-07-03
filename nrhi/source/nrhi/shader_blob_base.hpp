#pragma once

/** @file nrhi/shader_blob_base.hpp
*
*   Implement shader blob base class.
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

#include <nrhi/shader_blob_desc.hpp>
#include <nrhi/shader_type.hpp>

#pragma endregion



namespace nrhi {

	class A_device;



	class NRHI_API A_shader_blob {

	private:
		F_shader_blob_desc desc_;

	public:
		NCPP_FORCE_INLINE const F_shader_blob_desc& desc() const noexcept { return desc_; }



	protected:
		A_shader_blob(
			const F_shader_blob_desc& desc,
			ED_shader_type overrided_type
		);

	public:
		virtual ~A_shader_blob();

	public:
		NCPP_OBJECT(A_shader_blob);

	};

}
