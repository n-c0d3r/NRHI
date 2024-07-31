#pragma once

/** @file nrhi/directx11/shader_compiler.hpp
*
*   Implement directx11 shader compiler.
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

#include <nrhi/shader_compiler_base.hpp>
#include <nrhi/nsl_shader_compiler.hpp>

#pragma endregion



namespace nrhi {

	class A_device;
	class A_shader_class;



	class NRHI_API HD_directx11_shader_compiler {

	public:
		static F_shader_binary compile_hlsl_from_src_content(
			const G_string& shader_class_name,
			const G_string& shader_name,
			const G_string& entry_point_name,
			const G_string& src_content,
			const G_string& abs_path,
			u32 model_major,
			u32 model_minor,
			ED_shader_type type
		);
		static F_shader_binary compile_hlsl(
			const G_string& shader_class_name,
			const G_string& shader_name,
			const G_string& entry_point_name,
			const G_string& abs_path,
			u32 model_major,
			u32 model_minor,
			ED_shader_type type
		);
		static F_shader_binary compile_nsl(
			const F_nsl_compiled_result& compiled_result,
			u32 shader_index
		);

	};

}
