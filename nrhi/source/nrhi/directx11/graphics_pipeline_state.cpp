#include <nrhi/directx11/graphics_pipeline_state.hpp>
#include <nrhi/directx11/shader.hpp>
#include <nrhi/directx11/shader_blob.hpp>
#include <nrhi/shader_type.hpp>

namespace nrhi {

	NRHI_PLATFORM_OBJECT_POOL_DEFINE(F_directx11_rasterizer_state_pool);

	ID3D11RasterizerState* F_directx11_rasterizer_state_pool::create_object(TK_valid<A_device> device_p, const F_rasterizer_desc& desc) {

		return 0;
	}
	void F_directx11_rasterizer_state_pool::destroy_object(TK_valid<A_device> device_p, ID3D11RasterizerState* object_p, const F_rasterizer_desc& desc) {

	}



	F_directx11_graphics_pipeline_state::F_directx11_graphics_pipeline_state(
		TK_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		E_pipeline_state_type overrided_type
	) :
		F_directx11_pipeline_state(device_p, desc, overrided_type)
	{

		// get d3d11 shaders
		for(auto shader_p : desc.shader_p_vector) {

			switch (shader_p->desc().blob_p->desc().type) {

			case E_shader_type::VERTEX:
				NCPP_ASSERT(!d3d11_vertex_shader_p_) << "only accept 1 vertex shader";
				d3d11_vertex_shader_p_ = shader_p.T_cast<F_directx11_vertex_shader>()->d3d11_vertex_shader_p();
				break;

			case E_shader_type::PIXEL:
				NCPP_ASSERT(!d3d11_pixel_shader_p_) << "only accept 1 pixel shader";
				d3d11_pixel_shader_p_ = shader_p.T_cast<F_directx11_pixel_shader>()->d3d11_pixel_shader_p();
				break;
			}
		}

		// acquire d3d11 rasterizer state
		d3d11_rasterizer_state_p_ = F_directx11_rasterizer_state_pool::acquire_object(
			device_p,
			desc.rasterizer_desc
		);

		NCPP_ASSERT(d3d11_vertex_shader_p_) << "vertex shader is required";
	}
	F_directx11_graphics_pipeline_state::~F_directx11_graphics_pipeline_state(){

		// release d3d11 rasterizer state
		F_directx11_rasterizer_state_pool::release_object(
			device_p(),
			desc().rasterizer_desc
		);
		d3d11_rasterizer_state_p_ = 0;
	}

	void F_directx11_graphics_pipeline_state::initialize_pools() {

		F_directx11_rasterizer_state_pool::initialize();
	}
	void F_directx11_graphics_pipeline_state::release_pools() {

		F_directx11_rasterizer_state_pool::release();
	}

}