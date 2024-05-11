#include <nrhi/nsl_shader_compiler.hpp>



namespace nrhi {

	F_nsl_shader_compiler::F_nsl_shader_compiler() {

	}
	F_nsl_shader_compiler::~F_nsl_shader_compiler() {

	}

	F_nsl_include_blob F_nsl_shader_compiler::load_include_blob(const G_string& abs_path) {

		return F_nsl_include_blob();
	}
	eastl::optional<G_string> F_nsl_shader_compiler::process_source(
		const G_string& src_content,
		const G_string& abs_path,
		const TG_span<F_shader_kernel_desc>& kernel_descs,
		u32 kernel_index
	) {

		const auto& kernel_desc = kernel_descs[kernel_index];

		const G_string& kernel_name = kernel_desc.name;
		sz kernel_name_length = kernel_name.length();

		NCPP_ASSERT(kernel_name_length) << "kernel name can't be empty";

		sz src_length = src_content.length();

		// essential variables to detect str, comments, and kernel name,...
		b8 is_prev_space = false;
		b8 is_prev_tab = false;
		b8 is_prev_close_parentheses = false;

		b8 is_prev_star = false;
		b8 is_prev_forwardslash = false;

		b8 is_curr_star = false;
		b8 is_curr_forwardslash = false;

		b8 is_in_str_1 = false; // for ' string
		b8 is_in_str_2 = false; // for " string
		b8 is_in_str = false;
		b8 is_in_comment_1 = false; // for single line comment
		b8 is_in_comment_2 = false; // for multiple line comment
		b8 is_in_comment = false;

		// find kernel location
		sz kernel_found_location = -1;
		for(sz i = 0; i < src_length;) {

			char current_char = src_content[i];

			is_curr_star = (current_char == '*');
			is_curr_forwardslash = (current_char == '/');

			// update is in str
			if(!is_in_comment)
			{
				if(
					!is_in_str
					&& (current_char == '\'')
				)
					is_in_str_1 = true;
				else if (
					is_in_str_1
					&& (current_char == '\'')
				)
					is_in_str_1 = false;

				if(
					!is_in_str
					&& (current_char == '"')
					)
					is_in_str_2 = true;
				else if (
					is_in_str_2
					&& (current_char == '"')
					)
					is_in_str_2 = false;

				is_in_str = (is_in_str_1 || is_in_str_2);
			}

			// update is in comment
			if(!is_in_str)
			{
				if(
					!is_in_comment
					&& is_prev_forwardslash
					&& is_curr_forwardslash
				)
					is_in_str_1 = true;
				else if (
					is_in_str_1
					&& (current_char == '\n')
				)
					is_in_str_1 = false;

				if(
					!is_in_comment
					&& is_prev_forwardslash
					&& is_curr_star
				)
					is_in_str_2 = true;
				else if (
					is_in_str_2
					&& is_prev_star
					&& is_curr_forwardslash
				)
					is_in_str_2 = false;

				is_in_comment = (is_in_comment_1 || is_in_comment_2);
			}

			// check substring
			if(
				(!is_in_str)
				&& (!is_in_comment)
				&& (is_prev_space || is_prev_tab || is_prev_close_parentheses)
			)
			{
				if ((i + kernel_name_length + 1) <= src_length)
				{
					b8 is_found = true;
					sz j = i;
					sz j_end = i + kernel_name_length;
					for (; j < j_end; ++j)
					{
						if (src_content[j] != kernel_name[j - i])
						{
							is_found = false;
							break;
						}
					}

					if (
						is_found
						&& (
							(src_content[j] == ' ')
							|| (src_content[j] == '(')
						)
					)
					{
						kernel_found_location = i;
						break;
					}
					++j;
					i = j;
				}
				else break;
			}
			else
			{
				++i;
			}

			is_prev_space = (current_char == ' ');
			is_prev_tab = (current_char == '\t');
			is_prev_star = (current_char == '*');

			is_prev_forwardslash = (current_char == '/');
			is_prev_close_parentheses = (current_char == ')');
		}

		// if kernel location is found, replace it by "main"
		if(kernel_found_location != -1) {

			G_string prev = src_content.substr(0, kernel_found_location);
			G_string next = src_content.substr(
				kernel_found_location + kernel_name_length,
				src_length
				- (
					kernel_found_location
					+ kernel_name_length
				)
			);

			return (
				prev
				+ "main"
				+ next
			);
		}

		// kernel location is not found, return null opt
		return eastl::nullopt;
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
		return null;
	}

	TU<A_shader_class> F_nsl_shader_compiler::compile(
		const G_string& src_content,
		const G_string& abs_path,
		const TG_span<F_shader_kernel_desc>& kernel_descs
	) {

		return null;
	}

}