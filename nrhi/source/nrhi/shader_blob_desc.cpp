#include <nrhi/shader_blob_desc.hpp>



namespace nrhi {

	F_shader_blob_desc H_shader_blob_desc::create_vertex_shader_blob_desc(V_string name) {

		return {
			.name = name,
			.type = E_shader_type::VERTEX
		};
	}
	F_shader_blob_desc H_shader_blob_desc::create_pixel_shader_blob_desc(V_string name) {

		return {
			.name = name,
			.type = E_shader_type::PIXEL
		};
	}

	F_shader_blob_desc H_shader_blob_desc::create_compute_shader_blob_desc(V_string name) {

		return {
			.name = name,
			.type = E_shader_type::COMPUTE
		};
	}

}