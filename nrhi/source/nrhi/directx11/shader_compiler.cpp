#include <nrhi/directx11/shader_compiler.hpp>

#include <d3dcompiler.h>



namespace nrhi {

	F_shader_binary HD_directx11_shader_compiler::compile_hlsl_from_src_content(
		const G_string& class_name,
		const G_string& entry_point_name,
		const G_string& src_content,
		const G_string& abs_path,
		u32 model_major,
		u32 model_minor,
		ED_shader_type type
	) {
		ID3DBlob* d3d11_shader_blob_p = 0;
		ID3DBlob* d3d11_error_blob_p = 0;

		HRESULT hr;

		G_string src_name = class_name + entry_point_name;

		G_string model = G_to_string(model_major) + "_" + G_to_string(model_minor);

		NRHI_ENUM_SWITCH(
			type,
			NRHI_ENUM_CASE(
				ED_shader_type::VERTEX,
				model = "vs_" + model;
				hr = D3DCompile(
					src_content.data(),
					src_content.size() * sizeof(char),
					src_name.data(),
					0,
					0,
					entry_point_name.data(),
					model.c_str(),
					0,
					0,
					&d3d11_shader_blob_p,
					&d3d11_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
					<< "can't compile "
					<< ncpp::T_cout_field_name("vertex")
					<< " shader blob "
					<< ncpp::T_cout_value(entry_point_name)
					<< std::endl
					<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
					<< (char*)d3d11_error_blob_p->GetBufferPointer();
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_shader_type::PIXEL,
				model = "ps_" + model;
				hr = D3DCompile(
					src_content.data(),
					src_content.size() * sizeof(char),
					src_name.data(),
					0,
					0,
					entry_point_name.data(),
					model.c_str(),
					0,
					0,
					&d3d11_shader_blob_p,
					&d3d11_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
					<< "can't compile "
					<< ncpp::T_cout_field_name("pixel")
					<< " shader blob "
					<< ncpp::T_cout_value(entry_point_name)
					<< std::endl
					<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
					<< (char*)d3d11_error_blob_p->GetBufferPointer();
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_shader_type::COMPUTE,
				model = "cs_" + model;
				hr = D3DCompile(
					src_content.data(),
					src_content.size() * sizeof(char),
					src_name.data(),
					0,
					0,
					entry_point_name.data(),
					model.c_str(),
					0,
					0,
					&d3d11_shader_blob_p,
					&d3d11_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
					<< "can't compile "
					<< ncpp::T_cout_field_name("compute")
					<< " shader blob "
					<< ncpp::T_cout_value(entry_point_name)
					<< std::endl
					<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
					<< (char*)d3d11_error_blob_p->GetBufferPointer();
				NRHI_ENUM_BREAK;
			)
		);

		F_shader_binary result(d3d11_shader_blob_p->GetBufferSize());
		memcpy(
			(void*)(result.data()),
			(void*)(d3d11_shader_blob_p->GetBufferPointer()),
			result.size()
		);

		return std::move(result);
	}
	F_shader_binary HD_directx11_shader_compiler::compile_hlsl(
		const G_string& class_name,
		const G_string& entry_point_name,
		const G_string& abs_path,
		u32 model_major,
		u32 model_minor,
		ED_shader_type type
	) {
		ID3DBlob* d3d11_shader_blob_p = 0;
		ID3DBlob* d3d11_error_blob_p = 0;

		HRESULT hr;

		G_string src_name = class_name + entry_point_name;

		G_wstring abs_wpath = G_to_wstring(abs_path);

		G_string model = G_to_string(model_major) + "_" + G_to_string(model_minor);

		NRHI_ENUM_SWITCH(
			type,
			NRHI_ENUM_CASE(
				ED_shader_type::VERTEX,
				model = "vs_" + model;
				hr = D3DCompileFromFile(
					abs_wpath.data(),
					0,
					D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entry_point_name.data(),
					model.c_str(),
					0,
					0,
					&d3d11_shader_blob_p,
					&d3d11_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
				<< "can't compile "
				<< ncpp::T_cout_field_name("vertex")
				<< " shader blob "
				<< ncpp::T_cout_value(entry_point_name)
				<< std::endl
				<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
				<< (char*)d3d11_error_blob_p->GetBufferPointer();
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_shader_type::PIXEL,
				model = "ps_" + model;
				hr = D3DCompileFromFile(
					abs_wpath.data(),
					0,
					D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entry_point_name.data(),
					model.c_str(),
					0,
					0,
					&d3d11_shader_blob_p,
					&d3d11_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
				<< "can't compile "
				<< ncpp::T_cout_field_name("pixel")
				<< " shader blob "
				<< ncpp::T_cout_value(entry_point_name)
				<< std::endl
				<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
				<< (char*)d3d11_error_blob_p->GetBufferPointer();
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_shader_type::COMPUTE,
				model = "cs_" + model;
				hr = D3DCompileFromFile(
					abs_wpath.data(),
					0,
					D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entry_point_name.data(),
					model.c_str(),
					0,
					0,
					&d3d11_shader_blob_p,
					&d3d11_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
				<< "can't compile "
				<< ncpp::T_cout_field_name("compute")
				<< " shader blob "
				<< ncpp::T_cout_value(entry_point_name)
				<< std::endl
				<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
				<< (char*)d3d11_error_blob_p->GetBufferPointer();
				NRHI_ENUM_BREAK;
			)
		);

		F_shader_binary result(d3d11_shader_blob_p->GetBufferSize());
		memcpy(
			(void*)(result.data()),
			(void*)(d3d11_shader_blob_p->GetBufferPointer()),
			result.size()
		);
		
		return std::move(result);
	}
	F_shader_binary HD_directx11_shader_compiler::compile_nsl(
		const G_string& shader_class_name,
		const F_nsl_compiled_result& compiled_result,
		u32 shader_index
	) {
		u32 model_major = 0;
		u32 model_minor = 0;

		switch (compiled_result.output_language_enum)
		{
		case E_nsl_output_language::HLSL_4:
			model_major = 4;
			break;
		case E_nsl_output_language::HLSL_5:
			model_major = 5;
			break;
		case E_nsl_output_language::HLSL_5_1:
			model_major = 5;
			model_minor = 1;
			break;
		}

		return compile_hlsl_from_src_content(
			shader_class_name,
			"main",
			compiled_result.build(shader_index),
			"",
			model_major,
			model_minor,
			compiled_result.reflection.shaders[shader_index].type
		);
	}

}