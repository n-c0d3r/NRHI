#pragma once

/** @file nrhi/directx12/shader.hpp
*
*   Implement directx12 shader.
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

#include <nrhi/shader_base.hpp>

#pragma endregion



namespace nrhi {

	class A_device;



	class NRHI_API F_directx12_shader : public A_shader {

	private:
		TG_vector<u8> binary_;

	public:
		NCPP_FORCE_INLINE const auto& binary() const noexcept { return binary_; }
		NCPP_FORCE_INLINE void set_binary(const TG_vector<u8>& value) noexcept { binary_ = value; }
		NCPP_FORCE_INLINE void set_binary(TG_vector<u8>&& value) noexcept { binary_ = std::move(value); }



	public:
		F_directx12_shader(
			TKPA_valid<A_device> device_p,
			const F_shader_desc& desc
		);
		virtual ~F_directx12_shader();

	};



	class NRHI_API HD_directx12_shader {

	public:
		static TU<A_shader> create(
			TKPA_valid<A_device> device_p,
			const F_shader_desc& desc
		);

	public:
		static U_vertex_shader_handle create_vertex_shader(
			TKPA_valid<A_device> device_p,
			const F_shader_desc& desc
		);
		static U_pixel_shader_handle create_pixel_shader(
			TKPA_valid<A_device> device_p,
			const F_shader_desc& desc
		);
		static U_compute_shader_handle create_compute_shader(
			TKPA_valid<A_device> device_p,
			const F_shader_desc& desc
		);

	};

}