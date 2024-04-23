#include <nrhi/shader_library_base.hpp>



namespace nrhi {

	A_shader_library::A_shader_library(
		const F_shader_library_desc& desc
	) :
		desc_(desc)
	{
		for(const auto& shader_blob_p : desc.shader_blob_p_vector) {

			shader_blob_p_map_[shader_blob_p->desc().name] = shader_blob_p.no_requirements();
		}
	}
	A_shader_library::~A_shader_library() {
	}

}