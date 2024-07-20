#include <nrhi/directx11/shader_compiler.hpp>
#include <nrhi/directx11/shader.hpp>

#include <d3dcompiler.h>



namespace nrhi {

	TG_vector<u8> HD_directx11_shader_compiler::compile_hlsl_from_src_content(
		const G_string& class_name,
		const G_string& entry_point_name,
		const G_string& src_content,
		const G_string& abs_path,
		ED_shader_type type
	) {
		ID3DBlob* d3d11_shader_blob_p = 0;
		ID3DBlob* d3d11_error_blob_p = 0;

		HRESULT hr;

		G_string src_name = class_name + entry_point_name;

		NRHI_ENUM_SWITCH(
			type,
			NRHI_ENUM_CASE(
				ED_shader_type::VERTEX,
				hr = D3DCompile(
					src_content.data(),
					src_content.size() * sizeof(char),
					src_name.data(),
					0,
					0,
					entry_point_name.data(),
					"vs_5_0",
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
				hr = D3DCompile(
					src_content.data(),
					src_content.size() * sizeof(char),
					src_name.data(),
					0,
					0,
					entry_point_name.data(),
					"ps_5_0",
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
				hr = D3DCompile(
					src_content.data(),
					src_content.size() * sizeof(char),
					src_name.data(),
					0,
					0,
					entry_point_name.data(),
					"cs_5_0",
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

		TG_vector<u8> result(d3d11_shader_blob_p->GetBufferSize());
		memcpy(
			(void*)(result.data()),
			(void*)(d3d11_shader_blob_p->GetBufferPointer()),
			result.size()
		);

		return std::move(result);
	}
	TG_vector<u8> HD_directx11_shader_compiler::compile_hlsl(
		const G_string& class_name,
		const G_string& entry_point_name,
		const G_string& abs_path,
		ED_shader_type type
	) {
		ID3DBlob* d3d11_shader_blob_p = 0;
		ID3DBlob* d3d11_error_blob_p = 0;

		HRESULT hr;

		G_string src_name = class_name + entry_point_name;

		G_wstring abs_wpath = G_to_wstring(abs_path);

		NRHI_ENUM_SWITCH(
			type,
			NRHI_ENUM_CASE(
				ED_shader_type::VERTEX,
				hr = D3DCompileFromFile(
					abs_wpath.data(),
					0,
					D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entry_point_name.data(),
					"vs_5_0",
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
				hr = D3DCompileFromFile(
					abs_wpath.data(),
					0,
					D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entry_point_name.data(),
					"ps_5_0",
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
				hr = D3DCompileFromFile(
					abs_wpath.data(),
					0,
					D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entry_point_name.data(),
					"cs_5_0",
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

		TG_vector<u8> result(d3d11_shader_blob_p->GetBufferSize());
		memcpy(
			(void*)(result.data()),
			(void*)(d3d11_shader_blob_p->GetBufferPointer()),
			result.size()
		);
		
		return std::move(result);
	}

}