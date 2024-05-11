#include <nrhi/nsl_shader_compiler.hpp>



namespace nrhi {

	void H_nsl_utilities::F_search_state::update(const F_search_state& prev_state, char new_character) {

		is_variable_name_character = (
			(
				(new_character >= '0')
				&& (new_character <= '9')
			)
			|| (
				(new_character >= 'a')
				&& (new_character <= 'z')
			)
			|| (
				(new_character >= 'A')
				&& (new_character <= 'Z')
			)
			|| (new_character == '_')
		);
		is_open_parentheses = (new_character == '(');
		is_close_parentheses = (new_character == ')');

		is_star = (new_character == '*');
		is_forwardslash = (new_character == '/');

		is_space = (
			(new_character == ' ')
			|| (new_character == '\t')
		);

		// update is in str
		if(!is_in_comment)
		{
			if(
				!is_in_str
				&& (new_character == '\'')
			)
				is_in_str_1 = true;
			else if (
				is_in_str_1
				&& (new_character == '\'')
			)
				is_in_str_1 = false;

			if(
				!is_in_str
				&& (new_character == '"')
			)
				is_in_str_2 = true;
			else if (
				is_in_str_2
				&& (new_character == '"')
			)
				is_in_str_2 = false;

			is_in_str = (is_in_str_1 || is_in_str_2);
		}

		// update is in comment
		if(!is_in_str)
		{
			if(
				!is_in_comment
				&& prev_state.is_forwardslash
				&& is_forwardslash
			)
				is_in_str_1 = true;
			else if (
				is_in_str_1
				&& (new_character == '\n')
			)
				is_in_str_1 = false;

			if(
				!is_in_comment
				&& prev_state.is_forwardslash
				&& new_character
			)
				is_in_str_2 = true;
			else if (
				is_in_str_2
				&& prev_state.is_star
				&& is_forwardslash
			)
				is_in_str_2 = false;

			is_in_comment = (is_in_comment_1 || is_in_comment_2);
		}
	}

