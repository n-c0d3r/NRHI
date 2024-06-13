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
					is_left_correct = !is_object_name_character(src_content[current_location - 1]);
				}
				if(end_keyword_location < src_length) {
					is_right_correct = !is_object_name_character(src_content[end_keyword_location]);
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

		target = tree.object_implementation.name;

		if(!H_nsl_utilities::is_variable_name(target)) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				tree.object_implementation.begin_name_location,
				"invalid name: " + target
			);

			return eastl::nullopt;
		}

		if(
			!(name_manager_p->is_name_registered(target))
		) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				tree.object_implementation.begin_name_location,
				target + " is not registered"
			);

			return eastl::nullopt;
		}

		name_manager_p->deregister_name(target);

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



	F_nsl_alias_object::F_nsl_alias_object(
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
	F_nsl_alias_object::~F_nsl_alias_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_alias_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		return TG_vector<F_nsl_ast_tree>();
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

	TK<A_nsl_object> F_nsl_alias_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_alias_object>()(
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

		target = tree.object_implementation.name;

		if(name_manager_p->is_name_registered(target)) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				tree.object_implementation.begin_name_location,
				target + " is already registered"
			);

			return eastl::nullopt;
		}

		target_type = H_nsl_utilities::clear_space_head_tail(
			tree.object_implementation.bodies[0].content
		);
		if(!H_nsl_utilities::is_variable_name(target_type)) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				tree.object_implementation.bodies[0].begin_location,
				"invalid target type " + target_type
			);

			return eastl::nullopt;
		}

		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>(target);
		data_type_manager_p->register_semantic(target, target_type);

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

		b8 is_in = true;
		b8 is_out = false;

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

			if(param_child_info_tree.name == "out") {

				is_in = false;
				is_out = true;
			}
			else if(param_child_info_tree.name == "in") {

				is_in = true;
				is_out = false;
			}
			else if(param_child_info_tree.name == "inout") {

				is_in = true;
				is_out = true;
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
				const auto& type_name = type_tree.name;

				F_nsl_data_type_desc type_desc;

				if(!(data_type_manager_p->search_data_type(type_name, type_desc))) {

					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(unit_p->error_group_p()->stack()),
						type_tree.begin_name_location,
						"not found param type " + type_name
					);
					return eastl::nullopt;
				}

				data_param_descs_.push_back(
					F_nsl_data_param_desc {
						.name = param_child_info_tree.name,
						.type_desc = type_desc,
						.is_in = is_in,
						.is_out = is_out
					}
				);

				is_in = true;
				is_out = false;
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
			TU<F_nsl_alias_object_type>()(shader_compiler_p_)
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

		switch (tree.type)
		{
		case E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION:
			object_type_p = shader_compiler_p_->object_manager_p()->type_p(tree.object_implementation.keyword).no_requirements();
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

	b8 F_nsl_data_type_manager::search_data_type(const G_string& name, F_nsl_data_type_desc& out_desc) const {

		auto name_manager_p = shader_compiler_p_->name_manager_p();

		if(!(name_manager_p->is_name_registered(name)))
			return false;

		G_string target = name_manager_p->target(name);

		G_string target_name = target;
		G_string semantic;

		if(is_name_has_semantic_target_type(target)) {

			target_name = this->semantic_target_type(target);
			semantic = target;
		}

		out_desc = F_nsl_data_type_desc {
			.name = target_name,
			.semantic = std::move(semantic),
			.size = size(target_name)
		};

		return true;
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

		data_type_manager_p->register_semantic("SV_Position", "float4");
		data_type_manager_p->register_semantic("SV_Target", "float4");
	}



	F_nsl_resource_manager::F_nsl_resource_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_resource_manager::~F_nsl_resource_manager() {
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
		)
	{
	}
	F_nsl_shader_compiler::F_nsl_shader_compiler(
		TU<F_nsl_shader_module_manager>&& module_manager_p,
		TU<F_nsl_translation_unit_manager>&& translation_unit_manager_p,
		TU<F_nsl_translation_unit_compiler>&& translation_unit_compiler_p,
		TU<F_nsl_error_storage>&& error_storage_p,
		TU<F_nsl_object_manager>&& object_manager_p,
		TU<F_nsl_name_manager>&& name_manager_p,
		TU<F_nsl_data_type_manager>&& data_type_manager_p,
		TU<F_nsl_resource_manager>&& resource_manager_p
	) :
		module_manager_p_(std::move(module_manager_p)),
		translation_unit_manager_p_(std::move(translation_unit_manager_p)),
		translation_unit_compiler_p_(std::move(translation_unit_compiler_p)),
		error_storage_p_(std::move(error_storage_p)),
		object_manager_p_(std::move(object_manager_p)),
		name_manager_p_(std::move(name_manager_p)),
		data_type_manager_p_(std::move(data_type_manager_p)),
		resource_manager_p_(std::move(resource_manager_p))
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