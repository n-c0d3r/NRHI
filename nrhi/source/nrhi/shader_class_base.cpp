#include <nrhi/shader_class_base.hpp>



namespace nrhi {

	A_shader_class::A_shader_class(
		const F_shader_class_desc& desc
	) :
		desc_(desc)
	{
		for(const auto& shader_blob_p : desc.shader_blob_p_vector) {

			shader_blob_p_map_[shader_blob_p->desc().name] = shader_blob_p.no_requirements();
		}
	}
	A_shader_class::~A_shader_class() {
	}

}