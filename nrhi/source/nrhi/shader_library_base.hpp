#pragma once

/** @file nrhi/shader_library_base.hpp
*
*   Implement shader_library base class.
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

#include <nrhi/shader_type.hpp>
#include <nrhi/shader_desc.hpp>
#include <nrhi/shader_blob_desc.hpp>
#include <nrhi/shader_blob_base.hpp>

#pragma endregion



namespace nrhi {

	class A_device;
	class A_shader;



	struct F_shader_kernel_desc {

		F_shader_blob_desc blob_desc;
		TG_vector<eastl::pair<G_string, G_string>> defines;

	};

	struct F_shader_library_desc {

		G_string name;
		TG_vector<TS_valid<A_shader_blob>> shader_blob_p_vector;

	};



	class NRHI_API A_shader_library {

	public:
		using F_shader_blob_p_map = TG_unordered_map<G_string, TK<A_shader_blob>>;



	private:
		F_shader_library_desc desc_;
		F_shader_blob_p_map shader_blob_p_map_;

	public:
		NCPP_FORCE_INLINE const F_shader_library_desc& desc() noexcept { return desc_; }
		NCPP_FORCE_INLINE const F_shader_blob_p_map& shader_blob_p_map() noexcept { return shader_blob_p_map_; }
		NCPP_FORCE_INLINE TK_valid<A_shader_blob> shader_blob_p(V_string name) const noexcept {

			NCPP_ASSERT(shader_blob_p_map_.find(name) != shader_blob_p_map_.end()) << "not found shader blob named " << T_cout_value(name);

			return NCPP_FOREF_VALID(shader_blob_p_map_.find(name)->second);
		}



	protected:
		A_shader_library(
			const F_shader_library_desc& desc
		);

	public:
		virtual ~A_shader_library();

	};

}
