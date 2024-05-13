#include <nrhi/nsl_shader_compiler.hpp>



namespace nrhi {

	void F_nsl_str_state::begin_check(char c) {

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
	void F_nsl_str_state::end_check() {

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

	eastl::optional<TG_vector<F_nsl_info_tree>> H_nsl_utilities::build_info_trees(
		const G_string& src_content,
		sz location_offset_to_save,
		F_nsl_error_stack* error_stack_p
	) {

		TG_vector<F_nsl_info_tree> trees;

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
			sz begin_arg_location = i;
			sz end_arg_location = i;

			// name
			while(
				(i < src_length)
			) {
				if(
					!(
						is_variable_name_character(src_content[i])
						|| (src_content[i] == '.')
						|| (src_content[i] == ':')
						|| (src_content[i] == '>')
						|| (src_content[i] == '<')
						|| (src_content[i] == '+')
						|| (src_content[i] == '-')
						|| (src_content[i] == '*')
						|| (src_content[i] == '/')
						|| (src_content[i] == '\\')
						|| (src_content[i] == '|')
						|| (src_content[i] == '@')
						|| (src_content[i] == '!')
						|| (src_content[i] == '#')
						|| (src_content[i] == '$')
						|| (src_content[i] == '^')
						|| (src_content[i] == '&')
						|| (src_content[i] == ';')
						|| (src_content[i] == '~')
					)
				) {
					break;
				}
				++i;
			}
			b8 is_name_found = (i != begin_name_location);

			b8 is_name_as_str = false;
			F_nsl_str_state str_state;

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
				TG_vector<F_nsl_info_tree> childs;
				G_string child_src_content;

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

						i32 level = 1;

						begin_arg_location = t + 1;
						end_arg_location = begin_arg_location;

						F_nsl_str_state arg_str_state;

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

								end_arg_location = k;

								G_string childs_src = src_content.substr(begin_arg_location, end_arg_location - begin_arg_location);

								auto childs_opt = build_info_trees(childs_src, location_offset_to_save + begin_arg_location, error_stack_p);
								if(!childs_opt)
								{
									NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
										error_stack_p,
										location_offset_to_save + begin_arg_location, "can't build childs"
									);
									return eastl::nullopt;
								}

								childs = childs_opt.value();
								child_src_content = std::move(childs_src);

								i = end_arg_location + 1;
								break;
							}

							arg_str_state.end_check();
						}
					}
				}

				// push back current tree
				trees.push_back({
					.name = name,
					.childs = childs,
					.child_src_content = child_src_content,

					.begin_location = location_offset_to_save + begin_name_location,
					.end_location = location_offset_to_save + end_arg_location + 1,
					.begin_name_location = location_offset_to_save + begin_name_location,
					.end_name_location = location_offset_to_save + end_name_location,
					.begin_childs_location = location_offset_to_save + begin_arg_location,
					.end_childs_location = location_offset_to_save + end_arg_location
				});
			}
			else {
				// invalid src content
				if(
					(end_name_location != src_length)
					|| str_state.value
				)
				{
					if(str_state.value) {
						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							error_stack_p,
							location_offset_to_save + begin_name_location,
							"string is not closed: " + src_content.substr(begin_name_location, src_length - begin_name_location)
						);
					}
					else {
						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							error_stack_p,
							location_offset_to_save + begin_name_location,
							G_string("invalid character in variable name: '") + src_content[begin_name_location] + "'")
						;
					}
					return eastl::nullopt;
				}
			}
		}

		return std::move(trees);
	}

	eastl::optional<TG_vector<F_nsl_use>> H_nsl_utilities::find_uses(
		const ncpp::containers::G_string& src_content,
		F_nsl_error_stack* error_stack_p
	)
	{
		TG_vector<F_nsl_use> result;

		sz src_length = src_content.length();

		if(src_length < 2)
			return {};

		F_nsl_str_state str_state;
		for(sz i = 0; i < src_length;) {

			str_state.begin_check(src_content[i]);

			if(
				!(str_state.value)
				&& (src_content[i] == '@')
			)
			{
				sz begin_location = i;
				sz end_location = i;
				sz begin_name_location = i + 1;
				sz end_name_location = begin_name_location;

				sz j = begin_name_location;
				for(; j < src_length; ++j)
				{
					if(
						!(
							is_variable_name_character(src_content[j])
							|| src_content[j] == '.'
						)
					)
					{
						break;
					}
				}
				end_name_location = j;

				if(begin_name_location != end_name_location) {

					b8 is_left_correct = true;
					b8 is_right_correct = true;

					if(i > 0) {
						is_left_correct = !is_variable_name_character(src_content[i - 1]);
					}
					if(end_name_location < src_length) {
						is_right_correct = !is_variable_name_character(src_content[end_name_location]);
					}

					if(is_left_correct && is_right_correct) {

						G_string name = src_content.substr(begin_name_location, end_name_location - begin_name_location);

						sz t = end_name_location;
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

								i32 level = 1;

								sz begin_arg_location = t + 1;
								sz end_arg_location = begin_arg_location;

								F_nsl_str_state arg_str_state;

								// check for function-like use
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

										end_arg_location = k;
										end_location = k + 1;

										result.push_back({
											.name = name,
											.arg = src_content.substr(begin_arg_location, end_arg_location - begin_arg_location),

											.begin_location = begin_location,
											.end_location = end_location,
											.begin_name_location = begin_name_location,
											.end_name_location = end_name_location,
											.begin_arg_location = begin_arg_location,
											.end_arg_location = end_arg_location
										});

										i = end_location;

										goto NRHI_NEXT_CHARACTER_CHECK;
									}

									arg_str_state.end_check();
								}
							}
						}

						// this is variable-like use
						end_location = t;
						result.push_back({
							.name = name,
							.arg = "",

							.begin_location = begin_location,
							.end_location = end_name_location,
							.begin_name_location = begin_name_location,
							.end_name_location = end_name_location,
							.begin_arg_location = end_name_location,
							.end_arg_location = end_name_location
						});

						i = end_location;

						goto NRHI_NEXT_CHARACTER_CHECK;
					}
				}
				else {
					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						error_stack_p,
						begin_location,
						"use name is required"
					);
					return eastl::nullopt;
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
	eastl::optional<G_string> H_nsl_utilities::apply_uses(
		const ncpp::containers::G_string& src_content,
		const TG_vector<F_nsl_use>& uses,
		const nrhi::F_nsl_use_result_functor& result_functor,
		F_nsl_error_stack* error_stack_p
	)
	{
		G_string result;
		result.reserve(src_content.length());

		sz src_length = src_content.length();

		sz begin_location = 0;
		sz index = 0;
		for(const auto& use : uses) {

			result += src_content.substr(begin_location, use.begin_location - begin_location);
			result += result_functor(use, index);

			begin_location = use.end_location;
			++index;
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

	eastl::optional<TG_pack<G_string, TG_vector<sz>>> H_nsl_utilities::remove_comments(
		const G_string& src_content,
		F_nsl_error_stack* error_stack_p
	)
	{

		sz src_length = src_content.length();

		G_string result;
		result.reserve(src_length);
		TG_vector<sz> locations;
		locations.reserve(src_length);

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
			{
				result.push_back(src_content[i]);
				locations.push_back(i);
			}

			if(is_in_comment_1) {

				if(src_content[i] == '\n')
				{
					result.push_back('\n');
					locations.push_back(i);
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

		if(is_in_comment_2) {

			if(error_stack_p)
				error_stack_p->push({
					"multi-line comment is not closed"
					-1,
					src_length,
				});

			return eastl::nullopt;
		}

		return TG_pack<G_string, TG_vector<sz>> {
			std::move(result),
			std::move(locations)
		};
	}



	F_nsl_shader_header_loader::F_nsl_shader_header_loader() {
	}
	F_nsl_shader_header_loader::~F_nsl_shader_header_loader() {
	}

	eastl::optional<G_string> F_nsl_shader_header_loader::load(const G_string& path)
	{
		return eastl::nullopt;
	}



	F_nsl_shader_compiler::F_nsl_shader_compiler() :
		header_loader_p_(TU<F_nsl_shader_header_loader>()())
	{
	}
	F_nsl_shader_compiler::F_nsl_shader_compiler(TU<F_nsl_shader_header_loader>&& header_loader_p) :
		header_loader_p_(std::move(header_loader_p))
	{
	}
	F_nsl_shader_compiler::~F_nsl_shader_compiler() {
	}

	eastl::optional<F_nsl_preprocessed_src> F_nsl_shader_compiler::include_src(
		const F_nsl_preprocessed_src& current_src,
		const G_string& path,
		F_nsl_error_stack* error_stack_p
	) {
		return eastl::nullopt;
	}
	eastl::optional<F_nsl_preprocessed_src> F_nsl_shader_compiler::preprocess_src(
		const G_string& src_content,
		const G_string& abs_path,
		F_nsl_error_stack* error_stack_p
	) {
		auto comment_removed_src_opt = H_nsl_utilities::remove_comments(src_content, error_stack_p);

		if(comment_removed_src_opt) {

			auto comment_removed_src = comment_removed_src_opt.value();

			return F_nsl_preprocessed_src {
				.content = comment_removed_src.first(),
				.locations = comment_removed_src.second(),
				.abs_path = abs_path
			};
		}
		else {

			if(error_stack_p)
				error_stack_p->push({
					"can't remove comments"
					-1,
					0
				});

			return eastl::nullopt;
		}
	}

}