#pragma once

/** @file nrhi/directx11/shader.hpp
*
*   Implement directx11 shader.
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



	class NRHI_API F_directx11_shader : public A_shader {

	public:
		F_directx11_shader(
			TKPA_valid<A_device> device_p,
			const F_shader_desc& desc
		);
		virtual ~F_directx11_shader();

	};



	class NRHI_API F_directx11_vertex_shader : public F_directx11_shader {

	private:
		ID3D11VertexShader* d3d11_vertex_shader_p_ = 0;
		ID3D11InputLayout* d3d11_input_layout_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D11VertexShader* d3d11_vertex_shader_p() noexcept { return d3d11_vertex_shader_p_; }
		NCPP_FORCE_INLINE void set_d3d11_vertex_shader_p_unsafe(ID3D11VertexShader* value) noexcept { d3d11_vertex_shader_p_ = value; }
		NCPP_FORCE_INLINE ID3D11InputLayout* d3d11_input_layout_p() noexcept { return d3d11_input_layout_p_; }
		NCPP_FORCE_INLINE void set_d3d11_input_layout_p_unsafe(ID3D11InputLayout* value) noexcept { d3d11_input_layout_p_ = value; }



	public:
		F_directx11_vertex_shader(
			TK_valid<A_device> device_p,
			const F_shader_desc& desc
		);
		virtual ~F_directx11_vertex_shader();

	};



	class NRHI_API F_directx11_pixel_shader : public F_directx11_shader {

	private:
		ID3D11PixelShader* d3d11_pixel_shader_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D11PixelShader* d3d11_pixel_shader_p() noexcept { return d3d11_pixel_shader_p_; }
		NCPP_FORCE_INLINE void set_d3d11_pixel_shader_p_unsafe(ID3D11PixelShader* value) noexcept { d3d11_pixel_shader_p_ = value; }



	public:
		F_directx11_pixel_shader(
			TK_valid<A_device> device_p,
			const F_shader_desc& desc
		);
		virtual ~F_directx11_pixel_shader();

	};



	class NRHI_API F_directx11_compute_shader : public F_directx11_shader {

	private:
		ID3D11ComputeShader* d3d11_compute_shader_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D11ComputeShader* d3d11_compute_shader_p() noexcept { return d3d11_compute_shader_p_; }
		NCPP_FORCE_INLINE void set_d3d11_compute_shader_p_unsafe(ID3D11ComputeShader* value) noexcept { d3d11_compute_shader_p_ = value; }



	public:
		F_directx11_compute_shader(
			TK_valid<A_device> device_p,
			const F_shader_desc& desc
		);
		virtual ~F_directx11_compute_shader();

	};



	class NRHI_API HD_directx11_shader {

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
