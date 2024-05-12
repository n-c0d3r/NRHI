#include <nrhi/nsl_shader_compiler.hpp>



namespace nrhi {

	void H_nsl_utilities::F_str_state::begin_check(char c) {

		if(!value) {
			if(c == '\'')
			{
				value_1 = true;
				next_value_1 = true;
				value = true;
				next_value = true;
			}
			if(c == '"')
			{
				value_2 = true;
				next_value_2 = true;
				value = true;
				next_value = true;
			}
		}
		else {
			if(c == '\'')
			{
				next_value_1 = false;
				next_value = false;
			}
			if(c == '"')
			{
				next_value_2 = false;
				next_value = false;
			}
		}
	}
	void H_nsl_utilities::F_str_state::end_check() {

		value = next_value;
		value_1 = next_value_1;
		value_2 = next_value_2;

		prev_value = value;
		prev_value_1 = value_1;
		prev_value_2 = value_2;
	}



	b8 H_nsl_utilities::is_variable_name_character(char c) {

		return (
			(c == '_')
			|| (
				(c >= '0')
				&& (c <= '9')
			)
			|| (
				(c >= 'a')
				&& (c <= 'z')
			)
			|| (
				(c >= 'A')
				&& (c <= 'Z')
			)
		);
	}

	eastl::optional<TG_vector<H_nsl_utilities::F_info_tree>> H_nsl_utilities::build_info_trees(const G_string& src_content) {

		TG_vector<F_info_tree> trees;

		sz src_length = src_content.length();

		sz i = 0;
		while(i < src_length) {

			// space, tab, new line
			while(
				(i < src_length)
			) {
				if(
					!(
						(src_content[i] == ' ')
						|| (src_content[i] == '\t')
						|| (src_content[i] == '\n')
						|| (src_content[i] == '\r')
					)
				) {
					break;
				}
				++i;
			}

			sz begin_name_location = i;
			sz end_name_location = i;

			// name
			while(
				(i < src_length)
			) {
				if(
					!is_variable_name_character(src_content[i])
				) {
					break;
				}
				++i;
			}
			b8 is_name_found = (i != begin_name_location);

			b8 is_name_as_str = false;
			F_str_state str_state;

			if(is_name_found) {
				end_name_location = i;
			}
			else {
				// name as str
				while(
					(i < src_length)
				) {
					str_state.begin_check(src_content[i]);
					if(
						!(str_state.value)
					) {
						str_state.end_check();
						break;
					}
					is_name_as_str = true;
					str_state.end_check();
					++i;
				}
				end_name_location = i;
			}

			// if name is found
			if(
				(begin_name_location != end_name_location)
				&& !(str_state.value)
			) {

				// get name
				G_string name;
				if(is_name_as_str)
					name = src_content.substr(begin_name_location + 1, end_name_location - begin_name_location - 2);
				else
					name = src_content.substr(begin_name_location, end_name_location - begin_name_location);

				// parse childs
				TG_vector<F_info_tree> childs;

				// skip spaces, tabs, and new lines
				sz t = i;
				for(; t < src_length; ++t)
				{
					if(
						!(
							(src_content[t] == ' ')
							|| (src_content[t] == '\t')
							|| (src_content[t] == '\n')
							|| (src_content[t] == '\r')
						)
					) {
						break;
					}
				}

				// if not end of src content, continue childs parsing
				if(t < src_length) {

					// if beginning of childs
					if(src_content[t] == '{') {

						i32 level = 0;

						sz begin_arg_location = t;
						sz end_arg_location = begin_arg_location;

						F_str_state arg_str_state;

						sz k = begin_arg_location;
						for(; k < src_length; ++k) {

							arg_str_state.begin_check(src_content[k]);

							if(
								!(arg_str_state.value)
								&& (src_content[k] == '{')
							)
								++level;

							if(
								!(arg_str_state.value)
								&& (src_content[k] == '}')
							)
								--level;

							if(level == 0) {

								end_arg_location = k + 1;

								G_string childs_src = src_content.substr(begin_arg_location + 1, end_arg_location - begin_arg_location - 2);

								auto childs_opt = build_info_trees(childs_src);
								if(!childs_opt)
									return eastl::nullopt;

								childs = childs_opt.value();

								i = end_arg_location;
								break;
							}

							arg_str_state.end_check();
						}
					}
				}

				// push back current tree
				trees.push_back({
					.name = name,
					.childs = childs
				});
			}
			else {
				// invalid src content
				if(
					(end_name_location != src_length)
					|| str_state.value
				)
					return eastl::nullopt;
			}
		}

		return std::move(trees);
	}

