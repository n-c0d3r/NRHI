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
	b8 H_nsl_utilities::is_variable_name(const G_string& str) {

		for(auto c : str) {

			if(!is_variable_name_character(c))
				return false;
		}

		return true;
	}
	b8 H_nsl_utilities::is_object_name_character(char c) {

		return (
			is_variable_name_character(c)
			|| (c == '@')
			|| (c == '#')
			|| (c == '&')
			|| (c == '$')
			|| (c == '!')
		);
	}
	b8 H_nsl_utilities::is_object_name(const G_string& str) {

		for(auto c : str) {

			if(!is_object_name_character(c))
				return false;
		}

		return true;
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

					char open_body_character = '(';
					char close_body_character = ')';

					if (src_content[t] == '{') {

						open_body_character = '{';
						close_body_character = '}';
					}

					// if beginning of childs
					if(src_content[t] == open_body_character) {

						i32 level = 1;

						begin_arg_location = t + 1;
						end_arg_location = begin_arg_location;

						F_nsl_str_state arg_str_state;

						sz k = begin_arg_location;
						for(; k < src_length; ++k) {

							arg_str_state.begin_check(src_content[k]);

							if(
								!(arg_str_state.value)
								&& (src_content[k] == open_body_character)
							)
								++level;

							if(
								!(arg_str_state.value)
								&& (src_content[k] == close_body_character)
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

								childs = std::move(childs_opt.value());
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
					(
						(
							src_content[j]
							!= keyword[
								eastl::min<sz>(
									j - begin_keyword_location,
									keyword.size() - 1
								)
							]
						)
						&& (keyword[0] != '@')
					)
					|| (
						(!H_nsl_utilities::is_object_name_character(src_content[j]))
						&& (keyword[0] == '@')
					)
				)
				{
					break;
				}
			}
			end_keyword_location = j;

			if(
				(
					((end_keyword_location - begin_keyword_location) == keyword.length())
					&& (keyword[0] != '@')
				)
				|| (
					(src_content[begin_keyword_location] == '@')
					&& (keyword[0] == '@')
				)
			) {
				b8 is_left_correct = true;
				b8 is_right_correct = true;

				if(current_location > 0) {
					is_left_correct = !is_object_name_character(src_content[current_location - 1]);
				}
				if(end_keyword_location < src_length) {
					is_right_correct = !is_object_name_character(src_content[end_keyword_location]);
				}

				if(is_left_correct && is_right_correct) {

					sz j2 = end_keyword_location;
					if(is_object_name_required)
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
					if(is_object_name_required)
						for(; j3 < src_length; ++j3)
						{
							if(
								!is_object_name_character(src_content[j3])
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

					G_string name;

					if(keyword[0] == '@') {
						name = src_content.substr(begin_keyword_location + 1, end_keyword_location - begin_keyword_location - 1);

						begin_name_location = begin_keyword_location + 1;

						end_keyword_location = begin_keyword_location + 1;
					}
					else {
						name = src_content.substr(begin_name_location, end_name_location - begin_name_location);
					}

					sz begin_body_location = end_name_location;
					sz end_body_location = begin_body_location;

					TG_vector<F_nsl_object_implementation_body> bodies;

					char open_body_chars[] = {
						'(',
						'{'
					};
					char close_body_chars[] = {
						')',
						'}'
					};

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
							char open_body_character = open_body_chars[object_body_index];
							char close_body_character = close_body_chars[object_body_index];

							if (src_content[t] == open_body_character)
							{
								i32 level = 1;

								begin_body_location = t + 1;
								end_body_location = begin_body_location;

								F_nsl_str_state body_str_state;

								sz begin_str_location = 0;

								b8 is_body_ended = false;

								// check for function-like
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
											.begin_location = location_offset_to_save + begin_body_location,
											.end_location = location_offset_to_save + end_body_location
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
										"function-like scope is not ended"
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
							"object body count is required to be >= " + G_to_string(min_object_body_count)
						);

						return eastl::nullopt;
					}

					begin_location = temp_begin_location;
					end_location = temp_end_location;

					F_nsl_object_implementation object_implementation = {
						.keyword = keyword,
						.name = name,

						.begin_location = location_offset_to_save + begin_location,
						.end_location = location_offset_to_save + end_location,
						.begin_keyword_location = location_offset_to_save + begin_keyword_location,
						.end_keyword_location = location_offset_to_save + end_keyword_location,
						.begin_name_location = location_offset_to_save + begin_name_location,
						.end_name_location = location_offset_to_save + end_name_location,

						.bodies = bodies
					};

					F_nsl_ast_tree tree = {
						.type = E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION,
						.object_implementation = object_implementation,
						.begin_location = location_offset_to_save + begin_location,
						.end_location = location_offset_to_save + end_location,
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
		const F_nsl_ast_tree_recursive_build_functor& recursive_build_functor,
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

				auto try_build_result = std::move(result_opt.value());

				if(try_build_result.trees.size()) {

					for(auto& tree : try_build_result.trees) {

						if(tree.begin_location != (location_offset_to_save + end_location)) {

							begin_location = end_location;
							end_location = tree.begin_location - location_offset_to_save;

							F_nsl_plain_text plain_text = {
								.content = src_content.substr(begin_location, end_location - begin_location),
								.begin_location = location_offset_to_save + begin_location,
								.end_location = location_offset_to_save + end_location,
							};

							F_nsl_ast_tree plain_text_tree = {
								.type = E_nsl_ast_tree_type::PLAIN_TEXT,
								.plain_text = plain_text,
								.begin_location = location_offset_to_save + begin_location,
								.end_location = location_offset_to_save + end_location
							};

							result.push_back(plain_text_tree);
						}

						begin_location = tree.begin_location - location_offset_to_save;
						end_location = tree.end_location - location_offset_to_save;

						result.push_back(tree);
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
				.begin_location = location_offset_to_save + begin_location,
				.end_location = location_offset_to_save + end_location,
			};

			F_nsl_ast_tree tree = {
				.type = E_nsl_ast_tree_type::PLAIN_TEXT,
				.plain_text = plain_text,
				.begin_location = location_offset_to_save + begin_location,
				.end_location = location_offset_to_save + end_location
			};

			result.push_back(tree);
		}



		// recursively build childs
		for(auto& tree : result) {

			auto childs_opt = recursive_build_functor(
				result,
				tree_index,
				error_stack_p
			);
			if(!childs_opt) {
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p,
					tree.begin_location,
					"can't build child ast trees"
				);
				return eastl::nullopt;
			}

			tree.childs = std::move(childs_opt.value());
			++tree_index;
		}

		return result;
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

	b8 H_nsl_utilities::is_plain_text_blank(const F_nsl_plain_text& plain_text) {

		for(auto c : plain_text.content) {

			if(
			 	!(
					(c == ' ')
					|| (c == '\t')
					|| (c == '\n')
					|| (c == '\r')
			 	)
			)
				return false;
		}

		return true;
	}



	b8 F_nsl_info_tree_reader::is_maps_setup_ = false;
	TG_map<G_string, b8> F_nsl_info_tree_reader::b8_str_to_value_map_;
	TG_map<G_string, E_nsl_element_format> F_nsl_info_tree_reader::element_format_str_to_value_map_;
	TG_map<G_string, E_nsl_semantic_input_class> F_nsl_info_tree_reader::semantic_input_class_str_to_value_map_;
	TG_map<G_string, E_filter> F_nsl_info_tree_reader::filter_str_to_value_map_;
	TG_map<G_string, E_texcoord_address_mode> F_nsl_info_tree_reader::texcoord_address_mode_str_to_value_map_;
	TG_map<G_string, E_cull_mode> F_nsl_info_tree_reader::cull_mode_str_to_value_map_;
	TG_map<G_string, E_fill_mode> F_nsl_info_tree_reader::fill_mode_str_to_value_map_;
	TG_map<G_string, E_format> F_nsl_info_tree_reader::format_str_to_value_map_;
	TG_map<G_string, E_depth_comparison_func> F_nsl_info_tree_reader::depth_comparison_func_str_to_value_map_;

	F_nsl_info_tree_reader::F_nsl_info_tree_reader(
		const TG_vector<F_nsl_info_tree>& info_trees,
		u32 location_offset_to_save,
		F_nsl_error_stack* error_stack_p
	) :
		info_trees_(info_trees),
		location_offset_to_save_(location_offset_to_save),
		error_stack_p_(error_stack_p)
	{
		if(!is_maps_setup_)
		{
			is_maps_setup_ = true;

			// setup b8_str_to_value_map_
			b8_str_to_value_map_["true"] = true;
			b8_str_to_value_map_["TRUE"] = true;
			b8_str_to_value_map_["on"] = true;
			b8_str_to_value_map_["ON"] = true;
			b8_str_to_value_map_["yes"] = true;
			b8_str_to_value_map_["YES"] = true;
			b8_str_to_value_map_["false"] = false;
			b8_str_to_value_map_["FALSE"] = false;
			b8_str_to_value_map_["off"] = false;
			b8_str_to_value_map_["OFF"] = false;
			b8_str_to_value_map_["no"] = false;
			b8_str_to_value_map_["NO"] = false;

			// setup element_format_str_to_value_map_
			element_format_str_to_value_map_["FLOAT_64"] = E_nsl_element_format::FLOAT_64;
			element_format_str_to_value_map_["UINT_64"] = E_nsl_element_format::UINT_64;
			element_format_str_to_value_map_["SINT_64"] = E_nsl_element_format::SINT_64;
			element_format_str_to_value_map_["TYPELESS_64"] = E_nsl_element_format::TYPELESS_64;
			element_format_str_to_value_map_["FLOAT_32"] = E_nsl_element_format::FLOAT_32;
			element_format_str_to_value_map_["FLOAT_16"] = E_nsl_element_format::FLOAT_16;
			element_format_str_to_value_map_["UNORM_16"] = E_nsl_element_format::UNORM_16;
			element_format_str_to_value_map_["UNORM_8"] = E_nsl_element_format::UNORM_8;
			element_format_str_to_value_map_["SNORM_16"] = E_nsl_element_format::SNORM_16;
			element_format_str_to_value_map_["SNORM_8"] = E_nsl_element_format::SNORM_8;
			element_format_str_to_value_map_["UINT_32"] = E_nsl_element_format::UINT_32;
			element_format_str_to_value_map_["UINT_16"] = E_nsl_element_format::UINT_16;
			element_format_str_to_value_map_["UINT_8"] = E_nsl_element_format::UINT_8;
			element_format_str_to_value_map_["SINT_32"] = E_nsl_element_format::SINT_32;
			element_format_str_to_value_map_["SINT_16"] = E_nsl_element_format::SINT_16;
			element_format_str_to_value_map_["SINT_8"] = E_nsl_element_format::SINT_8;
			element_format_str_to_value_map_["TYPELESS_32"] = E_nsl_element_format::TYPELESS_32;
			element_format_str_to_value_map_["TYPELESS_16"] = E_nsl_element_format::TYPELESS_16;
			element_format_str_to_value_map_["TYPELESS_8"] = E_nsl_element_format::TYPELESS_8;

			// setup semantic_input_class_str_to_value_map_
			semantic_input_class_str_to_value_map_["PER_VERTEX"] = E_nsl_semantic_input_class::PER_VERTEX;
			semantic_input_class_str_to_value_map_["PER_INSTANCE"] = E_nsl_semantic_input_class::PER_INSTANCE;

			// setup filter_str_to_value_map_
			filter_str_to_value_map_["MIN_MAG_MIP_POINT"] = E_filter::MIN_MAG_MIP_POINT;
			filter_str_to_value_map_["MIN_MAG_POINT_MIP_LINEAR"] = E_filter::MIN_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["MIN_POINT_MAG_LINEAR_MIP_POINT"] = E_filter::MIN_POINT_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["MIN_POINT_MAG_MIP_LINEAR"] = E_filter::MIN_POINT_MAG_MIP_LINEAR;
			filter_str_to_value_map_["MIN_LINEAR_MAG_MIP_POINT"] = E_filter::MIN_LINEAR_MAG_MIP_POINT;
			filter_str_to_value_map_["MIN_LINEAR_MAG_POINT_MIP_LINEAR"] = E_filter::MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["MIN_MAG_LINEAR_MIP_POINT"] = E_filter::MIN_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["MIN_MAG_MIP_LINEAR"] = E_filter::MIN_MAG_MIP_LINEAR;
			filter_str_to_value_map_["ANISOTROPIC"] = E_filter::ANISOTROPIC;
			filter_str_to_value_map_["COMPARISON_MIN_MAG_MIP_POINT"] = E_filter::COMPARISON_MIN_MAG_MIP_POINT;
			filter_str_to_value_map_["COMPARISON_MIN_MAG_POINT_MIP_LINEAR"] =
				E_filter::COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT"] =
				E_filter::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["COMPARISON_MIN_POINT_MAG_MIP_LINEAR"] =
				E_filter::COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
			filter_str_to_value_map_["COMPARISON_MIN_LINEAR_MAG_MIP_POINT"] =
				E_filter::COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
			filter_str_to_value_map_["COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR"] =
				E_filter::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["COMPARISON_MIN_MAG_LINEAR_MIP_POINT"] =
				E_filter::COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["COMPARISON_MIN_MAG_MIP_LINEAR"] = E_filter::COMPARISON_MIN_MAG_MIP_LINEAR;
			filter_str_to_value_map_["COMPARISON_ANISOTROPIC"] = E_filter::COMPARISON_ANISOTROPIC;
			filter_str_to_value_map_["MINIMUM_MIN_MAG_MIP_POINT"] = E_filter::MINIMUM_MIN_MAG_MIP_POINT;
			filter_str_to_value_map_["MINIMUM_MIN_MAG_POINT_MIP_LINEAR"] = E_filter::MINIMUM_MIN_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT"] =
				E_filter::MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["MINIMUM_MIN_POINT_MAG_MIP_LINEAR"] = E_filter::MINIMUM_MIN_POINT_MAG_MIP_LINEAR;
			filter_str_to_value_map_["MINIMUM_MIN_LINEAR_MAG_MIP_POINT"] = E_filter::MINIMUM_MIN_LINEAR_MAG_MIP_POINT;
			filter_str_to_value_map_["MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR"] =
				E_filter::MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["MINIMUM_MIN_MAG_LINEAR_MIP_POINT"] = E_filter::MINIMUM_MIN_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["MINIMUM_MIN_MAG_MIP_LINEAR"] = E_filter::MINIMUM_MIN_MAG_MIP_LINEAR;
			filter_str_to_value_map_["MINIMUM_ANISOTROPIC"] = E_filter::MINIMUM_ANISOTROPIC;
			filter_str_to_value_map_["MAXIMUM_MIN_MAG_MIP_POINT"] = E_filter::MAXIMUM_MIN_MAG_MIP_POINT;
			filter_str_to_value_map_["MAXIMUM_MIN_MAG_POINT_MIP_LINEAR"] = E_filter::MAXIMUM_MIN_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT"] =
				E_filter::MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["MAXIMUM_MIN_POINT_MAG_MIP_LINEAR"] = E_filter::MAXIMUM_MIN_POINT_MAG_MIP_LINEAR;
			filter_str_to_value_map_["MAXIMUM_MIN_LINEAR_MAG_MIP_POINT"] = E_filter::MAXIMUM_MIN_LINEAR_MAG_MIP_POINT;
			filter_str_to_value_map_["MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR"] =
				E_filter::MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["MAXIMUM_MIN_MAG_LINEAR_MIP_POINT"] = E_filter::MAXIMUM_MIN_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["MAXIMUM_MIN_MAG_MIP_LINEAR"] = E_filter::MAXIMUM_MIN_MAG_MIP_LINEAR;
			filter_str_to_value_map_["MAXIMUM_ANISOTROPIC"] = E_filter::MAXIMUM_ANISOTROPIC;

			// setup texcoord_address_mode_str_to_value_map_
			texcoord_address_mode_str_to_value_map_["CLAMP"] = E_texcoord_address_mode::CLAMP;
			texcoord_address_mode_str_to_value_map_["WRAP"] = E_texcoord_address_mode::WRAP;
			texcoord_address_mode_str_to_value_map_["BORDER"] = E_texcoord_address_mode::BORDER;
			texcoord_address_mode_str_to_value_map_["MIRROR"] = E_texcoord_address_mode::MIRROR;
			texcoord_address_mode_str_to_value_map_["MIRROR_ONCE"] = E_texcoord_address_mode::MIRROR_ONCE;

			// setup cull_mode_str_to_value_map_
			cull_mode_str_to_value_map_["NONE"] = E_cull_mode::NONE;
			cull_mode_str_to_value_map_["BACK"] = E_cull_mode::BACK;
			cull_mode_str_to_value_map_["FRONT"] = E_cull_mode::FRONT;
			cull_mode_str_to_value_map_["BOTH"] = flag_combine(
				E_cull_mode::BACK,
				E_cull_mode::FRONT
			);

			// setup fill_mode_str_to_value_map_
			fill_mode_str_to_value_map_["WIREFRAME"] = E_fill_mode::WIREFRAME;
			fill_mode_str_to_value_map_["SOLID"] = E_fill_mode::SOLID;

			// setup format_str_to_value_map_
			format_str_to_value_map_["NONE"] = E_format::NONE;
			format_str_to_value_map_["R32G32B32A32_FLOAT"] = E_format::R32G32B32A32_FLOAT;
			format_str_to_value_map_["R32G32B32_FLOAT"] = E_format::R32G32B32_FLOAT;
			format_str_to_value_map_["R32G32_FLOAT"] = E_format::R32G32_FLOAT;
			format_str_to_value_map_["R32_FLOAT"] = E_format::R32_FLOAT;
			format_str_to_value_map_["R16G16B16A16_FLOAT"] = E_format::R16G16B16A16_FLOAT;
			format_str_to_value_map_["R16G16_FLOAT"] = E_format::R16G16_FLOAT;
			format_str_to_value_map_["R16_FLOAT"] = E_format::R16_FLOAT;
			format_str_to_value_map_["D32_FLOAT"] = E_format::D32_FLOAT;
			format_str_to_value_map_["R16G16B16A16_UNORM"] = E_format::R16G16B16A16_UNORM;
			format_str_to_value_map_["R16G16_UNORM"] = E_format::R16G16_UNORM;
			format_str_to_value_map_["R16_UNORM"] = E_format::R16_UNORM;
			format_str_to_value_map_["R8G8B8A8_UNORM"] = E_format::R8G8B8A8_UNORM;
			format_str_to_value_map_["R8G8_UNORM"] = E_format::R8G8_UNORM;
			format_str_to_value_map_["R8_UNORM"] = E_format::R8_UNORM;
			format_str_to_value_map_["R16G16B16A16_SNORM"] = E_format::R16G16B16A16_SNORM;
			format_str_to_value_map_["R16G16_SNORM"] = E_format::R16G16_SNORM;
			format_str_to_value_map_["R16_SNORM"] = E_format::R16_SNORM;
			format_str_to_value_map_["R8G8B8A8_SNORM"] = E_format::R8G8B8A8_SNORM;
			format_str_to_value_map_["R8G8_SNORM"] = E_format::R8G8_SNORM;
			format_str_to_value_map_["R8_SNORM"] = E_format::R8_SNORM;
			format_str_to_value_map_["R32G32B32A32_UINT"] = E_format::R32G32B32A32_UINT;
			format_str_to_value_map_["R32G32B32_UINT"] = E_format::R32G32B32_UINT;
			format_str_to_value_map_["R32G32_UINT"] = E_format::R32G32_UINT;
			format_str_to_value_map_["R32_UINT"] = E_format::R32_UINT;
			format_str_to_value_map_["R16G16B16A16_UINT"] = E_format::R16G16B16A16_UINT;
			format_str_to_value_map_["R16G16_UINT"] = E_format::R16G16_UINT;
			format_str_to_value_map_["R16_UINT"] = E_format::R16_UINT;
			format_str_to_value_map_["R8G8B8A8_UINT"] = E_format::R8G8B8A8_UINT;
			format_str_to_value_map_["R8G8_UINT"] = E_format::R8G8_UINT;
			format_str_to_value_map_["R8_UINT"] = E_format::R8_UINT;
			format_str_to_value_map_["R32G32B32A32_SINT"] = E_format::R32G32B32A32_SINT;
			format_str_to_value_map_["R32G32B32_SINT"] = E_format::R32G32B32_SINT;
			format_str_to_value_map_["R32G32_SINT"] = E_format::R32G32_SINT;
			format_str_to_value_map_["R32_SINT"] = E_format::R32_SINT;
			format_str_to_value_map_["R16G16B16A16_SINT"] = E_format::R16G16B16A16_SINT;
			format_str_to_value_map_["R16G16_SINT"] = E_format::R16G16_SINT;
			format_str_to_value_map_["R16_SINT"] = E_format::R16_SINT;
			format_str_to_value_map_["R8G8B8A8_SINT"] = E_format::R8G8B8A8_SINT;
			format_str_to_value_map_["R8G8_SINT"] = E_format::R8G8_SINT;
			format_str_to_value_map_["R8_SINT"] = E_format::R8_SINT;
			format_str_to_value_map_["R32G32B32A32_TYPELESS"] = E_format::R32G32B32A32_TYPELESS;
			format_str_to_value_map_["R32G32B32_TYPELESS"] = E_format::R32G32B32_TYPELESS;
			format_str_to_value_map_["R32G32_TYPELESS"] = E_format::R32G32_TYPELESS;
			format_str_to_value_map_["R32_TYPELESS"] = E_format::R32_TYPELESS;
			format_str_to_value_map_["R16G16B16A16_TYPELESS"] = E_format::R16G16B16A16_TYPELESS;
			format_str_to_value_map_["R16G16_TYPELESS"] = E_format::R16G16_TYPELESS;
			format_str_to_value_map_["R16_TYPELESS"] = E_format::R16_TYPELESS;
			format_str_to_value_map_["R8G8B8A8_TYPELESS"] = E_format::R8G8B8A8_TYPELESS;
			format_str_to_value_map_["R8G8_TYPELESS"] = E_format::R8G8_TYPELESS;
			format_str_to_value_map_["R8_TYPELESS"] = E_format::R8_TYPELESS;

			// setup depth_comparison_func_str_to_value_map_
			depth_comparison_func_str_to_value_map_["NEVER"] = E_depth_comparison_func::NEVER;
			depth_comparison_func_str_to_value_map_["LESS"] = E_depth_comparison_func::LESS;
			depth_comparison_func_str_to_value_map_["EQUAL"] = E_depth_comparison_func::EQUAL;
			depth_comparison_func_str_to_value_map_["LESS_EQUAL"] = E_depth_comparison_func::LESS_EQUAL;
			depth_comparison_func_str_to_value_map_["GREATER"] = E_depth_comparison_func::GREATER;
			depth_comparison_func_str_to_value_map_["NOT_EQUAL"] = E_depth_comparison_func::NOT_EQUAL;
			depth_comparison_func_str_to_value_map_["GREATER_EQUAL"] = E_depth_comparison_func::GREATER_EQUAL;
			depth_comparison_func_str_to_value_map_["ALWAYS"] = E_depth_comparison_func::ALWAYS;
		}
	}
	F_nsl_info_tree_reader::~F_nsl_info_tree_reader() {
	}

	F_nsl_info_tree_reader::F_nsl_info_tree_reader(const F_nsl_info_tree_reader& x) :
		info_trees_(x.info_trees_),
		location_offset_to_save_(x.location_offset_to_save_),
		error_stack_p_(x.error_stack_p_)
	{
	}
	F_nsl_info_tree_reader& F_nsl_info_tree_reader::operator = (const F_nsl_info_tree_reader& x) {

		info_trees_ = x.info_trees_;
		location_offset_to_save_ = x.location_offset_to_save_;
		error_stack_p_ = x.error_stack_p_;

		return *this;
	}
	F_nsl_info_tree_reader::F_nsl_info_tree_reader(F_nsl_info_tree_reader&& x) :
		info_trees_(std::move(x.info_trees_)),
		location_offset_to_save_(x.location_offset_to_save_),
		error_stack_p_(x.error_stack_p_)
	{
	}
	F_nsl_info_tree_reader& F_nsl_info_tree_reader::operator = (F_nsl_info_tree_reader&& x) {

		info_trees_ = std::move(x.info_trees_);
		location_offset_to_save_ = x.location_offset_to_save_;
		error_stack_p_ = x.error_stack_p_;

		return *this;
	}


	b8 F_nsl_info_tree_reader::guarantee_not_empty() const {

		if(info_trees_.size() == 0) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				location_offset_to_save_,
				"no info provided"
			);
			return false;
		}

		return true;
	}
	b8 F_nsl_info_tree_reader::guarantee_index(u32 index) const {

		if(info_trees_.size() <= index) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				location_offset_to_save_,
				"not found argument at index \"" + G_to_string(index) + "\""
			);
			return false;
		}

		return true;
	}

	eastl::optional<b8> F_nsl_info_tree_reader::read_b8(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		auto it = b8_str_to_value_map_.find(value_str);

		if (it == b8_str_to_value_map_.end()) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<u8> F_nsl_info_tree_reader::read_u8(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		u8 value = 0;

		try{
			value = std::stoull(value_str.c_str());
		}
		catch(std::invalid_argument) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<u16> F_nsl_info_tree_reader::read_u16(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		u16 value = 0;

		try{
			value = std::stoull(value_str.c_str());
		}
		catch(std::invalid_argument) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<u32> F_nsl_info_tree_reader::read_u32(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		u32 value = 0;

		try{
			value = std::stoull(value_str.c_str());
		}
		catch(std::invalid_argument) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<u64> F_nsl_info_tree_reader::read_u64(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		u64 value = 0;

		try{
			value = std::stoull(value_str.c_str());
		}
		catch(std::invalid_argument) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<i8> F_nsl_info_tree_reader::read_i8(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		i8 value = 0;

		try{
			value = std::stoll(value_str.c_str());
		}
		catch(std::invalid_argument) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<i16> F_nsl_info_tree_reader::read_i16(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		i8 value = 0;

		try{
			value = std::stoll(value_str.c_str());
		}
		catch(std::invalid_argument) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<i32> F_nsl_info_tree_reader::read_i32(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		i8 value = 0;

		try{
			value = std::stoll(value_str.c_str());
		}
		catch(std::invalid_argument) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<i64> F_nsl_info_tree_reader::read_i64(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		i8 value = 0;

		try{
			value = std::stoll(value_str.c_str());
		}
		catch(std::invalid_argument) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<f32> F_nsl_info_tree_reader::read_f32(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		f32 value = 0.0f;

		try{
			value = std::stof(value_str.c_str());
		}
		catch(std::invalid_argument) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<f64> F_nsl_info_tree_reader::read_f64(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		f64 value = 0.0f;

		try{
			value = std::stod(value_str.c_str());
		}
		catch(std::invalid_argument) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<G_string> F_nsl_info_tree_reader::read_string(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		return info_trees_[index].name;
	}
	b8 F_nsl_info_tree_reader::guarantee_flag(const G_string& name) const {

		for(const auto& info_tree : info_trees_) {

			if(info_tree.name == name)
				return true;
		}

		NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
			error_stack_p_,
			location_offset_to_save_,
			"not found flag \"" + name + "\""
		);
		return false;
	}
	eastl::optional<F_nsl_info_tree_reader> F_nsl_info_tree_reader::read_sub(const G_string& name) const {

		for(const auto& info_tree : info_trees_) {

			if(info_tree.name == name) {

				return F_nsl_info_tree_reader(
					info_tree.childs,
					info_tree.begin_location,
					error_stack_p_
				);
			}
		}

		NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
			error_stack_p_,
			location_offset_to_save_,
			"not found sub info tree \"" + name + "\""
		);
		return eastl::nullopt;
	}
	eastl::optional<E_nsl_element_format> F_nsl_info_tree_reader::read_element_format(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		auto it = element_format_str_to_value_map_.find(value_str);

		if (it == element_format_str_to_value_map_.end()) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<E_nsl_semantic_input_class> F_nsl_info_tree_reader::read_semantic_input_class(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		auto it = semantic_input_class_str_to_value_map_.find(value_str);

		if (it == semantic_input_class_str_to_value_map_.end()) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<E_filter> F_nsl_info_tree_reader::read_filter(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		auto it = filter_str_to_value_map_.find(value_str);

		if (it == filter_str_to_value_map_.end()) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<E_texcoord_address_mode> F_nsl_info_tree_reader::read_texcoord_address_mode(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		auto it = texcoord_address_mode_str_to_value_map_.find(value_str);

		if (it == texcoord_address_mode_str_to_value_map_.end()) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<E_cull_mode> F_nsl_info_tree_reader::read_cull_mode(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		auto it = cull_mode_str_to_value_map_.find(value_str);

		if (it == cull_mode_str_to_value_map_.end()) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<E_fill_mode> F_nsl_info_tree_reader::read_fill_mode(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		auto it = fill_mode_str_to_value_map_.find(value_str);

		if (it == fill_mode_str_to_value_map_.end()) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<E_format> F_nsl_info_tree_reader::read_format(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		auto it = format_str_to_value_map_.find(value_str);

		if (it == format_str_to_value_map_.end()) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<E_depth_comparison_func> F_nsl_info_tree_reader::read_depth_comparison_func(u32 index) const {

		if(!guarantee_index(index)) {

			return eastl::nullopt;
		}

		G_string value_str = H_nsl_utilities::clear_space_head_tail(info_trees_[index].name);

		auto it = depth_comparison_func_str_to_value_map_.find(value_str);

		if (it == depth_comparison_func_str_to_value_map_.end()) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
			return eastl::nullopt;
		}

		return it->second;
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

			const auto& load_result = std::move(load_result_opt.value());
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



	A_nsl_reflection_item::A_nsl_reflection_item(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		const G_string& name
	) :
		shader_compiler_p_(shader_compiler_p),
		name_(name)
	{
	}
	A_nsl_reflection_item::~A_nsl_reflection_item()
	{
	}



	A_nsl_object::A_nsl_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		shader_compiler_p_(shader_compiler_p),
		type_p_(type_p),
		translation_unit_p_(translation_unit_p),
		name_(name)
	{
	}
	A_nsl_object::~A_nsl_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> A_nsl_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		return TG_vector<F_nsl_ast_tree>();
	}



	A_nsl_object_type::A_nsl_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		const G_string& name,
		b8 is_object_name_required,
		sz min_object_body_count,
		sz max_object_body_count,
		F_nsl_object_type_channel_mask channel_mask
	) :
		shader_compiler_p_(shader_compiler_p),
		name_(name),
		is_object_name_required_(is_object_name_required),
		min_object_body_count_(min_object_body_count),
		max_object_body_count_(max_object_body_count),
		channel_mask_(channel_mask)
	{
	}
	A_nsl_object_type::~A_nsl_object_type() {
	}

	TK<A_nsl_object> A_nsl_object_type::register_object(TU<A_nsl_object>&& object_p) {

		auto keyed_object_p = object_p.keyed();

		object_p_vector_.push_back(std::move(object_p));

		return keyed_object_p;
	}



	F_nsl_import_object::F_nsl_import_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_import_object::~F_nsl_import_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_import_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		return TG_vector<F_nsl_ast_tree>();
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

	TK<A_nsl_object> F_nsl_import_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_import_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_require_object::F_nsl_require_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_require_object::~F_nsl_require_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_require_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		const auto& tree = trees[index];

		auto& formula_content_body = tree.object_implementation.bodies[0];
		auto& body_content_body = tree.object_implementation.bodies[1];

		auto name_manager_p = shader_compiler_p()->name_manager_p();

		auto formula_checked_opt = name_manager_p->check_formula(
			unit_p,
			formula_content_body.begin_location,
			formula_content_body.content
		);
		if(!formula_checked_opt)
			return eastl::nullopt;

		is_enabled_ = formula_checked_opt.value();

		if(is_enabled_) {

			context.parent_object_p = NCPP_KTHIS().no_requirements();

			auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();
			auto childs_opt = translation_unit_compiler_p->parse(
				unit_p,
				body_content_body.content,
				context,
				body_content_body.begin_location
			);

			if(!childs_opt) {

				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					&(unit_p->error_group_p()->stack()),
					body_content_body.begin_location,
					"can't not parse require body"
				);
				return eastl::nullopt;
			}

			auto childs = std::move(childs_opt.value());

			return std::move(childs);
		}

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_require_object_type::F_nsl_require_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"require",
			false,
			2,
			2
		)
	{
	}
	F_nsl_require_object_type::~F_nsl_require_object_type() {
	}

	TK<A_nsl_object> F_nsl_require_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_require_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_define_object::F_nsl_define_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_define_object::~F_nsl_define_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_define_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		const auto& tree = trees[index];

		auto name_manager_p = shader_compiler_p()->name_manager_p();

		target = tree.object_implementation.name;

		if(tree.object_implementation.bodies.size()) {

			auto& target_content_body = tree.object_implementation.bodies[0];

			target = target_content_body.content;
		}

		if(name_manager_p->is_name_registered(tree.object_implementation.name)) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				tree.object_implementation.begin_name_location,
				tree.object_implementation.name + " is already registered"
			);

			return eastl::nullopt;
		}

		name_manager_p->register_name(tree.object_implementation.name, target);

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_define_object_type::F_nsl_define_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"define",
			true,
			0,
			1
		)
	{
	}
	F_nsl_define_object_type::~F_nsl_define_object_type() {
	}

	TK<A_nsl_object> F_nsl_define_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_define_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_undef_object::F_nsl_undef_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_undef_object::~F_nsl_undef_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_undef_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		const auto& tree = trees[index];

		auto name_manager_p = shader_compiler_p()->name_manager_p();

		if(!H_nsl_utilities::is_variable_name(tree.object_implementation.name)) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				tree.object_implementation.begin_name_location,
				"invalid name: " + tree.object_implementation.name
			);

			return eastl::nullopt;
		}

		if(
			!(name_manager_p->is_name_registered(tree.object_implementation.name))
		) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				tree.object_implementation.begin_name_location,
				tree.object_implementation.name + " is not registered"
			);

			return eastl::nullopt;
		}

		name_manager_p->deregister_name(tree.object_implementation.name);

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_undef_object_type::F_nsl_undef_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"undef",
			true,
			0,
			0
		)
	{
	}
	F_nsl_undef_object_type::~F_nsl_undef_object_type() {
	}

	TK<A_nsl_object> F_nsl_undef_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_undef_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_otherwise_object::F_nsl_otherwise_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_otherwise_object::~F_nsl_otherwise_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_otherwise_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		const auto& tree = trees[index];

		// check for prev conditional section
		{
			ptrdiff_t prev_index = index - 1;

			while(true)
			{
				if(prev_index < 0) {

					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(unit_p->error_group_p()->stack()),
						tree.begin_location,
						"no prev conditional section found"
					);
					return eastl::nullopt;
				}

				const auto& prev_tree = trees[prev_index];

				if(
					(prev_tree.type != E_nsl_ast_tree_type::PLAIN_TEXT)
					&& (prev_tree.type != E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION)
				) {
					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(unit_p->error_group_p()->stack()),
						prev_tree.begin_location,
						"invalid section, require \'require(){}\' or \'otherwise(){}\' before \'otherwise(){}\'"
					);
					return eastl::nullopt;
				}

				if(prev_tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) {

					const auto& object_implementation = prev_tree.object_implementation;

					if(
						(object_implementation.keyword != "require")
						&& (object_implementation.keyword != "otherwise")
					) {
						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							&(unit_p->error_group_p()->stack()),
							prev_tree.begin_location,
							"invalid section, require \'require(){}\' or \'otherwise(){}\' before \'otherwise(){}\'"
						);
						return eastl::nullopt;
					}

					if(object_implementation.keyword == "require")
					{
						is_enabled_ = !(object_implementation.attached_object_p.T_cast<F_nsl_require_object>()->is_enabled());
						break;
					}
					if(object_implementation.keyword == "otherwise")
					{
						is_enabled_ = !(object_implementation.attached_object_p.T_cast<F_nsl_otherwise_object>()->is_enabled());
						break;
					}
				}

				--prev_index;
			}
		}

		if(!is_enabled_)
			return TG_vector<F_nsl_ast_tree>();

		if(name() == "require") {

			if(tree.object_implementation.bodies.size() < 2) {

				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					&(unit_p->error_group_p()->stack()),
					tree.begin_location,
					"require condition and body"
				);
				return eastl::nullopt;
			}

			auto name_manager_p = shader_compiler_p()->name_manager_p();

			auto& formula_content_body = tree.object_implementation.bodies[0];

			auto formula_checked_opt = name_manager_p->check_formula(
				unit_p,
				formula_content_body.begin_location,
				formula_content_body.content
			);
			if(!formula_checked_opt)
				return eastl::nullopt;

			is_enabled_ = formula_checked_opt.value();
		}

		auto& body_content_body = tree.object_implementation.bodies.back();

		if(is_enabled_) {

			context.parent_object_p = NCPP_KTHIS().no_requirements();

			auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();
			auto childs_opt = translation_unit_compiler_p->parse(
				unit_p,
				body_content_body.content,
				context,
				body_content_body.begin_location
			);

			if(!childs_opt) {

				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					&(unit_p->error_group_p()->stack()),
					body_content_body.begin_location,
					"can't not parse require body"
				);
				return eastl::nullopt;
			}

			auto childs = std::move(childs_opt.value());

			return std::move(childs);
		}

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_otherwise_object_type::F_nsl_otherwise_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"otherwise",
			false,
			2,
			2
		)
	{
	}
	F_nsl_otherwise_object_type::~F_nsl_otherwise_object_type() {
	}

	TK<A_nsl_object> F_nsl_otherwise_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_otherwise_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_annotation_object::F_nsl_annotation_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_annotation_object::~F_nsl_annotation_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_annotation_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		if(object_implementation.bodies.size() != 0) {

			auto info_trees_opt = H_nsl_utilities::build_info_trees(
				object_implementation.bodies[0].content,
				object_implementation.bodies[0].begin_location,
				&(unit_p->error_group_p()->stack())
			);

			if(!info_trees_opt) {

				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					&(unit_p->error_group_p()->stack()),
					object_implementation.bodies[0].begin_location,
					"can't not parse annotation info trees"
				);
				return eastl::nullopt;
			}

			context.temp_object_config[object_implementation.name] = F_nsl_info_tree_reader(
				info_trees_opt.value(),
				object_implementation.bodies[0].begin_location,
				&(unit_p->error_group_p()->stack())
			);

			return TG_vector<F_nsl_ast_tree>();
		}

		context.temp_object_config[object_implementation.name] = F_nsl_info_tree_reader();

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_annotation_object_type::F_nsl_annotation_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"@",
			false,
			0,
			1
		)
	{
	}
	F_nsl_annotation_object_type::~F_nsl_annotation_object_type() {
	}

	TK<A_nsl_object> F_nsl_annotation_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_annotation_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_semantic_object::F_nsl_semantic_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_semantic_object::~F_nsl_semantic_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_semantic_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		const auto& tree = trees[index];

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		if(name_manager_p->is_name_registered(tree.object_implementation.name)) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				tree.object_implementation.begin_name_location,
				tree.object_implementation.name + " is already registered"
			);
			return eastl::nullopt;
		}

		// get target type
		target_type = name_manager_p->target(
			H_nsl_utilities::clear_space_head_tail(
				tree.object_implementation.bodies[0].content
			)
		);
		if(!H_nsl_utilities::is_variable_name(target_type)) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				tree.object_implementation.bodies[0].begin_location,
				"invalid target type " + target_type
			);
			return eastl::nullopt;
		}

		// apply object config
		E_nsl_semantic_input_class input_class = E_nsl_semantic_input_class::PER_VERTEX;
		{
			auto it = context.current_object_config.find("input_class");
			if(it != context.current_object_config.end()) {

				auto value_opt = it->second.read_semantic_input_class(0);

				if(!value_opt)
					return eastl::nullopt;

				input_class = value_opt.value();
			}
		}
		E_nsl_element_format element_format = data_type_manager_p->element_format(target_type);
		{
			auto it = context.current_object_config.find("element_format");
			if(it != context.current_object_config.end()) {

				auto value_opt = it->second.read_element_format(0);

				if(!value_opt)
					return eastl::nullopt;

				element_format = value_opt.value();
			}
		}

		// register semantic
		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>(tree.object_implementation.name);
		data_type_manager_p->register_semantic(
			tree.object_implementation.name,
			F_nsl_semantic_info {
				.target_type = target_type,
				.element_format = element_format,
				.input_class = input_class
			}
		);

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_semantic_object_type::F_nsl_semantic_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"semantic",
			true,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_semantic_object_type::~F_nsl_semantic_object_type() {
	}

	TK<A_nsl_object> F_nsl_semantic_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_semantic_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_structure_object::F_nsl_structure_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_structure_object::~F_nsl_structure_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_structure_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		context.parent_object_p = NCPP_KTHIS().no_requirements();

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		F_nsl_structure_info structure_info;
		structure_info.config_map = context.current_object_config;

		F_nsl_data_argument_config_map data_argument_config_map;

		// parse arguments
		auto argument_child_info_trees_opt = H_nsl_utilities::build_info_trees(
			object_implementation.bodies[0].content,
			object_implementation.bodies[0].begin_location,
			&(unit_p->error_group_p()->stack())
		);
		if(!argument_child_info_trees_opt) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				object_implementation.bodies[0].begin_location,
				"can't not parse struct arguments"
			);
			return eastl::nullopt;
		}
		auto& argument_child_info_trees = argument_child_info_trees_opt.value();
		TG_vector<F_nsl_ast_tree> argument_childs(argument_child_info_trees.size());
		for(u32 i = 0; i < argument_childs.size(); ++i) {

			auto& argument_child_info_tree = argument_child_info_trees[i];

			if(argument_child_info_tree.name[0] == '@') {

				data_argument_config_map[
					argument_child_info_tree.name.substr(1, argument_child_info_tree.name.length() - 1)
				] = F_nsl_info_tree_reader(
					argument_child_info_tree.childs,
					argument_child_info_tree.begin_childs_location,
					&(unit_p->error_group_p()->stack())
				);
				continue;
			}

			if(argument_child_info_tree.childs.size() == 0) {

				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					&(unit_p->error_group_p()->stack()),
					argument_child_info_tree.begin_childs_location,
					"require argument type"
				);
				return eastl::nullopt;
			}

			// check for count
			u32 count = 1;
			if(argument_child_info_tree.childs.size() >= 2) {

				const auto& info_tree = argument_child_info_tree.childs[1];

				G_string value_str = info_tree.name;

				try{
					count = std::stoi(value_str.c_str());
				}
				catch(std::invalid_argument) {

					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(unit_p->error_group_p()->stack()),
						info_tree.childs[0].begin_location,
						"invalid argument element count \"" + value_str + "\""
					);
					return eastl::nullopt;
				}
				catch(std::out_of_range) {

					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(unit_p->error_group_p()->stack()),
						info_tree.childs[0].begin_location,
						"invalid argument element count \"" + value_str + "\""
					);
					return eastl::nullopt;
				}
			}

			const auto& type_tree = argument_child_info_tree.childs[0];

			structure_info.argument_members.push_back(
				F_nsl_data_argument_member {
					.argument = F_nsl_data_argument {
						.name = argument_child_info_tree.name,
						.type = name_manager_p->target(type_tree.name),
						.count = count,
						.config_map = std::move(data_argument_config_map)
					}
				}
			);

			argument_childs[i] = F_nsl_ast_tree {
				.type = E_nsl_ast_tree_type::INFO_TREE,
				.info_tree = argument_child_info_tree,
				.begin_location = argument_child_info_tree.begin_location,
				.end_location = argument_child_info_tree.end_location
			};
		}

		// check for alignment annotation
		{
			auto it = context.current_object_config.find("alignment");
			if(it != context.current_object_config.end()) {

				const auto& alignment_info_tree_reader = it->second;

				auto value_opt = alignment_info_tree_reader.read_u32(0);

				if(!value_opt)
					return eastl::nullopt;

				structure_info.alignment = value_opt.value();
			}
		}

		// register semantic
		name_manager_p->template T_register_name<FE_nsl_name_types::STRUCTURE>(tree.object_implementation.name);
		data_type_manager_p->register_structure(
			tree.object_implementation.name,
			structure_info
		);

		return std::move(argument_childs);
	}



	F_nsl_structure_object_type::F_nsl_structure_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"struct",
			true,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_structure_object_type::~F_nsl_structure_object_type() {
	}

	TK<A_nsl_object> F_nsl_structure_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_structure_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_enumeration_object::F_nsl_enumeration_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_enumeration_object::~F_nsl_enumeration_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_enumeration_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		context.parent_object_p = NCPP_KTHIS().no_requirements();

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		F_nsl_enumeration_info enumeration_info;
		enumeration_info.config_map = context.current_object_config;

		// parse arguments
		auto value_child_info_trees_opt = H_nsl_utilities::build_info_trees(
			object_implementation.bodies[0].content,
			object_implementation.bodies[0].begin_location,
			&(unit_p->error_group_p()->stack())
		);
		if(!value_child_info_trees_opt) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				object_implementation.bodies[0].begin_location,
				"can't not parse enum values"
			);
			return eastl::nullopt;
		}
		auto& value_child_info_trees = value_child_info_trees_opt.value();
		TG_vector<F_nsl_ast_tree> value_childs(value_child_info_trees.size());
		for(u32 i = 0; i < value_childs.size(); ++i) {

			auto& value_child_info_tree = value_child_info_trees[i];

			// check for value
			u64 value = i;
			if(value_child_info_tree.childs.size() > 0) {

				const auto& info_tree = value_child_info_tree.childs[0];

				G_string value_str = info_tree.name;

				try{
					value = std::stoull(value_str.c_str());
				}
				catch(std::invalid_argument) {

					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(unit_p->error_group_p()->stack()),
						info_tree.childs[0].begin_location,
						"invalid argument element count \"" + value_str + "\""
					);
					return eastl::nullopt;
				}
				catch(std::out_of_range) {

					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(unit_p->error_group_p()->stack()),
						info_tree.childs[0].begin_location,
						"invalid argument element count \"" + value_str + "\""
					);
					return eastl::nullopt;
				}
			}

			enumeration_info.values.push_back(
				{
					value_child_info_tree.name,
					value
				}
			);

			value_childs[i] = F_nsl_ast_tree {
				.type = E_nsl_ast_tree_type::INFO_TREE,
				.info_tree = value_child_info_tree,
				.begin_location = value_child_info_tree.begin_location,
				.end_location = value_child_info_tree.end_location
			};
		}

		// check for value_type annotation
		{
			auto it = context.current_object_config.find("value_type");
			if(it != context.current_object_config.end()) {

				const auto& value_type_info_tree_reader = it->second;

				auto value_opt = value_type_info_tree_reader.read_string(0);

				if(!value_opt)
					return eastl::nullopt;

				enumeration_info.value_type = name_manager_p->target(value_opt.value());
			}
		}

		// register semantic
		name_manager_p->template T_register_name<FE_nsl_name_types::STRUCTURE>(tree.object_implementation.name);
		data_type_manager_p->register_enumeration(
			tree.object_implementation.name,
			enumeration_info
		);

		return std::move(value_childs);
	}



	F_nsl_enumeration_object_type::F_nsl_enumeration_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"enum",
			true,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_enumeration_object_type::~F_nsl_enumeration_object_type() {
	}

	TK<A_nsl_object> F_nsl_enumeration_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_enumeration_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_resource_object::F_nsl_resource_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_resource_object::~F_nsl_resource_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_resource_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		context.parent_object_p = NCPP_KTHIS().no_requirements();

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();
		auto resource_manager_p = shader_compiler_p()->resource_manager_p();

		F_nsl_resource_info resource_info;
		resource_info.config_map = context.current_object_config;

		// parse child info trees
		auto child_info_trees_opt = H_nsl_utilities::build_info_trees(
			object_implementation.bodies[0].content,
			object_implementation.bodies[0].begin_location,
			&(unit_p->error_group_p()->stack())
		);
		if(!child_info_trees_opt) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				object_implementation.bodies[0].begin_location,
				"can't not parse resource type"
			);
			return eastl::nullopt;
		}

		auto& child_info_trees = child_info_trees_opt.value();

		if(child_info_trees.size() == 0) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				object_implementation.bodies[0].begin_location,
				"require resource type"
			);
			return eastl::nullopt;
		}

		auto& type_child_info_tree = child_info_trees[0];

		TG_vector<F_nsl_ast_tree> child_trees(1);
		child_trees[0] = F_nsl_ast_tree {
			.type = E_nsl_ast_tree_type::INFO_TREE,
			.info_tree = type_child_info_tree,
			.begin_location = type_child_info_tree.begin_location,
			.end_location = type_child_info_tree.end_location
		};

		TG_vector<G_string> type_args(type_child_info_tree.childs.size());
		for(u32 i = 0; i < type_args.size(); ++i) {

			type_args[i] = type_child_info_tree.childs[i].name;
		}

		resource_info.type = type_child_info_tree.name;
		resource_info.type_args = type_args;

		// check for slot annotation
		{
			auto it = context.current_object_config.find("slot");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_u32(0);

				if(!value_opt)
					return eastl::nullopt;

				resource_info.slot = value_opt.value();
			}
		}

		// check for shaders annotation
		{
			auto it = context.current_object_config.find("shaders");
			if(it != context.current_object_config.end()) {

				const auto& shaders_info_tree_reader = it->second;

				if(!shaders_info_tree_reader.guarantee_not_empty())
					return eastl::nullopt;

				resource_info.shader_filters = {};

				for(const auto& shader_info_tree : shaders_info_tree_reader.info_trees()) {

					resource_info.shader_filters.insert(shader_info_tree.name);
				}
			}
		}

		// register resource
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE>(tree.object_implementation.name);
		resource_manager_p->register_resource(
			tree.object_implementation.name,
			resource_info
		);

		return std::move(child_trees);
	}



	F_nsl_resource_object_type::F_nsl_resource_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"resource",
			true,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_resource_object_type::~F_nsl_resource_object_type() {
	}

	TK<A_nsl_object> F_nsl_resource_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_resource_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_uniform_object::F_nsl_uniform_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_uniform_object::~F_nsl_uniform_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_uniform_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		context.parent_object_p = NCPP_KTHIS().no_requirements();

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();
		auto uniform_manager_p = shader_compiler_p()->uniform_manager_p();

		F_nsl_uniform_info uniform_info;
		uniform_info.config_map = context.current_object_config;

		uniform_info.type = name_manager_p->target(
			H_nsl_utilities::clear_space_head_tail(
				object_implementation.bodies[0].content
			)
		);

		// check for buffer annotation
		uniform_info.buffer = context.default_uniform_buffer;
		{
			auto it = context.current_object_config.find("buffer");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_string(0);

				if(!value_opt)
					return eastl::nullopt;

				uniform_info.buffer = name_manager_p->target(value_opt.value());
			}
		}

		// register uniform
		name_manager_p->template T_register_name<FE_nsl_name_types::UNIFORM>(tree.object_implementation.name);
		uniform_manager_p->register_uniform(
			tree.object_implementation.name,
			uniform_info
		);

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_uniform_object_type::F_nsl_uniform_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"uniform",
			true,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_uniform_object_type::~F_nsl_uniform_object_type() {
	}

	TK<A_nsl_object> F_nsl_uniform_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_uniform_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_default_uniform_buffer_object::F_nsl_default_uniform_buffer_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_default_uniform_buffer_object::~F_nsl_default_uniform_buffer_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_default_uniform_buffer_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		context.parent_object_p = NCPP_KTHIS().no_requirements();

		context.default_uniform_buffer = shader_compiler_p()->name_manager_p()->target(
			H_nsl_utilities::clear_space_head_tail(
				object_implementation.bodies[0].content
			)
		);

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_default_uniform_buffer_object_type::F_nsl_default_uniform_buffer_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"default_uniform_buffer",
			false,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_default_uniform_buffer_object_type::~F_nsl_default_uniform_buffer_object_type() {
	}

	TK<A_nsl_object> F_nsl_default_uniform_buffer_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_default_uniform_buffer_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_sampler_state_object::F_nsl_sampler_state_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_sampler_state_object::~F_nsl_sampler_state_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_sampler_state_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		context.parent_object_p = NCPP_KTHIS().no_requirements();

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();
		auto sampler_state_manager_p = shader_compiler_p()->sampler_state_manager_p();

		F_nsl_sampler_state_info sampler_state_info;
		sampler_state_info.config_map = context.current_object_config;

		// check for slot annotation
		{
			auto it = context.current_object_config.find("slot");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_u32(0);

				if(!value_opt)
					return eastl::nullopt;

				sampler_state_info.slot = value_opt.value();
			}
		}

		// check for shaders annotation
		{
			auto it = context.current_object_config.find("shaders");
			if(it != context.current_object_config.end()) {

				const auto& shaders_info_tree_reader = it->second;

				if(!shaders_info_tree_reader.guarantee_not_empty())
					return eastl::nullopt;

				sampler_state_info.shader_filters = {};

				for(auto& shader_info_tree : shaders_info_tree_reader.info_trees()) {

					sampler_state_info.shader_filters.insert(shader_info_tree.name);
				}
			}
		}

		// check for filter annotation
		{
			auto it = context.current_object_config.find("filter");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_filter(0);

				if(!value_opt)
					return eastl::nullopt;

				sampler_state_info.desc.filter = value_opt.value();
			}
		}

		// check for texcoord_address_modes annotation
		{
			auto it = context.current_object_config.find("texcoord_address_modes");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				for(u32 i = 0; i < eastl::min<u32>(info_tree_reader.info_trees().size(), 3); ++i) {

					auto value_opt = info_tree_reader.read_texcoord_address_mode(i);

					if(!value_opt)
						return eastl::nullopt;

					sampler_state_info.desc.texcoord_address_modes[i] = value_opt.value();
				}
			}
		}

		// check for lod_offset annotation
		{
			auto it = context.current_object_config.find("lod_offset");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_f32(0);

				if(!value_opt)
					return eastl::nullopt;

				sampler_state_info.desc.lod_offset = value_opt.value();
			}
		}

		// check for min_lod annotation
		{
			auto it = context.current_object_config.find("min_lod");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_f32(0);

				if(!value_opt)
					return eastl::nullopt;

				sampler_state_info.desc.min_lod = value_opt.value();
			}
		}

		// check for max_lod annotation
		{
			auto it = context.current_object_config.find("max_lod");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_f32(0);

				if(!value_opt)
					return eastl::nullopt;

				sampler_state_info.desc.max_lod = value_opt.value();
			}
		}

		// register sampler_state
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE>(tree.object_implementation.name);
		sampler_state_manager_p->register_sampler_state(
			tree.object_implementation.name,
			sampler_state_info
		);

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_sampler_state_object_type::F_nsl_sampler_state_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"sampler_state",
			true,
			0,
			0,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_sampler_state_object_type::~F_nsl_sampler_state_object_type() {
	}

	TK<A_nsl_object> F_nsl_sampler_state_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_sampler_state_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	A_nsl_shader_object::A_nsl_shader_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	A_nsl_shader_object::~A_nsl_shader_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> A_nsl_shader_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		context.parent_object_p = NCPP_KTHIS().no_requirements();

		auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		b8 is_prev_in_keyword = false;
		b8 is_prev_out_keyword = false;
		F_nsl_data_argument_config_map data_argument_config_map;

		// parse params
		auto param_child_info_trees_opt = H_nsl_utilities::build_info_trees(
			object_implementation.bodies[0].content,
			object_implementation.bodies[0].begin_location,
			&(unit_p->error_group_p()->stack())
		);
		if(!param_child_info_trees_opt) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				object_implementation.bodies[0].begin_location,
				"can't not parse shader params"
			);
			return eastl::nullopt;
		}
		auto& param_child_info_trees = param_child_info_trees_opt.value();
		TG_vector<F_nsl_ast_tree> param_childs(param_child_info_trees.size());
		for(u32 i = 0; i < param_childs.size(); ++i) {

			auto& param_child_info_tree = param_child_info_trees[i];

			if(param_child_info_tree.name[0] == '@') {

				if(is_prev_in_keyword) {

					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(unit_p->error_group_p()->stack()),
						param_child_info_tree.begin_location,
						"annotation after \"in\" keyword is not allowed"
					);
					return eastl::nullopt;
				}
				if(is_prev_out_keyword) {

					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(unit_p->error_group_p()->stack()),
						param_child_info_tree.begin_location,
						"annotation after \"out\" keyword is not allowed"
					);
					return eastl::nullopt;
				}

				data_argument_config_map[
					param_child_info_tree.name.substr(1, param_child_info_tree.name.length() - 1)
				] = F_nsl_info_tree_reader(
					param_child_info_tree.childs,
					param_child_info_tree.begin_childs_location,
					&(unit_p->error_group_p()->stack())
				);
				continue;
			}

			if(param_child_info_tree.name == "out") {

				is_prev_out_keyword = true;
			}
			else if(param_child_info_tree.name == "in") {

				is_prev_in_keyword = true;
			}
			else if(param_child_info_tree.name == "inout") {

				is_prev_in_keyword = true;
				is_prev_out_keyword = true;
			}
			else {
				if(param_child_info_tree.childs.size() != 1) {

					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(unit_p->error_group_p()->stack()),
						param_child_info_tree.begin_childs_location,
						"require param type"
					);
					return eastl::nullopt;
				}

				const auto& type_tree = param_child_info_tree.childs[0];

				data_params_.push_back(
					F_nsl_data_param {
						.argument = F_nsl_data_argument{
							.name = param_child_info_tree.name,
							.type = type_tree.name,
							.config_map = std::move(data_argument_config_map)
						},
						.is_in = is_prev_in_keyword || !(is_prev_in_keyword || is_prev_out_keyword),
						.is_out = is_prev_out_keyword
					}
				);

				is_prev_in_keyword = false;
				is_prev_out_keyword = false;
			}

			param_childs[i] = F_nsl_ast_tree {
				.type = E_nsl_ast_tree_type::INFO_TREE,
				.info_tree = param_child_info_tree,
				.begin_location = param_child_info_tree.begin_location,
				.end_location = param_child_info_tree.end_location
			};
		}

		// parse body
		context.object_type_channel_mask_stack.push(nsl_function_body_object_type_channel_mask);
		auto body_childs_opt = translation_unit_compiler_p->parse(
			unit_p,
			object_implementation.bodies[1].content,
			context,
			object_implementation.bodies[1].begin_location,
			0
		);
		if(!body_childs_opt) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				object_implementation.bodies[1].begin_location,
				"can't not parse shader body"
			);
			return eastl::nullopt;
		}
		context.object_type_channel_mask_stack.pop();

		auto body_childs = std::move(body_childs_opt.value());

		// ast tree childs
		auto childs = std::move(param_childs);
		childs.insert(
			childs.end(),
			body_childs.begin(),
			body_childs.end()
		);

		auto name_manager_p = shader_compiler_p()->name_manager_p();

		name_manager_p->template T_register_name<FE_nsl_name_types::SHADER>(tree.object_implementation.name);

		return std::move(childs);
	}



	A_nsl_shader_object_type::A_nsl_shader_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		const G_string& name
	) :
		A_nsl_object_type(
			shader_compiler_p,
			name,
			true,
			2,
			2,
			nsl_global_object_type_channel_mask
		)
	{
	}
	A_nsl_shader_object_type::~A_nsl_shader_object_type() {
	}



	F_nsl_vertex_shader_object::F_nsl_vertex_shader_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_shader_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_vertex_shader_object::~F_nsl_vertex_shader_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_vertex_shader_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto childs = A_nsl_shader_object::recursive_build_ast_tree(
			context,
			unit_p,
			trees,
			index,
			error_stack_p
		);

		return std::move(childs);
	}



	F_nsl_vertex_shader_object_type::F_nsl_vertex_shader_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_shader_object_type(
			shader_compiler_p,
			"vertex_shader"
		)
	{
	}
	F_nsl_vertex_shader_object_type::~F_nsl_vertex_shader_object_type() {
	}

	TK<A_nsl_object> F_nsl_vertex_shader_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_vertex_shader_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_pixel_shader_object::F_nsl_pixel_shader_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_shader_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_pixel_shader_object::~F_nsl_pixel_shader_object() {
	}



	F_nsl_pixel_shader_object_type::F_nsl_pixel_shader_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_shader_object_type(
			shader_compiler_p,
			"pixel_shader"
		)
	{
	}
	F_nsl_pixel_shader_object_type::~F_nsl_pixel_shader_object_type() {
	}

	TK<A_nsl_object> F_nsl_pixel_shader_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_pixel_shader_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_compute_shader_object::F_nsl_compute_shader_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_shader_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_compute_shader_object::~F_nsl_compute_shader_object() {
	}



	F_nsl_compute_shader_object_type::F_nsl_compute_shader_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_shader_object_type(
			shader_compiler_p,
			"compute_shader"
		)
	{
	}
	F_nsl_compute_shader_object_type::~F_nsl_compute_shader_object_type() {
	}

	TK<A_nsl_object> F_nsl_compute_shader_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_compute_shader_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_object_manager::F_nsl_object_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
		register_type(
			TU<F_nsl_import_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_require_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_define_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_undef_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_otherwise_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_annotation_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_semantic_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_vertex_shader_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_pixel_shader_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_compute_shader_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_structure_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_enumeration_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_resource_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_uniform_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_default_uniform_buffer_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_sampler_state_object_type>()(shader_compiler_p_)
		);
	}
	F_nsl_object_manager::~F_nsl_object_manager() {
	}

	TK_valid<A_nsl_object_type> F_nsl_object_manager::register_type(TU<A_nsl_object_type>&& object_type_p) {

		auto keyed_object_type_p = NCPP_FOH_VALID(object_type_p);

		type_p_map_[keyed_object_type_p->name()] = std::move(object_type_p);

		return keyed_object_type_p;
	}

	TG_vector<F_nsl_ast_tree_try_build_functor> F_nsl_object_manager::ast_tree_try_build_functors(F_nsl_object_type_channel_mask mask) {

		TG_vector<F_nsl_ast_tree_try_build_functor> result;
		result.reserve(type_p_map_.size());

		for(const auto& it : type_p_map_) {

			auto object_type_p = NCPP_FOH_VALID(it.second);

			if(
				flag_is_has(
					object_type_p->channel_mask(),
					mask
				)
			)
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

	TU<A_nsl_section_storage>F_nsl_translation_unit::create_section_storage()
	{
		return TU<F_nsl_section_storage>()(NCPP_KTHIS());
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
			const auto& comment_removed_src_content = std::move(comment_removed_src_content_opt.value());

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

	b8 F_nsl_translation_unit_compiler::sort_internal() {

		return true;
	}
	b8 F_nsl_translation_unit_compiler::read_internal() {

		return true;
	}
	b8 F_nsl_translation_unit_compiler::apply_internal() {

		return true;
	}
	b8 F_nsl_translation_unit_compiler::apply_macro_definitions_internal() {

		return true;
	}
	b8 F_nsl_translation_unit_compiler::apply_types_internal() {

		return true;
	}
	b8 F_nsl_translation_unit_compiler::apply_resources_internal() {

		return true;
	}
	b8 F_nsl_translation_unit_compiler::apply_shader_entry_points_internal() {

		return true;
	}

	b8 F_nsl_translation_unit_compiler::compile_minimal() {

		if(!sort_internal())
			return false;
		if(!read_internal())
			return false;
		if(!apply_internal())
			return false;
		if(!apply_macro_definitions_internal())
			return false;
		if(!apply_types_internal())
			return false;
		if(!apply_resources_internal())
			return false;
		if(!apply_shader_entry_points_internal())
			return false;

		return true;
	}

	b8 F_nsl_translation_unit_compiler::prepare_unit(TK_valid<F_nsl_translation_unit> unit_p) {

		F_nsl_context context;

		auto ast_trees_opt = parse(
			unit_p,
			unit_p->preprocessed_src().content,
			context
		);

		if(ast_trees_opt) {

			unit_p->ast_trees() = std::move(ast_trees_opt.value());

			return true;
		}

		return false;
	}

	b8 F_nsl_translation_unit_compiler::prepare_units(
		const G_string& raw_src_content,
		const G_string& abs_path
	) {
		main_unit_p_ = shader_compiler_p_->translation_unit_manager_p()->create_unit(
			raw_src_content,
			abs_path
		);

		if(!main_unit_p_)
			return false;

		if(!prepare_unit(NCPP_FOH_VALID(main_unit_p_)))
			return false;

		return true;
	}
	eastl::optional<G_string> F_nsl_translation_unit_compiler::compile() {

		if(!compile_minimal())
			return eastl::nullopt;

		return G_string();
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_translation_unit_compiler::parse(
		TK_valid<F_nsl_translation_unit> unit_p,
		const G_string& src_content,
		F_nsl_context& context,
		sz location_offset_to_safe,
		F_nsl_object_type_channel_mask additional_object_type_channel_mask
	) {
		auto try_build_functors = shader_compiler_p_->object_manager_p()->ast_tree_try_build_functors(
			additional_object_type_channel_mask
			| context.object_type_channel_mask_stack.get_container().back()
		);

		auto ast_trees_opt = H_nsl_utilities::build_ast_trees(
			src_content,
			try_build_functors,
			[this, &context, &unit_p](
				TG_vector<F_nsl_ast_tree>& trees,
				sz index,
				F_nsl_error_stack* error_stack_p
			) -> eastl::optional<TG_vector<F_nsl_ast_tree>> {

			  	return recursive_build_ast_tree(
					context,
					unit_p,
					trees,
					index,
					error_stack_p
			  	);
			},
			location_offset_to_safe,
			&(unit_p->error_group_p()->stack())
		);

		if(ast_trees_opt) {

			return std::move(ast_trees_opt.value());
		}

		return eastl::nullopt;
	}
	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_translation_unit_compiler::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];

		TK<A_nsl_object> object_p;
		TK<A_nsl_object_type> object_type_p;

		eastl::optional<TG_vector<F_nsl_ast_tree>> result = TG_vector<F_nsl_ast_tree>();

		G_string object_type_name;
		b8 is_annotation = false;

		switch (tree.type)
		{
		case E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION:
			// check for the case of annotation object
			if(tree.object_implementation.keyword.size())
			{
				if(tree.object_implementation.keyword[0] == '@') {
					is_annotation = true;
					object_type_name = "@";
				}
				else object_type_name = tree.object_implementation.keyword;
			}
			else object_type_name = tree.object_implementation.keyword;

			// push current object config map to stack
			if(!is_annotation) {
				context.current_object_config = context.temp_object_config;
				context.object_config_stack.push(context.current_object_config);
				context.temp_object_config = {};
			}

			object_type_p = shader_compiler_p_->object_manager_p()->type_p(object_type_name).no_requirements();
			object_p = object_type_p->create_object(
				tree,
				context,
				unit_p
			);
			if(!object_p)
				return eastl::nullopt;
			result = object_p->recursive_build_ast_tree(
				context,
				unit_p,
				trees,
				index,
				error_stack_p
			);

			// pop current object config map
			if(!is_annotation) {
				context.current_object_config = {};
				context.temp_object_config = {};
				context.object_config_stack.pop();
				if(context.object_config_stack.size())
					context.current_object_config = context.object_config_stack.get_container().back();
			}

			if(!result) {
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p,
					tree.begin_location,
					"can't build object ast"
				);
				return eastl::nullopt;
			}
			context.parent_object_p = object_p;
			break;
		case E_nsl_ast_tree_type::PLAIN_TEXT:
			break;
		}

		return std::move(result);
	}



	A_nsl_section_storage::A_nsl_section_storage(TKPA_valid<F_nsl_translation_unit> translation_unit_p) :
		translation_unit_p_(translation_unit_p)
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



	F_nsl_name_manager::F_nsl_name_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_name_manager::~F_nsl_name_manager() {
	}

	eastl::optional<b8> F_nsl_name_manager::check_formula(
		TK_valid<F_nsl_translation_unit> translation_unit_p,
		sz location,
		const G_string& formula
	) {

		G_string space_cleared_formula = H_nsl_utilities::clear_space_head_tail(formula);

		i32 i = formula.length() - 1;
		for(; i >= 0; --i) {

			if(!H_nsl_utilities::is_variable_name_character(formula[i]))
				break;
		}

		G_string name = space_cleared_formula.substr(i + 1, space_cleared_formula.length() - i - 1);

		i32 j = 0;
		i32 negative_count = 0;
		for(; j <= i; ++j) {

			if(
				(formula[j] == ' ')
				|| (formula[j] == '\t')
				|| (formula[j] == '\n')
				|| (formula[j] == '\r')
				|| (formula[j] == '!')
			) {
				if(formula[j] == '!')
					++negative_count;
			}
			else {

				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					&(translation_unit_p->error_group_p()->stack()),
					location,
					"invalid character \'" + G_string(space_cleared_formula[j], 1) + "\' in formula"
				);
				return eastl::nullopt;
			}
		}

		return ((negative_count % 2 == 0) == is_name_registered(name));
	}



	F_nsl_data_type_manager::F_nsl_data_type_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_data_type_manager::~F_nsl_data_type_manager() {
	}

	F_nsl_semantic_info F_nsl_data_type_manager::process_semantic_info(const G_string& name, const F_nsl_semantic_info& semantic_info) {

		F_nsl_semantic_info result = semantic_info;

		auto name_manager_p = shader_compiler_p_->name_manager_p();

		register_size(
			name,
			size(semantic_info.target_type)
		);
		register_alignment(
			name,
			alignment(semantic_info.target_type)
		);
		register_element_format(
			name,
			semantic_info.element_format
		);

		return std::move(result);
	}
	F_nsl_structure_info F_nsl_data_type_manager::process_structure_info(const G_string& name, const F_nsl_structure_info& structure_info) {

		F_nsl_structure_info result = structure_info;

		constexpr u32 min_pack_alignment = 16;

		u32 offset = 0;

		for(u32 i = 0; i < result.argument_members.size(); ++i) {

			auto& argument_member = result.argument_members[i];
			auto& argument = argument_member.argument;

			u32 member_alignment = alignment(argument.type);
			u32 member_element_count = argument.count;

			u32 member_single_element_size = size(argument.type);
			u32 aligned_member_single_element_size = align_size(member_single_element_size, member_alignment);

			u32 member_size = (
				(member_element_count - 1) * eastl::max<u32>(aligned_member_single_element_size, min_pack_alignment)
				+ member_single_element_size
			);

			offset = align_size(offset, member_alignment);

			argument_member.offset = offset;

			offset += member_size;
		}

		result.size = align_size(offset, structure_info.alignment);

		register_size(
			name,
			result.size
		);
		register_alignment(
			name,
			structure_info.alignment
		);

		return std::move(result);
	}
	F_nsl_enumeration_info F_nsl_data_type_manager::process_enumeration_info(const G_string& name, const F_nsl_enumeration_info& enumeration_info) {

		F_nsl_enumeration_info result = enumeration_info;

		auto name_manager_p = shader_compiler_p_->name_manager_p();

		register_size(
			name,
			size(enumeration_info.value_type)
		);
		register_alignment(
			name,
			alignment(enumeration_info.value_type)
		);

		return std::move(result);
	}



	A_nsl_output_language::A_nsl_output_language(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language as_enum
	) :
		shader_compiler_p_(shader_compiler_p),
		as_enum_(as_enum)
	{
	}
	A_nsl_output_language::~A_nsl_output_language() {
	}



	F_nsl_output_hlsl::F_nsl_output_hlsl(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_output_language(shader_compiler_p, E_nsl_output_language::HLSL)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl::~F_nsl_output_hlsl() {
	}

	void F_nsl_output_hlsl::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->register_name("NSL_OUTPUT_HLSL");

		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("bool");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("int");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("uint");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("half");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("float");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("double");

		name_manager_p->register_name("b8", "bool");
		name_manager_p->register_name("i32", "int");
		name_manager_p->register_name("u32", "uint");
		name_manager_p->register_name("f16", "half");
		name_manager_p->register_name("f32", "float");
		name_manager_p->register_name("f64", "double");

		data_type_manager_p->register_size("bool", 1);
		data_type_manager_p->register_size("int", 4);
		data_type_manager_p->register_size("uint", 4);
		data_type_manager_p->register_size("half", 2);
		data_type_manager_p->register_size("float", 4);
		data_type_manager_p->register_size("double", 8);

		data_type_manager_p->register_alignment("bool", 1);
		data_type_manager_p->register_alignment("int", 4);
		data_type_manager_p->register_alignment("uint", 4);
		data_type_manager_p->register_alignment("half", 2);
		data_type_manager_p->register_alignment("float", 4);
		data_type_manager_p->register_alignment("double", 8);

		data_type_manager_p->register_element_format("bool", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double", E_nsl_element_format::FLOAT_64);

		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("bool2");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("int2");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("uint2");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("half2");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("float2");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("double2");

		name_manager_p->register_name("F_vector2_b8", "bool2");
		name_manager_p->register_name("F_vector2_i32", "int2");
		name_manager_p->register_name("F_vector2_u32", "uint2");
		name_manager_p->register_name("F_vector2_f16", "half2");
		name_manager_p->register_name("F_vector2_f32", "float2");
		name_manager_p->register_name("F_vector2_f64", "double2");

		data_type_manager_p->register_size("bool2", 1 * 2);
		data_type_manager_p->register_size("int2", 4 * 2);
		data_type_manager_p->register_size("uint2", 4 * 2);
		data_type_manager_p->register_size("half2", 2 * 2);
		data_type_manager_p->register_size("float2", 4 * 2);
		data_type_manager_p->register_size("double2", 8 * 2);

		data_type_manager_p->register_alignment("bool2", 1 * 2);
		data_type_manager_p->register_alignment("int2", 4 * 2);
		data_type_manager_p->register_alignment("uint2", 4 * 2);
		data_type_manager_p->register_alignment("half2", 2 * 2);
		data_type_manager_p->register_alignment("float2", 4 * 2);
		data_type_manager_p->register_alignment("double2", 8 * 2);

		data_type_manager_p->register_element_format("bool2", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int2", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint2", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half2", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float2", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double2", E_nsl_element_format::FLOAT_64);

		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("bool3");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("int3");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("uint3");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("half3");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("float3");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("double3");

		name_manager_p->register_name("F_vector3_b8", "bool3");
		name_manager_p->register_name("F_vector3_i32", "int3");
		name_manager_p->register_name("F_vector3_u32", "uint3");
		name_manager_p->register_name("F_vector3_f16", "half3");
		name_manager_p->register_name("F_vector3_f32", "float3");
		name_manager_p->register_name("F_vector3_f64", "double3");

		data_type_manager_p->register_size("bool3", 1 * 3);
		data_type_manager_p->register_size("int3", 4 * 3);
		data_type_manager_p->register_size("uint3", 4 * 3);
		data_type_manager_p->register_size("half3", 2 * 3);
		data_type_manager_p->register_size("float3", 4 * 3);
		data_type_manager_p->register_size("double3", 8 * 3);

		data_type_manager_p->register_alignment("bool3", 1 * 4);
		data_type_manager_p->register_alignment("int3", 4 * 4);
		data_type_manager_p->register_alignment("uint3", 4 * 4);
		data_type_manager_p->register_alignment("half3", 2 * 4);
		data_type_manager_p->register_alignment("float3", 4 * 4);
		data_type_manager_p->register_alignment("double3", 8 * 4);

		data_type_manager_p->register_element_format("bool3", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int3", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint3", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half3", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float3", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double3", E_nsl_element_format::FLOAT_64);

		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("bool4");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("int4");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("uint4");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("half4");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("float4");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("double4");

		name_manager_p->register_name("F_vector4_b8", "bool4");
		name_manager_p->register_name("F_vector4_i32", "int4");
		name_manager_p->register_name("F_vector4_u32", "uint4");
		name_manager_p->register_name("F_vector4_f16", "half4");
		name_manager_p->register_name("F_vector4_f32", "float4");
		name_manager_p->register_name("F_vector4_f64", "double4");

		data_type_manager_p->register_size("bool4", 1 * 4);
		data_type_manager_p->register_size("int4", 4 * 4);
		data_type_manager_p->register_size("uint4", 4 * 4);
		data_type_manager_p->register_size("half4", 2 * 4);
		data_type_manager_p->register_size("float4", 4 * 4);
		data_type_manager_p->register_size("double4", 8 * 4);

		data_type_manager_p->register_alignment("bool4", 1 * 4);
		data_type_manager_p->register_alignment("int4", 4 * 4);
		data_type_manager_p->register_alignment("uint4", 4 * 4);
		data_type_manager_p->register_alignment("half4", 2 * 4);
		data_type_manager_p->register_alignment("float4", 4 * 4);
		data_type_manager_p->register_alignment("double4", 8 * 4);

		data_type_manager_p->register_element_format("bool4", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int4", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint4", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half4", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float4", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double4", E_nsl_element_format::FLOAT_64);

		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("bool2x2");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("int2x2");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("uint2x2");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("half2x2");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("float2x2");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("double2x2");

		name_manager_p->register_name("F_matrix2x2_b8", "bool2x2");
		name_manager_p->register_name("F_matrix2x2_i32", "int2x2");
		name_manager_p->register_name("F_matrix2x2_u32", "uint2x2");
		name_manager_p->register_name("F_matrix2x2_f16", "half2x2");
		name_manager_p->register_name("F_matrix2x2_f32", "float2x2");
		name_manager_p->register_name("F_matrix2x2_f64", "double2x2");

		data_type_manager_p->register_size("bool2x2", 1 * 2 * 2);
		data_type_manager_p->register_size("int2x2", 4 * 2 * 2);
		data_type_manager_p->register_size("uint2x2", 4 * 2 * 2);
		data_type_manager_p->register_size("half2x2", 2 * 2 * 2);
		data_type_manager_p->register_size("float2x2", 4 * 2 * 2);
		data_type_manager_p->register_size("double2x2", 8 * 2 * 2);

		data_type_manager_p->register_alignment("bool2x2", 1 * 2 * 2);
		data_type_manager_p->register_alignment("int2x2", 4 * 2 * 2);
		data_type_manager_p->register_alignment("uint2x2", 4 * 2 * 2);
		data_type_manager_p->register_alignment("half2x2", 2 * 2 * 2);
		data_type_manager_p->register_alignment("float2x2", 4 * 2 * 2);
		data_type_manager_p->register_alignment("double2x2", 8 * 2 * 2);

		data_type_manager_p->register_element_format("bool2x2", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int2x2", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint2x2", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half2x2", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float2x2", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double2x2", E_nsl_element_format::FLOAT_64);

		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("bool3x3");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("int3x3");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("uint3x3");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("half3x3");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("float3x3");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("double3x3");

		name_manager_p->register_name("F_matrix3x3_b8", "bool3x3");
		name_manager_p->register_name("F_matrix3x3_i32", "int3x3");
		name_manager_p->register_name("F_matrix3x3_u32", "uint3x3");
		name_manager_p->register_name("F_matrix3x3_f16", "half3x3");
		name_manager_p->register_name("F_matrix3x3_f32", "float3x3");
		name_manager_p->register_name("F_matrix3x3_f64", "double3x3");

		data_type_manager_p->register_size("bool3x3", 1 * 3 * 3);
		data_type_manager_p->register_size("int3x3", 4 * 3 * 3);
		data_type_manager_p->register_size("uint3x3", 4 * 3 * 3);
		data_type_manager_p->register_size("half3x3", 2 * 3 * 3);
		data_type_manager_p->register_size("float3x3", 4 * 3 * 3);
		data_type_manager_p->register_size("double3x3", 8 * 3 * 3);

		data_type_manager_p->register_alignment("bool3x3", 1 * 4 * 4);
		data_type_manager_p->register_alignment("int3x3", 4 * 4 * 4);
		data_type_manager_p->register_alignment("uint3x3", 4 * 4 * 4);
		data_type_manager_p->register_alignment("half3x3", 2 * 4 * 4);
		data_type_manager_p->register_alignment("float3x3", 4 * 4 * 4);
		data_type_manager_p->register_alignment("double3x3", 8 * 4 * 4);

		data_type_manager_p->register_element_format("bool3x3", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int3x3", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint3x3", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half3x3", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float3x3", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double3x3", E_nsl_element_format::FLOAT_64);

		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("bool4x4");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("int4x4");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("uint4x4");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("half4x4");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("float4x4");
		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("double4x4");

		name_manager_p->register_name("F_matrix4x4_b8", "bool4x4");
		name_manager_p->register_name("F_matrix4x4_i32", "int4x4");
		name_manager_p->register_name("F_matrix4x4_u32", "uint4x4");
		name_manager_p->register_name("F_matrix4x4_f16", "half4x4");
		name_manager_p->register_name("F_matrix4x4_f32", "float4x4");
		name_manager_p->register_name("F_matrix4x4_f64", "double4x4");

		data_type_manager_p->register_size("bool4x4", 1 * 4 * 4);
		data_type_manager_p->register_size("int4x4", 4 * 4 * 4);
		data_type_manager_p->register_size("uint4x4", 4 * 4 * 4);
		data_type_manager_p->register_size("half4x4", 2 * 4 * 4);
		data_type_manager_p->register_size("float4x4", 4 * 4 * 4);
		data_type_manager_p->register_size("double4x4", 8 * 4 * 4);

		data_type_manager_p->register_alignment("bool4x4", 1 * 4 * 4);
		data_type_manager_p->register_alignment("int4x4", 4 * 4 * 4);
		data_type_manager_p->register_alignment("uint4x4", 4 * 4 * 4);
		data_type_manager_p->register_alignment("half4x4", 2 * 4 * 4);
		data_type_manager_p->register_alignment("float4x4", 4 * 4 * 4);
		data_type_manager_p->register_alignment("double4x4", 8 * 4 * 4);

		data_type_manager_p->register_element_format("bool4x4", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int4x4", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint4x4", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half4x4", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float4x4", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double4x4", E_nsl_element_format::FLOAT_64);

		name_manager_p->register_name("F_vector2", "F_vector2_f32");
		name_manager_p->register_name("F_vector3", "F_vector3_f32");
		name_manager_p->register_name("F_vector4", "F_vector4_f32");
		name_manager_p->register_name("F_matrix2x2", "F_matrix2x2_f32");
		name_manager_p->register_name("F_matrix3x3", "F_matrix3x3_f32");
		name_manager_p->register_name("F_matrix4x4", "F_matrix4x4_f32");

		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_Position");
		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_Target");

		name_manager_p->register_name("SV_POSITION", "SV_Position");
		name_manager_p->register_name("SV_TARGET", "SV_Target");

		data_type_manager_p->register_semantic(
			"SV_Position",
			F_nsl_semantic_info {
				"float4"
			}
		);
		data_type_manager_p->register_semantic(
			"SV_Target",
			F_nsl_semantic_info {
				"float4"
			}
		);
	}



	F_nsl_resource_manager::F_nsl_resource_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_resource_manager::~F_nsl_resource_manager() {
	}

	F_nsl_resource_info F_nsl_resource_manager::process_resource_info(const G_string& name, const F_nsl_resource_info& resource_info) {
		
		F_nsl_resource_info result = resource_info;

		return std::move(result);
	}



	F_nsl_uniform_manager::F_nsl_uniform_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_uniform_manager::~F_nsl_uniform_manager() {
	}

	F_nsl_uniform_info F_nsl_uniform_manager::process_uniform_info(const G_string& name, const F_nsl_uniform_info& uniform_info) {

		F_nsl_uniform_info result = uniform_info;

		return std::move(result);
	}



	F_nsl_sampler_state_manager::F_nsl_sampler_state_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_sampler_state_manager::~F_nsl_sampler_state_manager() {
	}

	F_nsl_sampler_state_info F_nsl_sampler_state_manager::process_sampler_state_info(const G_string& name, const F_nsl_sampler_state_info& sampler_state_info) {
		
		F_nsl_sampler_state_info result = sampler_state_info;

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
		name_manager_p_(
			TU<F_nsl_name_manager>()(NCPP_KTHIS())
		),
		data_type_manager_p_(
			TU<F_nsl_data_type_manager>()(NCPP_KTHIS())
		),
		resource_manager_p_(
			TU<F_nsl_resource_manager>()(NCPP_KTHIS())
		),
		uniform_manager_p_(
			TU<F_nsl_uniform_manager>()(NCPP_KTHIS())
		),
		sampler_state_manager_p_(
			TU<F_nsl_sampler_state_manager>()(NCPP_KTHIS())
		)
	{
	}
	F_nsl_shader_compiler::F_nsl_shader_compiler(
		TF_nsl_shader_compiler_subsystem_creator<F_nsl_shader_module_manager> module_manager_creator,
		TF_nsl_shader_compiler_subsystem_creator<F_nsl_translation_unit_manager> translation_unit_manager_creator,
		TF_nsl_shader_compiler_subsystem_creator<F_nsl_translation_unit_compiler> translation_unit_compiler_creator,
		TF_nsl_shader_compiler_subsystem_creator<F_nsl_error_storage> error_storage_creator,
		TF_nsl_shader_compiler_subsystem_creator<F_nsl_object_manager> object_manager_creator,
		TF_nsl_shader_compiler_subsystem_creator<F_nsl_name_manager> name_manager_creator,
		TF_nsl_shader_compiler_subsystem_creator<F_nsl_data_type_manager> data_type_manager_creator,
		TF_nsl_shader_compiler_subsystem_creator<F_nsl_resource_manager> resource_manager_creator,
		TF_nsl_shader_compiler_subsystem_creator<F_nsl_uniform_manager> uniform_manager_creator,
		TF_nsl_shader_compiler_subsystem_creator<F_nsl_sampler_state_manager> sampler_state_manager_creator
	) :
		module_manager_p_(module_manager_creator(NCPP_KTHIS())),
		translation_unit_manager_p_(translation_unit_manager_creator(NCPP_KTHIS())),
		translation_unit_compiler_p_(translation_unit_compiler_creator(NCPP_KTHIS())),
		error_storage_p_(error_storage_creator(NCPP_KTHIS())),
		object_manager_p_(object_manager_creator(NCPP_KTHIS())),
		name_manager_p_(name_manager_creator(NCPP_KTHIS())),
		data_type_manager_p_(data_type_manager_creator(NCPP_KTHIS())),
		resource_manager_p_(resource_manager_creator(NCPP_KTHIS())),
		uniform_manager_p_(uniform_manager_creator(NCPP_KTHIS())),
		sampler_state_manager_p_(sampler_state_manager_creator(NCPP_KTHIS()))
	{
	}
	F_nsl_shader_compiler::~F_nsl_shader_compiler() {
	}

	TU<A_nsl_output_language> F_nsl_shader_compiler::create_output_language() {

		return TU<F_nsl_output_hlsl>()(
			NCPP_KTHIS()
		);
	}

	eastl::optional<G_string> F_nsl_shader_compiler::compile(
		const G_string& raw_src_content,
		E_nsl_output_language output_language,
		const G_string& abs_path
	) {
		output_language_p_ = create_output_language();

		if(
			!(
				translation_unit_compiler_p_->prepare_units(
					raw_src_content,
					abs_path
				)
			)
		)
			return eastl::nullopt;

		return translation_unit_compiler_p_->compile();
	}

}