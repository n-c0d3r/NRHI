#include <nrhi/pipeline_state_base.hpp>
#include <nrhi/shader_base.hpp>



namespace nrhi {

	A_pipeline_state::A_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		ED_pipeline_state_type overrided_type
	) :
		A_device_child(device_p),
		desc_(desc)
	{
		desc_.type = overrided_type;

		for(const auto& shader_p : desc.shader_p_vector) {

			const auto& desc = shader_p->desc();
			if(desc.type == ED_shader_type::VERTEX) {

				vertex_buffer_count_ = (u32)(desc.input_assembler_desc.vertex_attribute_groups.size());
				instance_buffer_count_ = (u32)(desc.input_assembler_desc.instance_attribute_groups.size());

				break;
			}
		}
	}
	A_pipeline_state::~A_pipeline_state(){

	}

}