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

			const auto& input_assembler_desc = shader_p->input_assembler_desc();
			if(shader_p->type() == E_shader_type::VERTEX) {

				vertex_buffer_count_ = (u32)(input_assembler_desc.vertex_attribute_groups.size());
				instance_buffer_count_ = (u32)(input_assembler_desc.instance_attribute_groups.size());

				break;
			}
		}
	}
	A_pipeline_state::~A_pipeline_state(){

	}

}