	TG_vector<H_nsl_utilities::F_function_macro_use> H_nsl_utilities::find_function_macro_uses(
		const G_string& src_content,
		const G_string& macro_name
	) {
		TG_vector<F_function_macro_use> result;

		sz src_length = src_content.length();

		if(src_length < macro_name.length())
			return {};

		sz end_check = src_length - macro_name.length() + 1;

		F_str_state str_state;
		for(sz i = 0; i < end_check;) {

			str_state.begin_check(src_content[i]);

			{
				b8 is_march = true;
				sz j = i;
				sz j_end = i + macro_name.length();
				for(; j < j_end; ++j)
				{
					if(src_content[j] != macro_name[j - i]) {

						is_march = false;
						break;
					}
				}

				if(is_march) {

					b8 is_left_correct = true;
					b8 is_right_correct = true;

					if(i > 0) {
						is_left_correct = !is_variable_name_character(src_content[i - 1]);
					}
					if(j_end < src_length) {
						is_right_correct = !is_variable_name_character(src_content[j_end]);
					}

					if(is_left_correct && is_right_correct) {

						sz t = j_end;
						for(; t < src_length; ++t)
						{
							if(
								!(
									(src_content[t] == ' ')
									|| (src_content[t] == '\t')
									|| (src_content[t] == '\n')
									|| (src_content[t] == '\r')
								)
							) {
								break;
							}
						}

						if(t < src_length) {

							if(src_content[t] == '(') {

								i32 level = 0;

								sz begin_arg_location = t;
								sz end_arg_location = begin_arg_location;

								F_str_state arg_str_state;

								sz k = begin_arg_location;
								for(; k < src_length; ++k) {

									arg_str_state.begin_check(src_content[k]);

									if(
										!(arg_str_state.value)
										&& (src_content[k] == '(')
									)
										++level;

									if(
										!(arg_str_state.value)
										&& (src_content[k] == ')')
									)
										--level;

									if(level == 0) {

										end_arg_location = k + 1;

										result.push_back({
											.begin_location = i,
											.end_location = end_arg_location,
											.arg = src_content.substr(begin_arg_location + 1, end_arg_location - begin_arg_location - 2)
										});

										i = end_arg_location;

										goto NRHI_NEXT_CHARACTER_CHECK;
									}

									arg_str_state.end_check();
								}
							}
						}
					}
				}
			}

			str_state.end_check();
			++i;
			continue;

			NRHI_NEXT_CHARACTER_CHECK:
			str_state.end_check();
		}

		return result;
	}
	G_string H_nsl_utilities::apply_function_macro_uses(
		const G_string& src_content,
		const G_string& macro_name,
		const F_function_macro_result_functor& macro_result_functor,
		const TG_vector<F_function_macro_use>& uses
	) {
		G_string result;
		result.reserve(src_content.length());

		sz src_length = src_content.length();
		sz macro_name_length = macro_name.length();

		sz begin_location = 0;
		for(const auto& use : uses) {

			result += src_content.substr(begin_location, use.begin_location - begin_location);
			result += macro_result_functor(use);

			begin_location = use.end_location;
		}

		result += src_content.substr(begin_location, src_length - begin_location);

		return std::move(result);
	}

