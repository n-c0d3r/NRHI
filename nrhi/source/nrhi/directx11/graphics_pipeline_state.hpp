#pragma once

/** @file nrhi/directx11/graphics_pipeline_state.hpp
*
*   Implement directx11 graphics pipeline_state.
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

#include <nrhi/directx11/pipeline_state.hpp>
#include <nrhi/utilities/platform_object_pool.hpp>

#pragma endregion



namespace nrhi {

	class A_device;



	class NRHI_API F_directx11_rasterizer_state_pool : public utilities::TF_platform_object_pool<
		F_directx11_rasterizer_state_pool,
		ID3D11RasterizerState*,
		F_rasterizer_desc
	>
	{

	public:
		static F_rasterizer_desc default_input();
		static constexpr ID3D11RasterizerState* default_object() { return 0; }
		static ID3D11RasterizerState* create_object(TKPA_valid<A_device> device_p, const F_rasterizer_desc& desc);
		static void destroy_object(TKPA_valid<A_device> device_p, const F_rasterizer_desc& desc, ID3D11RasterizerState* object_p);

	};



	class NRHI_API F_directx11_depth_stencil_state_pool : public utilities::TF_platform_object_pool<
		F_directx11_depth_stencil_state_pool,
		ID3D11DepthStencilState*,
		F_depth_stencil_desc
	>
	{

	public:
		static F_depth_stencil_desc default_input();
		static constexpr ID3D11DepthStencilState* default_object() { return 0; }
		static ID3D11DepthStencilState* create_object(TKPA_valid<A_device> device_p, const F_depth_stencil_desc& desc);
		static void destroy_object(TKPA_valid<A_device> device_p, const F_depth_stencil_desc& desc, ID3D11DepthStencilState* object_p);

	};



	class NRHI_API F_directx11_graphics_pipeline_state : public F_directx11_pipeline_state {

	private:
		ID3D11VertexShader* d3d11_vertex_shader_p_ = 0;
		ID3D11PixelShader* d3d11_pixel_shader_p_ = 0;

		ID3D11InputLayout* d3d11_input_layout_p_ = 0;

		ID3D11RasterizerState* d3d11_rasterizer_state_p_ = 0;
		ID3D11DepthStencilState* d3d11_depth_stencil_state_p_ = 0;

		F_graphics_pipeline_state_options options_;

	public:
		NCPP_FORCE_INLINE ID3D11VertexShader* d3d11_vertex_shader_p() noexcept { return d3d11_vertex_shader_p_; }
		NCPP_FORCE_INLINE void set_d3d11_vertex_shader_p_unsafe_unsafe(ID3D11VertexShader* value) noexcept { d3d11_vertex_shader_p_ = value; }
		NCPP_FORCE_INLINE ID3D11PixelShader* d3d11_pixel_shader_p() noexcept { return d3d11_pixel_shader_p_; }
		NCPP_FORCE_INLINE void set_d3d11_pixel_shader_p_unsafe_unsafe(ID3D11PixelShader* value) noexcept { d3d11_pixel_shader_p_ = value; }

		NCPP_FORCE_INLINE ID3D11InputLayout* d3d11_input_layout_p() noexcept { return d3d11_input_layout_p_; }
		NCPP_FORCE_INLINE void set_d3d11_input_layout_p_unsafe_unsafe(ID3D11InputLayout* value) noexcept { d3d11_input_layout_p_ = value; }

		NCPP_FORCE_INLINE ID3D11RasterizerState* d3d11_rasterizer_state_p() noexcept { return d3d11_rasterizer_state_p_; }
		NCPP_FORCE_INLINE void set_d3d11_rasterizer_state_p_unsafe(ID3D11RasterizerState* value) noexcept { d3d11_rasterizer_state_p_ = value; }
		NCPP_FORCE_INLINE ID3D11DepthStencilState* d3d11_depth_stencil_state_p() noexcept { return d3d11_depth_stencil_state_p_; }
		NCPP_FORCE_INLINE void set_d3d11_depth_stencil_state_p_unsafe(ID3D11DepthStencilState* value) noexcept { d3d11_depth_stencil_state_p_ = value; }

		NCPP_FORCE_INLINE const auto& options() const noexcept { return options_; }

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
        virtual void set_debug_name(const F_debug_name& value) override;
#endif



	public:
		F_directx11_graphics_pipeline_state(
			TKPA_valid<A_device> device_p,
			const F_graphics_pipeline_state_options& options
		);
		virtual ~F_directx11_graphics_pipeline_state();

	};

}
