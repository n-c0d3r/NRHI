#pragma once

/** @file nrhi/directx12/graphics_pipeline_state.hpp
*
*   Implement directx12 graphics pipeline_state.
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

#include <nrhi/directx12/pipeline_state.hpp>
#include <nrhi/utilities/platform_object_pool.hpp>

#pragma endregion



namespace nrhi {

	class A_device;



	class NRHI_API F_directx12_rasterizer_state_pool : public utilities::TF_platform_object_pool<
		F_directx12_rasterizer_state_pool,
		ID3D12RasterizerState*,
		F_rasterizer_desc
	>
	{

	public:
		static F_rasterizer_desc default_input();
		static constexpr ID3D12RasterizerState* default_object() { return 0; }
		static ID3D12RasterizerState* create_object(TKPA_valid<A_device> device_p, const F_rasterizer_desc& desc);
		static void destroy_object(TKPA_valid<A_device> device_p, const F_rasterizer_desc& desc, ID3D12RasterizerState* object_p);

	};



	class NRHI_API F_directx12_depth_stencil_state_pool : public utilities::TF_platform_object_pool<
		F_directx12_depth_stencil_state_pool,
		ID3D12DepthStencilState*,
		F_depth_stencil_desc
	>
	{

	public:
		static F_depth_stencil_desc default_input();
		static constexpr ID3D12DepthStencilState* default_object() { return 0; }
		static ID3D12DepthStencilState* create_object(TKPA_valid<A_device> device_p, const F_depth_stencil_desc& desc);
		static void destroy_object(TKPA_valid<A_device> device_p, const F_depth_stencil_desc& desc, ID3D12DepthStencilState* object_p);

	};



	class NRHI_API F_directx12_graphics_pipeline_state : public F_directx12_pipeline_state {

	private:
		ID3D12VertexShader* d3d12_vertex_shader_p_ = 0;
		ID3D12PixelShader* d3d12_pixel_shader_p_ = 0;

		ID3D12InputLayout* d3d12_input_layout_p_ = 0;

		ID3D12RasterizerState* d3d12_rasterizer_state_p_ = 0;
		ID3D12DepthStencilState* d3d12_depth_stencil_state_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D12VertexShader* d3d12_vertex_shader_p() noexcept { return d3d12_vertex_shader_p_; }
		NCPP_FORCE_INLINE void set_d3d12_vertex_shader_p_unsafe_unsafe(ID3D12VertexShader* value) noexcept { d3d12_vertex_shader_p_ = value; }
		NCPP_FORCE_INLINE ID3D12PixelShader* d3d12_pixel_shader_p() noexcept { return d3d12_pixel_shader_p_; }
		NCPP_FORCE_INLINE void set_d3d12_pixel_shader_p_unsafe_unsafe(ID3D12PixelShader* value) noexcept { d3d12_pixel_shader_p_ = value; }

		NCPP_FORCE_INLINE ID3D12InputLayout* d3d12_input_layout_p() noexcept { return d3d12_input_layout_p_; }
		NCPP_FORCE_INLINE void set_d3d12_input_layout_p_unsafe_unsafe(ID3D12InputLayout* value) noexcept { d3d12_input_layout_p_ = value; }

		NCPP_FORCE_INLINE ID3D12RasterizerState* d3d12_rasterizer_state_p() noexcept { return d3d12_rasterizer_state_p_; }
		NCPP_FORCE_INLINE void set_d3d12_rasterizer_state_p_unsafe(ID3D12RasterizerState* value) noexcept { d3d12_rasterizer_state_p_ = value; }
		NCPP_FORCE_INLINE ID3D12DepthStencilState* d3d12_depth_stencil_state_p() noexcept { return d3d12_depth_stencil_state_p_; }
		NCPP_FORCE_INLINE void set_d3d12_depth_stencil_state_p_unsafe(ID3D12DepthStencilState* value) noexcept { d3d12_depth_stencil_state_p_ = value; }



	public:
		F_directx12_graphics_pipeline_state(
			TKPA_valid<A_device> device_p,
			const F_pipeline_state_desc& desc,
			ED_pipeline_state_type overrided_type = ED_pipeline_state_type::GRAPHICS
		);
		virtual ~F_directx12_graphics_pipeline_state();

	};

}