	TG_vector<H_nsl_utilities::F_variable_macro_use> H_nsl_utilities::find_variable_macro_uses(
		const G_string& src_content,
		const G_string& macro_name
	) {
		TG_vector<sz> result;

		sz src_length = src_content.length();

		if(src_length < macro_name.length())
			return {};

		sz end_check = src_length - macro_name.length() + 1;

		F_str_state str_state;
		for(sz i = 0; i < end_check;) {

			str_state.begin_check(src_content[i]);

			{
				b8 is_march = true;
				sz j = i;
				sz j_end = i + macro_name.length();
				for(; j < j_end; ++j)
				{
					if(src_content[j] != macro_name[j - i]) {

						is_march = false;
						break;
					}
				}

				if(is_march) {

					b8 is_left_correct = true;
					b8 is_right_correct = true;

					if(i > 0) {
						is_left_correct = !is_variable_name_character(src_content[i - 1]);
					}
					if(j_end < src_length) {
						is_right_correct = !is_variable_name_character(src_content[j_end]);
					}

					if(is_left_correct && is_right_correct) {

						result.push_back(i);

						i = j_end;
						continue;
					}
				}
			}

			str_state.end_check();
			++i;
		}

		return result;
	}
	G_string H_nsl_utilities::apply_variable_macro_uses(
		const G_string& src_content,
		const G_string& macro_name,
		const G_string& macro_result,
		const TG_vector<H_nsl_utilities::F_variable_macro_use>& uses
	) {
		G_string result;
		result.reserve(src_content.length());

		sz src_length = src_content.length();
		sz macro_name_length = macro_name.length();

		sz begin_location = 0;
		for(sz index : uses) {

			result += src_content.substr(begin_location, index - begin_location);
			result += macro_result;

			begin_location = index + macro_name_length;
		}

		result += src_content.substr(begin_location, src_length - begin_location);

		return std::move(result);
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



	H_nsl_tools::F_preprocessed_src H_nsl_tools::preprocess_src(const G_string& src_content) {

		return {
			.content = H_nsl_utilities::remove_comments(src_content)
		};
	}
	eastl::optional<TG_vector<H_nsl_tools::F_kernel_definition>> H_nsl_tools::find_kernel_definitions(
		const H_nsl_tools::F_preprocessed_src& src,
		G_string* error_p
	) {
		TG_vector<F_kernel_definition> kernel_definitions;

		// vertex shader
		{
			auto nsl_vs_uses = H_nsl_utilities::find_function_macro_uses(
				src.content,
				"NSL_VERTEX_SHADER"
			);
			sz vs_kernel_count = nsl_vs_uses.size();

			for(sz i = 0; i < vs_kernel_count; ++i) {

				const auto& use = nsl_vs_uses[i];

				auto trees_opt = H_nsl_utilities::build_info_trees(use.arg);

				if(trees_opt) {
					const auto& trees = trees_opt.value();

					if(trees.size() == 0) {
						if(error_p)
							*error_p = "vertex shader name is required";
						return eastl::nullopt;
					}

					// get info tree and macro definition trees
					H_nsl_utilities::F_info_tree info_tree = trees[0];
					TG_vector<H_nsl_utilities::F_info_tree> macro_definition_trees(
						trees.begin() + 1,
						trees.end()
					);

					// push back to result list
					kernel_definitions.push_back({
						.use = use,
						.info_tree = info_tree,
						.macro_definition_trees = macro_definition_trees,
						.shader_type = E_shader_type::VERTEX
					});
				}
				else {
					if(error_p)
						*error_p = "can't process vertex shader definition args: NSL_VERTEX_SHADER(" + use.arg + ")";
					return eastl::nullopt;
				}
			}
		}

		// pixel shader
		{
			auto nsl_vs_uses = H_nsl_utilities::find_function_macro_uses(
				src.content,
				"NSL_PIXEL_SHADER"
			);
			sz vs_kernel_count = nsl_vs_uses.size();

			for(sz i = 0; i < vs_kernel_count; ++i) {

				const auto& use = nsl_vs_uses[i];

				auto trees_opt = H_nsl_utilities::build_info_trees(use.arg);

				if(trees_opt) {
					const auto& trees = trees_opt.value();

					if(trees.size() == 0) {
						if(error_p)
							*error_p = "pixel shader name is required";
						return eastl::nullopt;
					}

					// get info tree and macro definition trees
					H_nsl_utilities::F_info_tree info_tree = trees[0];
					TG_vector<H_nsl_utilities::F_info_tree> macro_definition_trees(
						trees.begin() + 1,
						trees.end()
					);

					// push back to result list
					kernel_definitions.push_back({
						.use = use,
						.info_tree = info_tree,
						.macro_definition_trees = macro_definition_trees,
						.shader_type = E_shader_type::PIXEL
					});
				}
				else {
					if(error_p)
						*error_p = "can't process pixel shader definition args: NSL_PIXEL_SHADER(" + use.arg + ")";
					return eastl::nullopt;
				}
			}
		}

		// compute shader
		{
			auto nsl_vs_uses = H_nsl_utilities::find_function_macro_uses(
				src.content,
				"NSL_COMPUTE_SHADER"
			);
			sz vs_kernel_count = nsl_vs_uses.size();

			for(sz i = 0; i < vs_kernel_count; ++i) {

				const auto& use = nsl_vs_uses[i];

				auto trees_opt = H_nsl_utilities::build_info_trees(use.arg);

				if(trees_opt) {
					const auto& trees = trees_opt.value();

					if(trees.size() == 0) {
						if(error_p)
							*error_p = "compute shader name is required";
						return eastl::nullopt;
					}

					// get info tree and macro definition trees
					H_nsl_utilities::F_info_tree info_tree = trees[0];
					TG_vector<H_nsl_utilities::F_info_tree> macro_definition_trees(
						trees.begin() + 1,
						trees.end()
					);

					// push back to result list
					kernel_definitions.push_back({
						.use = use,
						.info_tree = info_tree,
						.macro_definition_trees = macro_definition_trees,
						.shader_type = E_shader_type::COMPUTE
					});
				}
				else {
					if(error_p)
						*error_p = "can't process compute shader definition args: NSL_COMPUTE_SHADER(" + use.arg + ")";
					return eastl::nullopt;
				}
			}
		}

		return std::move(kernel_definitions);
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