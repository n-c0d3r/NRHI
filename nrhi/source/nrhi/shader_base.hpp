#pragma once

/** @file nrhi/shader_base.hpp
*
*   Implement shader base class.
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

#include <nrhi/shader_desc.hpp>
#include <nrhi/shader_type.hpp>

#pragma endregion



namespace nrhi {

	class A_device;
	class A_shader;

	class H_vertex_shader;
	class H_pixel_shader;
	class H_compute_shader;



	NCPP_FHANDLE_TEMPLATE(vertex_shader_handle, A_shader) {

		NCPP_FHANDLE_GENERATED_BODY(vertex_shader_handle, A_shader);

		using H_shader_factory = H_vertex_shader;

	};

	NCPP_FHANDLE_TEMPLATE(pixel_shader_handle, A_shader) {

		NCPP_FHANDLE_GENERATED_BODY(pixel_shader_handle, A_shader);

		using H_shader_factory = H_pixel_shader;

	};

	NCPP_FHANDLE_TEMPLATE(compute_shader_handle, A_shader) {

		NCPP_FHANDLE_GENERATED_BODY(compute_shader_handle, A_shader);

		using H_shader_factory = H_compute_shader;

	};



#define NRHI_VCREATE_SHADER(...) std::remove_const_t<std::remove_reference_t<decltype(__VA_ARGS__)>>::H_shader_factory::create
#define NRHI_ECREATE_SHADER(...) 	ncpp::TF_nth_template_targ< \
										__VA_ARGS__ != nrhi::E_shader_type::VERTEX,          \
										nrhi::H_vertex_shader,                                     \
									ncpp::TF_nth_template_targ< \
										__VA_ARGS__ != nrhi::E_shader_type::PIXEL,          \
										nrhi::H_pixel_shader,                                     \
                                   	ncpp::TF_nth_template_targ< \
										__VA_ARGS__ != nrhi::E_shader_type::COMPUTE,          \
										nrhi::H_compute_shader,                             \
										void\
									>                                                   \
									>                                                    \
									>::create



	class NRHI_API A_shader {

	private:
		TK_valid<A_device> device_p_;
		F_shader_desc desc_;

	public:
		NCPP_FORCE_INLINE TK_valid<A_device> device_p() noexcept { return device_p_; }
		NCPP_FORCE_INLINE const F_shader_desc& desc() const noexcept { return desc_; }



	protected:
		A_shader(
			TKPA_valid<A_device> device_p,
			const F_shader_desc& desc
		);

	public:
		virtual ~A_shader();

	public:
		NCPP_OBJECT(A_shader);

	};

}
