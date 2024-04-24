#include <nrhi/directx11/graphics_pipeline.hpp>
#include <nrhi/directx11/shader.hpp>
#include <nrhi/directx11/shader_blob.hpp>
#include <nrhi/shader_type.hpp>

namespace nrhi {

	F_directx11_graphics_pipeline::F_directx11_graphics_pipeline(
		TK_valid<A_device> device_p,
		const F_pipeline_desc& desc,
		E_pipeline_type overrided_type
	) :
		F_directx11_pipeline(device_p, desc, overrided_type)
	{
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

		NCPP_ASSERT(d3d11_vertex_shader_p_) << "vertex shader is required";
	}
	F_directx11_graphics_pipeline::~F_directx11_graphics_pipeline(){

	}

}