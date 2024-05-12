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

		char8 = new_character;

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

		if(
			!is_in_str
			&& !is_in_comment
		) {
			if(is_open_parentheses)
				parentheses_level = (prev_state.parentheses_level + 1);
			if(is_close_parentheses)
				parentheses_level = (prev_state.parentheses_level - 1);
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

		// search states
		F_search_state curr_state;
		F_search_state prev_state;

		// find kernel location
		for(sz i = 0; i < src_length;) {

			curr_state.update(
				prev_state,
				src_content[i]
			);

			// check substring
			if(
				!(curr_state.is_in_str)
				&& !(curr_state.is_in_comment)
				&& !(prev_state.is_variable_name_character)
				&& curr_state.is_variable_name_character
			)
			{
				if ((i + macro_name_length + 1) <= src_length)
				{
					// temp search states
					F_search_state temp_curr_state = curr_state;
					F_search_state temp_prev_state = prev_state;

					b8 is_found = true;
					sz j = i;
					sz j_end = i + macro_name_length;
					for (; j < j_end; ++j)
					{
						temp_curr_state.update(
							temp_prev_state,
							src_content[j]
						);
						temp_prev_state = temp_curr_state;

						if (src_content[j] != macro_name[j - i])
						{
							++j;
							is_found = false;
							break;
						}
					}
					temp_curr_state.update(
						temp_prev_state,
						src_content[j]
					);

					if (
						is_found
						&& !(temp_curr_state.is_variable_name_character)
					)
					{
						curr_state = temp_prev_state;
						prev_state = temp_prev_state;
						parsed_src_content += macro_result;
						i = j;
					}
					else
					{
						parsed_src_content.push_back(curr_state.char8);
						++i;
						prev_state = curr_state;
					}
				}
				else if ((i + macro_name_length) == src_length)
				{
					// temp search states
					F_search_state temp_curr_state = curr_state;
					F_search_state temp_prev_state = prev_state;

					b8 is_found = true;
					sz j = i;
					sz j_end = i + macro_name_length;
					for (; j < j_end; ++j)
					{
						temp_curr_state.update(
							temp_prev_state,
							src_content[j]
						);
						temp_prev_state = temp_curr_state;

						if (src_content[j] != macro_name[j - i])
						{
							++j;
							is_found = false;
							break;
						}
					}

					if (is_found)
					{
						curr_state = temp_prev_state;
						prev_state = temp_prev_state;
						parsed_src_content += macro_result;
						i = j;
					}
					else
					{
						parsed_src_content.push_back(curr_state.char8);
						++i;
						prev_state = curr_state;
					}
				}
				else
				{
					parsed_src_content.push_back(curr_state.char8);
					++i;
					prev_state = curr_state;
				}
			}
			else
			{
				parsed_src_content.push_back(curr_state.char8);
				++i;
				prev_state = curr_state;
			}

			prev_state = curr_state;
		}

		return parsed_src_content;
	}

	G_string H_nsl_utilities::apply_function_like_macro(
		const G_string& src_content,
		const G_string& macro_name,
		const eastl::function<G_string(const G_string&)>& macro_result_functor
	) {
		sz macro_name_length = macro_name.length();

		NCPP_ASSERT(macro_name_length) << "kernel name can't be empty";

		sz src_length = src_content.length();

		// parsed_src_content's variables
		sz parsed_src_length = 0;
		G_string parsed_src_content;
		parsed_src_content.reserve(src_length);

		// search states
		F_search_state curr_state;
		F_search_state prev_state;

		// find kernel location
		for(sz i = 0; i < src_length;) {

			curr_state.update(
				prev_state,
				src_content[i]
			);

			// check substring
			if(
				!(curr_state.is_in_str)
				&& !(curr_state.is_in_comment)
				&& !(prev_state.is_variable_name_character)
				&& curr_state.is_variable_name_character
			)
			{
				if ((i + macro_name_length + 2) <= src_length)
				{
					// temp search states
					F_search_state temp_curr_state = curr_state;
					F_search_state temp_prev_state = prev_state;

					b8 is_macro_name_found = true;
					sz j = i;
					sz j_end = i + macro_name_length;
					for (; j < j_end; ++j)
					{
						temp_curr_state.update(
							temp_prev_state,
							src_content[j]
						);
						temp_prev_state = temp_curr_state;

						if (src_content[j] != macro_name[j - i])
						{
							++j;
							is_macro_name_found = false;
							break;
						}
					}

					i32 start_parentheses_level = temp_curr_state.parentheses_level;

					temp_curr_state.update(
						temp_prev_state,
						src_content[j]
					);

					if (
						is_macro_name_found
						&& !(temp_curr_state.is_variable_name_character)
					)
					{
						b8 is_open_parentheses_found = false;
						sz begin_arg_location = 0;
						for(;j < src_length; ++j) {

							temp_curr_state.update(
								temp_prev_state,
								src_content[j]
							);
							temp_prev_state = temp_curr_state;

							if(temp_curr_state.is_open_parentheses) {
								++j;
								begin_arg_location = j;
								is_open_parentheses_found = true;
								break;
							}
							else if(
								!(temp_curr_state.is_space)
							) {
								break;
							}
						}

						if(is_open_parentheses_found) {

							b8 is_close_parentheses_found = false;
							sz end_arg_location = 0;
							for(;j < src_length; ++j) {

								temp_curr_state.update(
									temp_prev_state,
									src_content[j]
								);
								temp_prev_state = temp_curr_state;

								if(temp_curr_state.parentheses_level == start_parentheses_level) {
									end_arg_location = j;
									++j;
									is_close_parentheses_found = true;
									break;
								}
							}

							if(is_close_parentheses_found) {

								curr_state = temp_prev_state;
								prev_state = temp_prev_state;

								parsed_src_content += macro_result_functor(
									src_content.substr(
										begin_arg_location,
										end_arg_location - begin_arg_location
									)
								);

								i = j;
							}
							else
							{
								parsed_src_content.push_back(curr_state.char8);
								++i;
								prev_state = curr_state;
							}
						}
						else
						{
							parsed_src_content.push_back(curr_state.char8);
							++i;
							prev_state = curr_state;
						}
					}
					else
					{
						parsed_src_content.push_back(curr_state.char8);
						++i;
						prev_state = curr_state;
					}
				}
				else
				{
					parsed_src_content.push_back(curr_state.char8);
					++i;
					prev_state = curr_state;
				}
			}
			else
			{
				parsed_src_content.push_back(curr_state.char8);
				++i;
				prev_state = curr_state;
			}

			prev_state = curr_state;
		}

		return parsed_src_content;
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
			)
				break;
		}

		return src_content.substr(
			0,
			i + 1
		);
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