	G_string H_nsl_utilities::apply_variable_like_macro(
		const G_string& src_content,
		const G_string& macro_name,
		const G_string& macro_result
	) {
		sz macro_name_length = macro_name.length();

		NCPP_ASSERT(macro_name_length) << "kernel name can't be empty";

		sz src_length = src_content.length();

		// parsed_src_content's variables
		sz parsed_src_length = 0;
		G_string parsed_src_content;
		parsed_src_content.reserve(src_length);

		// essential variables to detect str, comments, and kernel name,...
		b8 is_prev_variable_name_character = false;
		b8 is_prev_close_parentheses = false;

		b8 is_prev_star = false;
		b8 is_prev_forwardslash = false;

		b8 is_curr_variable_name_character = false;

		b8 is_curr_star = false;
		b8 is_curr_forwardslash = false;

		b8 is_in_str_1 = false; // for ' string
		b8 is_in_str_2 = false; // for " string
		b8 is_in_str = false;
		b8 is_in_comment_1 = false; // for single line comment
		b8 is_in_comment_2 = false; // for multiple line comment
		b8 is_in_comment = false;

		// find kernel location
		for(sz i = 0; i < src_length;) {

			char current_char = src_content[i];

			is_curr_variable_name_character = (
				(
					(current_char >= '0')
					&& (current_char <= '9')
				)
				|| (
					(current_char >= 'a')
					&& (current_char <= 'z')
				)
				|| (
					(current_char >= 'A')
					&& (current_char <= 'Z')
				)
				|| (current_char == '_')
			);

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
				&& (!is_prev_variable_name_character)
				&& is_curr_variable_name_character
			)
			{
				if ((i + macro_name_length + 1) <= src_length)
				{
					b8 is_found = true;
					sz j = i;
					sz j_end = i + macro_name_length;
					for (; j < j_end; ++j)
					{
						is_prev_variable_name_character = (
							(
								(src_content[j] >= '0')
								&& (src_content[j] <= '9')
							)
							|| (
								(src_content[j] >= 'a')
								&& (src_content[j] <= 'z')
							)
							|| (
								(src_content[j] >= 'A')
								&& (src_content[j] <= 'Z')
							)
							|| (src_content[j] == '_')
						);
						is_prev_star = (src_content[j] == '*');

						is_prev_forwardslash = (src_content[j] == '/');
						is_prev_close_parentheses = (src_content[j] == ')');

						if (src_content[j] != macro_name[j - i])
						{
							++j;
							is_found = false;
							break;
						}
					}

					if (
						is_found
						&& !(
							(
								(src_content[j] >= '0')
								&& (src_content[j] <= '9')
							)
							|| (
								(src_content[j] >= 'a')
								&& (src_content[j] <= 'z')
							)
							|| (
								(src_content[j] >= 'A')
								&& (src_content[j] <= 'Z')
							)
							|| (src_content[j] == '_')
						)
					)
					{
						parsed_src_content += macro_result;
					}
					else
					{
						parsed_src_content += src_content.substr(i, j - i);
					}

					i = j;
				}
				else if ((i + macro_name_length) == src_length)
				{
					b8 is_found = true;
					sz j = i;
					sz j_end = i + macro_name_length;
					for (; j < j_end; ++j)
					{
						is_prev_variable_name_character = (
							(
								(src_content[j] >= '0')
								&& (src_content[j] <= '9')
							)
							|| (
								(src_content[j] >= 'a')
								&& (src_content[j] <= 'z')
							)
							|| (
								(src_content[j] >= 'A')
								&& (src_content[j] <= 'Z')
							)
							|| (src_content[j] == '_')
						);
						is_prev_star = (src_content[j] == '*');

						is_prev_forwardslash = (src_content[j] == '/');
						is_prev_close_parentheses = (src_content[j] == ')');

						if (src_content[j] != macro_name[j - i])
						{
							++j;
							is_found = false;
							break;
						}
					}

					if (is_found)
					{
						parsed_src_content += macro_result;
					}
					else
					{
						parsed_src_content += src_content.substr(i, j - i);
					}

					i = j;
				}
				else
				{
					parsed_src_content.push_back(current_char);
					++i;

					is_prev_variable_name_character = (
						(
							(current_char >= '0')
							&& (current_char <= '9')
						)
						|| (
							(current_char >= 'a')
							&& (current_char <= 'z')
						)
						|| (
							(current_char >= 'A')
							&& (current_char <= 'Z')
						)
						|| (current_char == '_')
					);
					is_prev_star = (current_char == '*');

					is_prev_forwardslash = (current_char == '/');
					is_prev_close_parentheses = (current_char == ')');
				}
			}
			else
			{
				parsed_src_content.push_back(current_char);
				++i;

				is_prev_variable_name_character = (
					(
						(current_char >= '0')
						&& (current_char <= '9')
					)
					|| (
						(current_char >= 'a')
						&& (current_char <= 'z')
					)
					|| (
						(current_char >= 'A')
						&& (current_char <= 'Z')
					)
					|| (current_char == '_')
				);
				is_prev_star = (current_char == '*');

				is_prev_forwardslash = (current_char == '/');
				is_prev_close_parentheses = (current_char == ')');
			}
		}

		return parsed_src_content;
	}

	G_string H_nsl_utilities::apply_function_like_macro(
		const G_string& src_content,
		const G_string& macro_name,
		const eastl::function<G_string(const G_string&)>& macro_result_functor
	) {
		return "";
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
		return H_nsl_utilities::apply_variable_like_macro(
			src_content,
			kernel_descs[kernel_index].name,
			"main"
		);
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
		const G_string& abs_path,
		const TG_span<F_shader_kernel_desc>& kernel_descs
	) {

		return null;
	}

}