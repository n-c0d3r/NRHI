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
#include <nrhi/shader_class_base.hpp>

#pragma endregion



namespace nrhi {

	class A_device;
	class A_shader_class;



	class NRHI_API HD_directx11_shader_compiler {

	public:
		static TU<A_shader_class> compile_hlsl_from_src_content(
			const G_string& name,
			const G_string& src_content,
			const G_string& abs_path,
			TG_span<F_shader_kernel_desc> kernel_descs
		);
		static TU<A_shader_class> compile_hlsl(
			const G_string& name,
			const G_string& abs_path,
			TG_span<F_shader_kernel_desc> kernel_descs
		);

	};

}
