#pragma once

/** @file nrhi/shader_blob_desc.hpp
*
*   Implement shader_blob desc.
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

#include <nrhi/shader_type.hpp>
#include <nrhi/format.hpp>

#pragma endregion



namespace nrhi {

	class A_device;
	class A_shader_blob;



	struct F_vertex_attribute {

		G_string name;
		ED_format format;
		u32 duplicate_count = 1;
		u32 offset = NCPP_U32_MAX;

	};
	struct F_instance_attribute {

		G_string name;
		ED_format format;
		u32 duplicate_count = 1;
		u32 offset = NCPP_U32_MAX;

	};

	struct F_input_assembler_desc {

		TG_vector<TG_vector<F_vertex_attribute>> vertex_attribute_groups;
		TG_vector<TG_vector<F_instance_attribute>> instance_attribute_groups;

	};



	struct F_shader_blob_desc {

		G_string name;
		ED_shader_type type = ED_shader_type::NONE;

		F_input_assembler_desc input_assembler_desc;

	};

}
