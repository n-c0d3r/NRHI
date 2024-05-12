#include <nrhi/nsl_shader_compiler.hpp>



namespace nrhi {

	TG_vector<sz> H_nsl_utilities::find_macro_uses(
		const G_string& src_content,
		const G_string& macro_name
	) {

		TG_vector<sz> result;

		// find all potential locations
		size_t pos = src_content.find(macro_name, 0);
		while(pos != eastl::string::npos)
		{
			result.push_back(pos);
			pos = src_content.find(macro_name, pos + 1);
		}

		return result;
	}

	G_string H_nsl_utilities::clear_space_head_tail(const G_string& src_content) {

		return clear_space_head(
			clear_space_tail(
				src_content
			)
		);
	}
	G_string H_nsl_utilities::clear_space_head(const G_string& src_content) {

		sz src_length = src_content.length();
		sz i = 0;
		for(; i < src_length; ++i) {
			char new_char = src_content[i];
			if(
				(new_char != ' ')
				&& (new_char != '\t')
				&& (new_char != '\n')
				&& (new_char != '\r')
			)
				break;
		}

		return src_content.substr(
			i,
			src_length - i
		);
	}
	G_string H_nsl_utilities::clear_space_tail(const G_string& src_content) {

		sz src_length = src_content.length();

		if(src_length == 0)
			return src_content;

		ptrdiff_t i = (src_length - 1);
		for(; i >= 0; --i) {
			char new_char = src_content[i];
			if(
				(new_char != ' ')
				&& (new_char != '\t')
				&& (new_char != '\n')
				&& (new_char != '\r')
			)
				break;
		}

		return src_content.substr(
			0,
			i + 1
		);
	}

	G_string H_nsl_utilities::remove_comments(const G_string& src_content) {

		sz src_length = src_content.length();

		G_string result;
		result.reserve(src_length);

		b8 is_in_comment_1 = false;
		b8 is_in_comment_2 = false;
		b8 is_in_comment = false;
		for(ptrdiff_t i = 0; i < src_length; ++i) {

			if(!is_in_comment) {

				if(i + 1 < src_length) {

					if(
						(src_content[i] == '/')
						&& (src_content[i + 1] == '/')
					)
						is_in_comment_1 = true;
				}

				if(i + 1 < src_length) {

					if(
						(src_content[i] == '/')
						&& (src_content[i + 1] == '*')
					)
						is_in_comment_2 = true;
				}
			}
			is_in_comment = (is_in_comment_1 || is_in_comment_2);

			if(!is_in_comment)
				result.push_back(src_content[i]);

			if(is_in_comment_1) {

				if(src_content[i] == '\n')
				{
					result.push_back('\n');
					is_in_comment_1 = false;
				}
			}

			if(is_in_comment_2) {

				if(
					(src_content[i - 1] == '*')
					&& (src_content[i] == '/')
				)
					is_in_comment_2 = false;
			}
		}

		return std::move(result);
	}



	eastl::optional<TG_vector<F_shader_kernel_desc>> H_nsl_tools::search_kernel_descs(
		const G_string& src_content
	) {

		TG_vector<F_shader_kernel_desc> kernel_descs;



		return std::move(kernel_descs);
	}



	F_nsl_shader_compiler::F_nsl_shader_compiler() {

	}
	F_nsl_shader_compiler::~F_nsl_shader_compiler() {

	}

	eastl::optional<F_nsl_include_blob> F_nsl_shader_compiler::load_include_blob(
		const G_string& path,
		const TG_span<F_shader_kernel_desc>& kernel_descs,
		u32 kernel_index
	) {

		if(path == "nsl")
			return F_nsl_include_blob();

		return eastl::nullopt;
	}
	G_string F_nsl_shader_compiler::process_source(
		const G_string& src_content,
		const G_string& abs_path,
		const TG_span<F_shader_kernel_desc>& kernel_descs,
		u32 kernel_index
	) {
//		return H_nsl_utilities::apply_variable_like_macro(
//			src_content,
//			kernel_descs[kernel_index].name,
//			"main"
//		);
		return "";
	}

	TU<A_shader_blob> F_nsl_shader_compiler::compile_processed_source(
		const G_string& processed_src_content,
		const G_string& abs_path,
		const TG_span<F_shader_kernel_desc>& kernel_descs,
		u32 kernel_index
	) {
		return null;
	}
	TU<A_shader_blob> F_nsl_shader_compiler::compile_source(
		const G_string& src_content,
		const G_string& abs_path,
		const TG_span<F_shader_kernel_desc>& kernel_descs,
		u32 kernel_index
	) {
		return compile_processed_source(
			process_source(
				src_content,
				abs_path,
				kernel_descs,
				kernel_index
			),
			abs_path,
			kernel_descs,
			kernel_index
		);
	}

	TU<A_shader_class> F_nsl_shader_compiler::compile(
		const G_string& src_content,
		const G_string& abs_path
	) {

		return null;
	}

}