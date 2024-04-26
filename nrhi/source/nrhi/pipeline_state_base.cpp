#include <nrhi/pipeline_state_base.hpp>
#include <nrhi/shader_base.hpp>
#include <nrhi/shader_blob_base.hpp>



namespace nrhi {

	A_pipeline_state::A_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		E_pipeline_state_type overrided_type
	) :
		device_p_(device_p),
		desc_(desc)
	{
		desc_.type = overrided_type;

		for(const auto& shader_p : desc.shader_p_vector) {

			const auto& shader_blob_desc = shader_p->desc().blob_p->desc();
			if(shader_blob_desc.type == E_shader_type::VERTEX) {

				vertex_buffer_count_ = shader_blob_desc.vertex_attribute_group_desc_vector.size();
				instance_buffer_count_ = shader_blob_desc.instance_attribute_group_desc_vector.size();

				break;
			}
		}
	}
	A_pipeline_state::~A_pipeline_state(){

	}

}