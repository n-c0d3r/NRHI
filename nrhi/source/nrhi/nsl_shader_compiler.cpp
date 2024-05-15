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

			if(!is_special_character) {
				if(c == '\\')
				{
					is_special_character = true;
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
			else {
				is_special_character = false;
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

	eastl::optional<F_nsl_ast_tree_try_build_result> H_nsl_utilities::try_build_use(
		const F_nsl_ast_tree_try_build_input& input
	) {

		const auto& src_content = input.src_content;
		auto src_length = src_content.length();

		auto error_stack_p = input.error_stack_p;

		const auto& str_state = input.str_state;

		auto current_location = input.current_location;
		auto location_offset_to_save = input.location_offset_to_save;



		F_nsl_ast_tree_try_build_result result;

		auto& trees = result.trees;

		auto& begin_location = result.begin_location;
		begin_location = input.begin_location;

		auto& end_location = result.end_location;
		end_location = input.end_location;



		if(
			!(str_state.value)
			&& (src_content[current_location] == '@')
		)
		{
			sz temp_begin_location = current_location;
			sz temp_end_location = current_location;
			sz begin_name_location = current_location + 1;
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

				if(current_location > 0) {
					is_left_correct = !is_variable_name_character(src_content[current_location - 1]);
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

							sz begin_str_location = 0;

							// check for function-like use
							sz k = begin_arg_location;
							for(; k < src_length; ++k) {

								arg_str_state.begin_check(src_content[k]);

								if(
									arg_str_state.value
									&& !arg_str_state.prev_value
								)
									begin_str_location = k;

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
									temp_end_location = k + 1;

									begin_location = temp_begin_location;
									end_location = temp_end_location;

									F_nsl_use use = {
										.name = name,
										.arg = src_content.substr(begin_arg_location, end_arg_location - begin_arg_location),

										.begin_location = begin_location,
										.end_location = end_location,
										.begin_name_location = begin_name_location,
										.end_name_location = end_name_location,
										.begin_arg_location = begin_arg_location,
										.end_arg_location = end_arg_location
									};

									F_nsl_ast_tree tree = {
										.type = E_nsl_ast_tree_type::USE,
										.use = use,
										.begin_location = begin_location,
										.end_location = end_location,
									};
									trees.push_back(tree);

									goto NRHI_END_TRY_BUILD_AST_TREE;
								}

								arg_str_state.end_check();
							}

							if(arg_str_state.value) {

								NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
									error_stack_p,
									location_offset_to_save + begin_str_location,
									"string is not ended"
								);
								return eastl::nullopt;
							}

							NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
								error_stack_p,
								location_offset_to_save + begin_arg_location,
								"function-like use scope is not ended"
							);
							return eastl::nullopt;
						}
						// for function-like use with {}
						else if(src_content[t] == '{') {

							i32 level = 1;

							sz begin_arg_location = t + 1;
							sz end_arg_location = begin_arg_location;

							F_nsl_str_state arg_str_state;

							sz begin_str_location = 0;

							// check for function-like use
							sz k = begin_arg_location;
							for(; k < src_length; ++k) {

								arg_str_state.begin_check(src_content[k]);

								if(
									arg_str_state.value
									&& !arg_str_state.prev_value
									)
									begin_str_location = k;

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
									temp_end_location = k + 1;

									begin_location = temp_begin_location;
									end_location = temp_end_location;

									F_nsl_use use = {
										.name = name,
										.arg = src_content.substr(begin_arg_location, end_arg_location - begin_arg_location),

										.begin_location = begin_location,
										.end_location = end_location,
										.begin_name_location = begin_name_location,
										.end_name_location = end_name_location,
										.begin_arg_location = begin_arg_location,
										.end_arg_location = end_arg_location
									};

									F_nsl_ast_tree tree = {
										.type = E_nsl_ast_tree_type::USE,
										.use = use,
										.begin_location = begin_location,
										.end_location = end_location,
									};
									trees.push_back(tree);

									goto NRHI_END_TRY_BUILD_AST_TREE;
								}

								arg_str_state.end_check();
							}

							if(arg_str_state.value) {

								NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
									error_stack_p,
									location_offset_to_save + begin_str_location,
									"string is not ended"
								);
								return eastl::nullopt;
							}

							NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
								error_stack_p,
								location_offset_to_save + begin_arg_location,
								"function-like use scope is not ended"
							);
							return eastl::nullopt;
						}
					}

					// this is variable-like use
					temp_end_location = end_name_location;

					begin_location = temp_begin_location;
					end_location = temp_end_location;

					F_nsl_use use = {
						.name = name,
						.arg = "",

						.begin_location = begin_location,
						.end_location = end_location,
						.begin_name_location = begin_name_location,
						.end_name_location = end_name_location,
						.begin_arg_location = end_name_location,
						.end_arg_location = end_name_location
					};

					F_nsl_ast_tree tree = {
						.type = E_nsl_ast_tree_type::USE,
						.use = use,
						.begin_location = begin_location,
						.end_location = end_location,
					};
					trees.push_back(tree);

					goto NRHI_END_TRY_BUILD_AST_TREE;
				}
			}
			else {
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p,
					location_offset_to_save + begin_location,
					"use name is required"
				);
				return eastl::nullopt;
			}
		}

		NRHI_END_TRY_BUILD_AST_TREE:
		return std::move(result);
	}
	eastl::optional<F_nsl_ast_tree_try_build_result> H_nsl_utilities::try_build_object_implementation(
		const G_string& keyword,
		b8 is_object_name_required,
		sz min_object_body_count,
		sz max_object_body_count,
		const F_nsl_ast_tree_try_build_input& input
	) {

		const auto& src_content = input.src_content;
		auto src_length = src_content.length();

		auto error_stack_p = input.error_stack_p;

		const auto& str_state = input.str_state;

		auto current_location = input.current_location;
		auto location_offset_to_save = input.location_offset_to_save;



		F_nsl_ast_tree_try_build_result result;

		auto& trees = result.trees;

		auto& begin_location = result.begin_location;
		begin_location = input.begin_location;

		auto& end_location = result.end_location;
		end_location = input.end_location;



		if(
			!(str_state.value)
			&& (current_location + keyword.length() <= src_length)
		) {
			sz temp_begin_location = current_location;
			sz temp_end_location = temp_begin_location;
			sz begin_keyword_location = current_location;
			sz end_keyword_location = begin_keyword_location;

			sz j = begin_keyword_location;
			for(; j < src_length; ++j)
			{
				if(
					!(src_content[j] == keyword[j - begin_keyword_location])
				)
				{
					break;
				}
			}
			end_keyword_location = j;

			if(
				(end_keyword_location - begin_keyword_location) == keyword.length()
			) {

				b8 is_left_correct = true;
				b8 is_right_correct = true;

				if(current_location > 0) {
					is_left_correct = !is_variable_name_character(src_content[current_location - 1]);
				}
				if(end_keyword_location < src_length) {
					is_right_correct = !is_variable_name_character(src_content[end_keyword_location]);
				}

				if(is_left_correct && is_right_correct) {

					sz j2 = end_keyword_location;
					for(; j2 < src_length; ++j2)
					{
						if(
							!(
								(src_content[j2] == ' ')
								|| (src_content[j2] == '\t')
								|| (src_content[j2] == '\n')
								|| (src_content[j2] == '\r')
							)
						)
						{
							break;
						}
					}

					sz begin_name_location = j2;

					sz j3 = begin_name_location;
					for(; j3 < src_length; ++j3)
					{
						if(
							!(
								is_variable_name_character(src_content[j3])
								|| (src_content[j3] == '.')
							)
						)
						{
							break;
						}
					}
					sz end_name_location = j3;

					if(is_object_name_required && (begin_name_location == end_name_location)) {

						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							error_stack_p,
							location_offset_to_save + begin_name_location,
							"object name is required"
						);

						return eastl::nullopt;
					}

					temp_end_location = end_name_location;

					G_string name = src_content.substr(begin_name_location, end_name_location - begin_name_location);

					sz begin_body_location = end_name_location;
					sz end_body_location = begin_body_location;

					TG_vector<F_nsl_object_implementation_body> bodies;

					sz detected_object_body_count = 0;
					for(sz object_body_index = 0; object_body_index < max_object_body_count; ++object_body_index)
					{
						sz t = temp_end_location;
						for (; t < src_length; ++t)
						{
							if (
								!(
									(src_content[t] == ' ')
									|| (src_content[t] == '\t')
									|| (src_content[t] == '\n')
									|| (src_content[t] == '\r')
								)
							)
							{
								break;
							}
						}

						if (t < src_length)
						{
							char open_body_character = '(';
							char close_body_character = ')';

							if (src_content[t] == '{') {

								open_body_character = '{';
								close_body_character = '}';
							}

							if (src_content[t] == open_body_character)
							{
								i32 level = 1;

								begin_body_location = t + 1;
								end_body_location = begin_body_location;

								F_nsl_str_state body_str_state;

								sz begin_str_location = 0;

								b8 is_body_ended = false;

								// check for function-like use
								sz k = begin_body_location;
								for (; k < src_length; ++k)
								{
									body_str_state.begin_check(src_content[k]);

									if (
										body_str_state.value
										&& !body_str_state.prev_value
									)
										begin_str_location = k;

									if (
										!(body_str_state.value)
										&& (src_content[k] == open_body_character)
									)
										++level;

									if (
										!(body_str_state.value)
										&& (src_content[k] == close_body_character)
									)
										--level;

									if (level == 0)
									{
										end_body_location = k;
										temp_end_location = k + 1;

										bodies.push_back({
											.content = src_content.substr(
												begin_body_location,
												end_body_location - begin_body_location
											),
											.begin_location = begin_body_location,
											.end_location = end_body_location
										});
										is_body_ended = true;

										break;
									}

									body_str_state.end_check();
								}

								if(!is_body_ended)
								{
									if (body_str_state.value)
									{
										NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
											error_stack_p,
											location_offset_to_save + begin_str_location,
											"string is not ended"
										);
										return eastl::nullopt;
									}

									NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
										error_stack_p,
										location_offset_to_save + begin_body_location,
										"function-like use scope is not ended"
									);

									return eastl::nullopt;
								}
							}
							else break;
						}
					}

					if(bodies.size() < min_object_body_count) {

						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							error_stack_p,
							location_offset_to_save + begin_name_location,
							"object body is required to be >= " + G_to_string(min_object_body_count)
						);

						return eastl::nullopt;
					}

					begin_location = temp_begin_location;
					end_location = temp_end_location;

					F_nsl_object_implementation object_implementation = {
						.keyword = keyword,
						.name = name,

						.begin_location = begin_location,
						.end_location = end_location,
						.begin_keyword_location = begin_keyword_location,
						.end_keyword_location = end_keyword_location,
						.begin_name_location = begin_name_location,
						.end_name_location = end_name_location,

						.bodies = bodies
					};

					F_nsl_ast_tree tree = {
						.type = E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION,
						.object_implementation = object_implementation,
						.begin_location = begin_location,
						.end_location = end_location,
					};
					trees.push_back(tree);

					goto NRHI_END_TRY_BUILD_AST_TREE;
				}
			}
		}

		NRHI_END_TRY_BUILD_AST_TREE:
		return std::move(result);
	}
	F_nsl_ast_tree_try_build_functor H_nsl_utilities::make_try_build_object_implementation_functor(
		const G_string& keyword,
		b8 is_object_name_required,
		sz min_object_body_count,
		sz max_object_body_count
	) {
		NCPP_ASSERT(keyword.length() > 0) << "invalid object type name";
		return [
			keyword,
			is_object_name_required,
			min_object_body_count,
			max_object_body_count
		](const F_nsl_ast_tree_try_build_input& input) -> eastl::optional<F_nsl_ast_tree_try_build_result> {
			return H_nsl_utilities::try_build_object_implementation(
				keyword,
				is_object_name_required,
				min_object_body_count,
				max_object_body_count,
				input
			);
		};
	}
	eastl::optional<TG_vector<F_nsl_ast_tree>> H_nsl_utilities::build_ast_trees(
		const G_string& src_content,
		const TG_vector<F_nsl_ast_tree_try_build_functor>& try_build_functors,
		const F_nsl_ast_tree_recursive_build_functor& resursive_build_functor,
		sz location_offset_to_save,
		F_nsl_error_stack* error_stack_p
	) {
		TG_vector<F_nsl_ast_tree> result;

		sz src_length = src_content.length();

		sz begin_location = 0;
		sz end_location = 0;

		sz tree_index = 0;

		F_nsl_str_state str_state;
		for(sz i = 0; i < src_length;) {

			str_state.begin_check(src_content[i]);

			for(const auto& try_build_functor : try_build_functors) {

				F_nsl_ast_tree_try_build_input input = {
					.src_content = src_content,
					.error_stack_p = error_stack_p,

					.str_state = str_state,

					.current_location = i,

					.begin_location = begin_location,
					.end_location = end_location,

					.location_offset_to_save = location_offset_to_save
				};

				auto result_opt = try_build_functor(input);

				if(!result_opt)
					return eastl::nullopt;

				auto try_build_result = result_opt.value();

				if(try_build_result.trees.size()) {

					for(auto& tree : try_build_result.trees) {

						if(tree.begin_location != end_location) {

							begin_location = end_location;
							end_location = tree.begin_location;

							F_nsl_plain_text plain_text = {
								.content = src_content.substr(begin_location, end_location - begin_location),
								.begin_location = begin_location,
								.end_location = end_location,
							};

							F_nsl_ast_tree plain_text_tree = {
								.type = E_nsl_ast_tree_type::PLAIN_TEXT,
								.plain_text = plain_text,
								.begin_location = location_offset_to_save + begin_location,
								.end_location = location_offset_to_save + end_location
							};

							auto childs_opt = resursive_build_functor(
								src_content.substr(begin_location, end_location - begin_location),
								location_offset_to_save + begin_location,
								plain_text_tree,
								tree_index,
								error_stack_p
							);
							if(!childs_opt) {
								NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
									error_stack_p,
									location_offset_to_save + begin_location,
									"can't build child ast trees"
								);
							}

							plain_text_tree.childs = childs_opt.value();

							result.push_back(tree);
							++tree_index;
						}

						begin_location = tree.begin_location;
						end_location = tree.end_location;

						auto childs_opt = resursive_build_functor(
							src_content.substr(begin_location, end_location - begin_location),
							location_offset_to_save + begin_location,
							tree,
							tree_index,
							error_stack_p
						);
						if(!childs_opt) {
							NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
								error_stack_p,
								location_offset_to_save + begin_location,
								"can't build child ast trees"
							);
						}

						tree.childs = childs_opt.value();

						result.push_back(tree);
						++tree_index;
					}

					i = end_location;
					goto NRHI_NEXT_CHARACTER_CHECK;
				}
			}

			str_state.end_check();
			++i;
			continue;

		NRHI_NEXT_CHARACTER_CHECK:
			str_state.end_check();
		}



		// try push back last plain text
		if(end_location != src_length) {

			begin_location = end_location;
			end_location = src_length;

			F_nsl_plain_text plain_text = {
				.content = src_content.substr(begin_location, end_location - begin_location),
				.begin_location = begin_location,
				.end_location = end_location,
			};

			F_nsl_ast_tree tree = {
				.type = E_nsl_ast_tree_type::PLAIN_TEXT,
				.plain_text = plain_text,
				.begin_location = location_offset_to_save + begin_location,
				.end_location = location_offset_to_save + end_location
			};

			auto childs_opt = resursive_build_functor(
				src_content.substr(begin_location, end_location - begin_location),
				location_offset_to_save + begin_location,
				tree,
				tree_index,
				error_stack_p
			);
			if(!childs_opt) {
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p,
					location_offset_to_save + begin_location,
					"can't build child ast trees"
				);
			}

			tree.childs = childs_opt.value();

			result.push_back(tree);
			++tree_index;
		}

		return result;
	}
	eastl::optional<TG_vector<F_nsl_ast_tree>> H_nsl_utilities::build_ast_trees(
		const ncpp::containers::G_string& src_content,
		const F_nsl_ast_tree_recursive_build_functor& resursive_build_functor,
		sz location_offset_to_save,
		F_nsl_error_stack* error_stack_p
	)
	{
		return build_ast_trees(
			src_content,
			{
				// for syntax:
				// + function-like use: @keyword()
				// + variable-like use: @keyword
				&H_nsl_utilities::try_build_use
			},
			resursive_build_functor,
			location_offset_to_save,
			error_stack_p
		);
	}
	eastl::optional<G_string> H_nsl_utilities::apply_ast_trees(
		const ncpp::containers::G_string& src_content,
		const TG_vector<F_nsl_ast_tree>& trees,
		const nrhi::F_nsl_ast_tree_result_functor& result_functor,
		F_nsl_error_stack* error_stack_p
	)
	{
		G_string result;
		result.reserve(src_content.length());

		sz src_length = src_content.length();

		sz index = 0;
		for(const auto& tree : trees) {

			result += result_functor(src_content, tree, index);
			++index;
		}

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
		TG_vector<sz> raw_locations;
		raw_locations.reserve(src_length);

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
				raw_locations.push_back(i);
			}

			if(is_in_comment_1) {

				if(src_content[i] == '\n')
				{
					result.push_back('\n');
					raw_locations.push_back(i);
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
			std::move(raw_locations)
		};
	}



	F_nsl_shader_module_manager::F_nsl_shader_module_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_shader_module_manager::~F_nsl_shader_module_manager() {
	}

	eastl::optional<F_nsl_shader_module_manager::F_load_src_content_result> F_nsl_shader_module_manager::load_src_content(
		TKPA_valid<F_nsl_translation_unit> from_unit_p,
		const G_string& path,
		const F_nsl_ast_tree& tree,
		G_string& out_src_content
	)
	{
		return eastl::nullopt;
	}
	TK<F_nsl_translation_unit> F_nsl_shader_module_manager::load(
		TKPA_valid<F_nsl_translation_unit> from_unit_p,
		const G_string& path,
		const F_nsl_ast_tree& tree
	)
	{
		G_string src_content;
		auto load_result_opt = load_src_content(
			from_unit_p,
			path,
			tree,
			src_content
		);

		if(load_result_opt) {

			const auto& load_result = load_result_opt.value();
			const auto& abs_path = load_result.abs_path;

			auto it = abs_path_to_translation_unit_p_.find(abs_path);

			if(it == abs_path_to_translation_unit_p_.end()) {

				return shader_compiler_p_->translation_unit_manager_p()->create_unit(
					src_content,
					abs_path
				).no_requirements();
			}
			else {

				return it->second;
			}
		}

		return null;
	}



	A_nsl_object::A_nsl_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		const G_string& name
	) :
		shader_compiler_p_(shader_compiler_p),
		type_p_(type_p),
		name_(name)
	{
	}
	A_nsl_object::~A_nsl_object() {
	}



	A_nsl_object_type::A_nsl_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		const G_string& name,
		b8 is_object_name_required,
		sz min_object_body_count,
		sz max_object_body_count
	) :
		shader_compiler_p_(shader_compiler_p),
		name_(name),
		is_object_name_required_(is_object_name_required),
		min_object_body_count_(min_object_body_count),
		max_object_body_count_(max_object_body_count)
	{
	}
	A_nsl_object_type::~A_nsl_object_type() {
	}



	A_nsl_item_object::A_nsl_item_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			name
		)
	{
	}
	A_nsl_item_object::~A_nsl_item_object() {

		set_parent_p(null);
	}

	void A_nsl_item_object::set_parent_p(F_null)
	{
		if(parent_p_)
		{
			parent_p_->remove_child_internal(NCPP_KTHIS());
		}
		parent_p_ = null;
	}
	void A_nsl_item_object::set_parent_p(TKPA<A_nsl_item_object> parent_p)
	{
		if(parent_p_)
		{
			parent_p_->remove_child_internal(NCPP_KTHIS());
		}

		if(parent_p)
			parent_p->add_child_internal(NCPP_KTHIS());

		parent_p_ = parent_p;
	}
	void A_nsl_item_object::set_parent_p(TKPA_valid<A_nsl_item_object> parent_p)
	{
		if(parent_p_)
		{
			parent_p_->remove_child_internal(NCPP_KTHIS());
		}

		parent_p->add_child_internal(NCPP_KTHIS());

		parent_p_ = parent_p.no_requirements();
	}

	void A_nsl_item_object::add_child_internal(TKPA_valid<A_nsl_item_object> child_p)
	{
		child_p_list_.push_back(child_p);
		child_p->handle_ = --(child_p_list_.end());
	}
	void A_nsl_item_object::remove_child_internal(TKPA_valid<A_nsl_item_object> child_p)
	{
		child_p_list_.erase(child_p->handle_);
	}



	F_nsl_namespace_object::F_nsl_namespace_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		const G_string& name
	) :
		A_nsl_item_object(
			shader_compiler_p,
			type_p,
			name
		)
	{
	}
	F_nsl_namespace_object::~F_nsl_namespace_object() {
	}



	F_nsl_namespace_object_type::F_nsl_namespace_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"namespace",
			true,
			1,
			1
		)
	{
	}
	F_nsl_namespace_object_type::~F_nsl_namespace_object_type() {
	}

	TU<A_nsl_object> F_nsl_namespace_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = TU<F_nsl_namespace_object>()(
			shader_compiler_p(),
			NCPP_KTHIS(),
			tree.object_implementation.name
		);

		tree.object_implementation.attached_object_p = object_p;

		return std::move(object_p);
	}



	F_nsl_import_object::F_nsl_import_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			name
		)
	{
	}
	F_nsl_import_object::~F_nsl_import_object() {
	}



	F_nsl_import_object_type::F_nsl_import_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"import",
			true,
			1,
			1
		)
	{
	}
	F_nsl_import_object_type::~F_nsl_import_object_type() {
	}

	TU<A_nsl_object> F_nsl_import_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = TU<F_nsl_import_object>()(
			shader_compiler_p(),
			NCPP_KTHIS(),
			tree.object_implementation.name
		);

		tree.object_implementation.attached_object_p = object_p;

		return std::move(object_p);
	}



	F_nsl_type_object::F_nsl_type_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		const G_string& name
	) :
		A_nsl_item_object(
			shader_compiler_p,
			type_p,
			name
		)
	{
	}
	F_nsl_type_object::~F_nsl_type_object() {
	}



	F_nsl_alias_object::F_nsl_alias_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			name
		)
	{
	}
	F_nsl_alias_object::~F_nsl_alias_object() {
	}



	F_nsl_alias_object_type::F_nsl_alias_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"alias",
			true,
			1,
			1
		)
	{
	}
	F_nsl_alias_object_type::~F_nsl_alias_object_type() {
	}

	TU<A_nsl_object> F_nsl_alias_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = TU<F_nsl_alias_object>()(
			shader_compiler_p(),
			NCPP_KTHIS(),
			tree.object_implementation.name
		);

		tree.object_implementation.attached_object_p = object_p;

		return std::move(object_p);
	}



	F_nsl_object_manager::F_nsl_object_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
		register_type(
			TU<F_nsl_namespace_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_import_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_alias_object_type>()(shader_compiler_p_)
		);
	}
	F_nsl_object_manager::~F_nsl_object_manager() {
	}

	TK_valid<A_nsl_object_type> F_nsl_object_manager::register_type(TU<A_nsl_object_type>&& object_type_p) {

		auto keyed_object_type_p = NCPP_FOH_VALID(object_type_p);

		type_p_map_[keyed_object_type_p->name()] = std::move(object_type_p);

		return keyed_object_type_p;
	}

	TG_vector<F_nsl_ast_tree_try_build_functor> F_nsl_object_manager::ast_tree_try_build_functors() {

		TG_vector<F_nsl_ast_tree_try_build_functor> result;
		result.reserve(type_p_map_.size());

		for(const auto& it : type_p_map_) {

			auto object_type_p = NCPP_FOH_VALID(it.second);
			result.push_back(
				H_nsl_utilities::make_try_build_object_implementation_functor(
					object_type_p->name(),
					object_type_p->is_object_name_required(),
					object_type_p->min_object_body_count(),
					object_type_p->max_object_body_count()
				)
			);
		}

		return std::move(result);
	}



	F_nsl_error_group::F_nsl_error_group(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		const G_string& abs_path
	) :
		shader_compiler_p_(shader_compiler_p),
		abs_path_(abs_path)
	{
	}
	F_nsl_error_group::~F_nsl_error_group()
	{
	}



	F_nsl_error_storage::F_nsl_error_storage(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_error_storage::~F_nsl_error_storage() {
	}

	TK_valid<F_nsl_error_group> F_nsl_error_storage::optain_group(const G_string& abs_path) {

		auto group_p = TU<F_nsl_error_group>()(
			shader_compiler_p(),
			abs_path
		);

		auto keyed_group_p = NCPP_FOH_VALID(group_p);

		group_p_stack_.push(std::move(group_p));

		return keyed_group_p;
	}



	F_nsl_translation_unit::F_nsl_translation_unit(
		TKPA_valid <nrhi::F_nsl_shader_compiler> shader_compiler_p,
		const ncpp::containers::G_string& raw_src_content,
		const ncpp::containers::G_string& abs_path,
		const F_nsl_preprocessed_src& preprocessed_src,
		TKPA_valid<F_nsl_error_group> error_group_p
	) :
		shader_compiler_p_(shader_compiler_p),
		raw_src_content_(raw_src_content),
		abs_path_(abs_path),
		preprocessed_src_(preprocessed_src),
		error_group_p_(error_group_p)
	{
	}
	F_nsl_translation_unit::~F_nsl_translation_unit() {
	}

	void F_nsl_translation_unit::add_dependency(TK_valid<F_nsl_translation_unit> dependency_p) {

		dependency_p_vector_.push_back(dependency_p);
	}



	F_nsl_translation_unit_manager::F_nsl_translation_unit_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_translation_unit_manager::~F_nsl_translation_unit_manager() {
	}

	TU<F_nsl_translation_unit> F_nsl_translation_unit_manager::create_unit_instance(
		const G_string& raw_src_content,
		const G_string& abs_path,
		const F_nsl_preprocessed_src& preprocessed_src,
		TKPA_valid<F_nsl_error_group> error_group_p
	) {
		return TU<F_nsl_translation_unit>()(
			shader_compiler_p(),
			raw_src_content,
			abs_path,
			preprocessed_src,
			error_group_p
		);
	}

	TK<F_nsl_translation_unit> F_nsl_translation_unit_manager::create_unit(
		const G_string& raw_src_content,
		const G_string& abs_path
	) {
		auto error_group_p = shader_compiler_p_->error_storage_p()->optain_group(abs_path);

		auto comment_removed_src_content_opt = H_nsl_utilities::remove_comments(raw_src_content, &(error_group_p->stack()));

		if(comment_removed_src_content_opt) {
			const auto& comment_removed_src_content = comment_removed_src_content_opt.value();

			F_nsl_preprocessed_src preprocessed_src = {
				.content = comment_removed_src_content.first(),
				.raw_locations = comment_removed_src_content.second(),
				.abs_path = abs_path
			};

			auto translation_unit_p = create_unit_instance(
				raw_src_content,
				abs_path,
				preprocessed_src,
				error_group_p
			);

			auto keyed_translation_unit_p = NCPP_FOH_VALID(translation_unit_p);

			translation_unit_p_vector_.push_back(std::move(translation_unit_p));

			return keyed_translation_unit_p.no_requirements();
		}
		else {
			error_group_p->stack().push({
				"can't preprocess source"
				-1,
				raw_src_content.length()
			});
			return null;
		}
	}



	F_nsl_translation_unit_compiler::F_nsl_translation_unit_compiler(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_translation_unit_compiler::~F_nsl_translation_unit_compiler() {
	}

	void F_nsl_translation_unit_compiler::sort_internal() {
	}
	void F_nsl_translation_unit_compiler::read_internal() {
	}
	void F_nsl_translation_unit_compiler::apply_internal() {
	}
	void F_nsl_translation_unit_compiler::apply_macro_definitions_internal() {
	}
	void F_nsl_translation_unit_compiler::apply_types_internal() {
	}
	void F_nsl_translation_unit_compiler::apply_resources_internal() {
	}
	void F_nsl_translation_unit_compiler::apply_shader_entry_points_internal() {
	}

	void F_nsl_translation_unit_compiler::compile_minimal() {

		sort_internal();
		read_internal();
		apply_internal();
		apply_macro_definitions_internal();
		apply_types_internal();
		apply_resources_internal();
		apply_shader_entry_points_internal();
	}

	void F_nsl_translation_unit_compiler::prepare_units(
		const G_string& raw_src_content,
		const G_string& abs_path
	) {

	}
	G_string F_nsl_translation_unit_compiler::compile() {

		compile_minimal();

		return shader_compiler_p_->section_storage_p()->combine();
	}



	A_nsl_section_storage::A_nsl_section_storage(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	A_nsl_section_storage::~A_nsl_section_storage() {
	}

	G_string A_nsl_section_storage::combine() {

		G_string result = "";

		for(auto id : section_ids_) {

			result += section_map_[id] + "\n";
		}

		return std::move(result);
	}



	F_nsl_shader_compiler::F_nsl_shader_compiler() :
		module_manager_p_(
			TU<F_nsl_shader_module_manager>()(NCPP_KTHIS())
		),
		translation_unit_manager_p_(
			TU<F_nsl_translation_unit_manager>()(NCPP_KTHIS())
		),
		translation_unit_compiler_p_(
			TU<F_nsl_translation_unit_compiler>()(NCPP_KTHIS())
		),
		error_storage_p_(
			TU<F_nsl_error_storage>()(NCPP_KTHIS())
		),
		object_manager_p_(
			TU<F_nsl_object_manager>()(NCPP_KTHIS())
		),
		section_storage_p_(
			TU<F_nsl_section_storage>()(NCPP_KTHIS())
		)
	{
	}
	F_nsl_shader_compiler::F_nsl_shader_compiler(
		TU<F_nsl_shader_module_manager>&& module_manager_p,
		TU<F_nsl_translation_unit_manager>&& translation_unit_manager_p,
		TU<F_nsl_translation_unit_compiler>&& translation_unit_compiler_p,
		TU<F_nsl_error_storage>&& error_storage_p,
		TU<F_nsl_object_manager>&& object_manager_p,
		TU<A_nsl_section_storage>&& section_storage_p
	) :
		module_manager_p_(std::move(module_manager_p)),
		translation_unit_manager_p_(std::move(translation_unit_manager_p)),
		translation_unit_compiler_p_(std::move(translation_unit_compiler_p)),
		error_storage_p_(std::move(error_storage_p)),
		object_manager_p_(std::move(object_manager_p)),
		section_storage_p_(std::move(section_storage_p))
	{
	}
	F_nsl_shader_compiler::~F_nsl_shader_compiler() {
	}

	G_string F_nsl_shader_compiler::compile_hlsl(
		const G_string& raw_src_content,
		const G_string& abs_path
	) {

		translation_unit_compiler_p_->prepare_units(
			raw_src_content,
			abs_path
		);

		return translation_unit_compiler_p_->compile();
	}

}