#include <nrhi/nsl_shader_compiler.hpp>



namespace nrhi {

	G_string F_nsl_compiled_result::build(u32 shader_index) const {

		G_string result;

		u32 shader_count = reflection.shaders.size();

		NCPP_ASSERT((0 <= shader_index) && (shader_index <= shader_count)) << "shader index is out of bound";

		auto& shader = reflection.shaders[shader_index];
		NCPP_ASSERT(shader.type != ED_shader_type::NONE) << "invalid shader type";

		// define shader index
		{
			NCPP_ASSERT(shader_index < shader_count)
				<< "shader index out of bound";

			result += (
				"#define NSL_SHADER_"
				+ shader.name
				+ "\n"
			);
			result += (
				"#define NSL_SHADER_INDEX_"
				+ G_to_string(shader_index)
				+ "\n"
			);
		}

		// define shader type
		{
			NRHI_ENUM_SWITCH(
				shader.type,
				NRHI_ENUM_CASE(
					ED_shader_type::VERTEX,
					result += "#define NSL_SHADER_TYPE_VERTEX \n";
				)
				NRHI_ENUM_CASE(
					ED_shader_type::PIXEL,
					result += "#define NSL_SHADER_TYPE_PIXEL \n";
				)
				NRHI_ENUM_CASE(
					ED_shader_type::COMPUTE,
					result += "#define NSL_SHADER_TYPE_COMPUTE \n";
				)
			);
		}

		// override sampler state actual slots
		{
			u32 sample_state_count = reflection.sampler_states.size();

			for (u32 i = 0; i < sample_state_count; ++i)
			{
				const auto& sampler_state = reflection.sampler_states[i];

				u32 actual_slot = sampler_state.actual_slots[shader_index];

				G_string register_slot_macro = (
					"NSL_REGISTER_SLOT_"
					+ sampler_state.name
				);

				result += (
					"#define "
					+ register_slot_macro
					+ " "
					+ G_to_string(actual_slot)
					+ "\n"
				);
			}
		}

		// override resource actual slots
		{
			u32 sample_state_count = reflection.resources.size();

			for (u32 i = 0; i < sample_state_count; ++i)
			{
				const auto& resource = reflection.resources[i];

				u32 actual_slot = resource.actual_slots[shader_index];

				if(actual_slot == -1)
					continue;

				G_string register_slot_macro = (
					"NSL_REGISTER_SLOT_"
					+ resource.name
				);

				result += (
					"#define "
					+ register_slot_macro
					+ " "
					+ G_to_string(actual_slot)
					+ "\n"
				);
			}
		}

		result += src_content;

		return result;
	}



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
	TG_map<G_string, ED_filter> F_nsl_info_tree_reader::filter_str_to_value_map_;
	TG_map<G_string, ED_texcoord_address_mode> F_nsl_info_tree_reader::texcoord_address_mode_str_to_value_map_;
	TG_map<G_string, ED_cull_mode> F_nsl_info_tree_reader::cull_mode_str_to_value_map_;
	TG_map<G_string, ED_fill_mode> F_nsl_info_tree_reader::fill_mode_str_to_value_map_;
	TG_map<G_string, ED_format> F_nsl_info_tree_reader::format_str_to_value_map_;
	TG_map<G_string, ED_depth_comparison_func> F_nsl_info_tree_reader::depth_comparison_func_str_to_value_map_;
	TG_map<G_string, ED_primitive_topology> F_nsl_info_tree_reader::primitive_topology_str_to_value_map_;

	F_nsl_info_tree_reader::F_nsl_info_tree_reader(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		const TG_vector<F_nsl_info_tree>& info_trees,
		u32 location_offset_to_save,
		F_nsl_error_stack* error_stack_p
	) :
		shader_compiler_p_(shader_compiler_p.no_requirements()),
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
			filter_str_to_value_map_["MIN_MAG_MIP_POINT"] = ED_filter::MIN_MAG_MIP_POINT;
			filter_str_to_value_map_["MIN_MAG_POINT_MIP_LINEAR"] = ED_filter::MIN_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["MIN_POINT_MAG_LINEAR_MIP_POINT"] = ED_filter::MIN_POINT_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["MIN_POINT_MAG_MIP_LINEAR"] = ED_filter::MIN_POINT_MAG_MIP_LINEAR;
			filter_str_to_value_map_["MIN_LINEAR_MAG_MIP_POINT"] = ED_filter::MIN_LINEAR_MAG_MIP_POINT;
			filter_str_to_value_map_["MIN_LINEAR_MAG_POINT_MIP_LINEAR"] = ED_filter::MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["MIN_MAG_LINEAR_MIP_POINT"] = ED_filter::MIN_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["MIN_MAG_MIP_LINEAR"] = ED_filter::MIN_MAG_MIP_LINEAR;
			filter_str_to_value_map_["ANISOTROPIC"] = ED_filter::ANISOTROPIC;
			filter_str_to_value_map_["COMPARISON_MIN_MAG_MIP_POINT"] = ED_filter::COMPARISON_MIN_MAG_MIP_POINT;
			filter_str_to_value_map_["COMPARISON_MIN_MAG_POINT_MIP_LINEAR"] =
				ED_filter::COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT"] =
				ED_filter::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["COMPARISON_MIN_POINT_MAG_MIP_LINEAR"] =
				ED_filter::COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
			filter_str_to_value_map_["COMPARISON_MIN_LINEAR_MAG_MIP_POINT"] =
				ED_filter::COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
			filter_str_to_value_map_["COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR"] =
				ED_filter::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["COMPARISON_MIN_MAG_LINEAR_MIP_POINT"] =
				ED_filter::COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["COMPARISON_MIN_MAG_MIP_LINEAR"] = ED_filter::COMPARISON_MIN_MAG_MIP_LINEAR;
			filter_str_to_value_map_["COMPARISON_ANISOTROPIC"] = ED_filter::COMPARISON_ANISOTROPIC;
			filter_str_to_value_map_["MINIMUM_MIN_MAG_MIP_POINT"] = ED_filter::MINIMUM_MIN_MAG_MIP_POINT;
			filter_str_to_value_map_["MINIMUM_MIN_MAG_POINT_MIP_LINEAR"] = ED_filter::MINIMUM_MIN_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT"] =
				ED_filter::MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["MINIMUM_MIN_POINT_MAG_MIP_LINEAR"] = ED_filter::MINIMUM_MIN_POINT_MAG_MIP_LINEAR;
			filter_str_to_value_map_["MINIMUM_MIN_LINEAR_MAG_MIP_POINT"] = ED_filter::MINIMUM_MIN_LINEAR_MAG_MIP_POINT;
			filter_str_to_value_map_["MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR"] =
				ED_filter::MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["MINIMUM_MIN_MAG_LINEAR_MIP_POINT"] = ED_filter::MINIMUM_MIN_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["MINIMUM_MIN_MAG_MIP_LINEAR"] = ED_filter::MINIMUM_MIN_MAG_MIP_LINEAR;
			filter_str_to_value_map_["MINIMUM_ANISOTROPIC"] = ED_filter::MINIMUM_ANISOTROPIC;
			filter_str_to_value_map_["MAXIMUM_MIN_MAG_MIP_POINT"] = ED_filter::MAXIMUM_MIN_MAG_MIP_POINT;
			filter_str_to_value_map_["MAXIMUM_MIN_MAG_POINT_MIP_LINEAR"] = ED_filter::MAXIMUM_MIN_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT"] =
				ED_filter::MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["MAXIMUM_MIN_POINT_MAG_MIP_LINEAR"] = ED_filter::MAXIMUM_MIN_POINT_MAG_MIP_LINEAR;
			filter_str_to_value_map_["MAXIMUM_MIN_LINEAR_MAG_MIP_POINT"] = ED_filter::MAXIMUM_MIN_LINEAR_MAG_MIP_POINT;
			filter_str_to_value_map_["MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR"] =
				ED_filter::MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			filter_str_to_value_map_["MAXIMUM_MIN_MAG_LINEAR_MIP_POINT"] = ED_filter::MAXIMUM_MIN_MAG_LINEAR_MIP_POINT;
			filter_str_to_value_map_["MAXIMUM_MIN_MAG_MIP_LINEAR"] = ED_filter::MAXIMUM_MIN_MAG_MIP_LINEAR;
			filter_str_to_value_map_["MAXIMUM_ANISOTROPIC"] = ED_filter::MAXIMUM_ANISOTROPIC;

			// setup texcoord_address_mode_str_to_value_map_
			texcoord_address_mode_str_to_value_map_["CLAMP"] = ED_texcoord_address_mode::CLAMP;
			texcoord_address_mode_str_to_value_map_["WRAP"] = ED_texcoord_address_mode::WRAP;
			texcoord_address_mode_str_to_value_map_["BORDER"] = ED_texcoord_address_mode::BORDER;
			texcoord_address_mode_str_to_value_map_["MIRROR"] = ED_texcoord_address_mode::MIRROR;
			texcoord_address_mode_str_to_value_map_["MIRROR_ONCE"] = ED_texcoord_address_mode::MIRROR_ONCE;

			// setup cull_mode_str_to_value_map_
			cull_mode_str_to_value_map_["NONE"] = ED_cull_mode::NONE;
			cull_mode_str_to_value_map_["BACK"] = ED_cull_mode::BACK;
			cull_mode_str_to_value_map_["FRONT"] = ED_cull_mode::FRONT;
			cull_mode_str_to_value_map_["BOTH"] = flag_combine(
				ED_cull_mode::BACK,
				ED_cull_mode::FRONT
			);

			// setup fill_mode_str_to_value_map_
			fill_mode_str_to_value_map_["WIREFRAME"] = ED_fill_mode::WIREFRAME;
			fill_mode_str_to_value_map_["SOLID"] = ED_fill_mode::SOLID;

			// setup format_str_to_value_map_
			format_str_to_value_map_["NONE"] = ED_format::NONE;
			format_str_to_value_map_["R32G32B32A32_FLOAT"] = ED_format::R32G32B32A32_FLOAT;
			format_str_to_value_map_["R32G32B32_FLOAT"] = ED_format::R32G32B32_FLOAT;
			format_str_to_value_map_["R32G32_FLOAT"] = ED_format::R32G32_FLOAT;
			format_str_to_value_map_["R32_FLOAT"] = ED_format::R32_FLOAT;
			format_str_to_value_map_["R16G16B16A16_FLOAT"] = ED_format::R16G16B16A16_FLOAT;
			format_str_to_value_map_["R16G16_FLOAT"] = ED_format::R16G16_FLOAT;
			format_str_to_value_map_["R16_FLOAT"] = ED_format::R16_FLOAT;
			format_str_to_value_map_["D32_FLOAT"] = ED_format::D32_FLOAT;
			format_str_to_value_map_["R16G16B16A16_UNORM"] = ED_format::R16G16B16A16_UNORM;
			format_str_to_value_map_["R16G16_UNORM"] = ED_format::R16G16_UNORM;
			format_str_to_value_map_["R16_UNORM"] = ED_format::R16_UNORM;
			format_str_to_value_map_["R8G8B8A8_UNORM"] = ED_format::R8G8B8A8_UNORM;
			format_str_to_value_map_["R8G8_UNORM"] = ED_format::R8G8_UNORM;
			format_str_to_value_map_["R8_UNORM"] = ED_format::R8_UNORM;
			format_str_to_value_map_["R16G16B16A16_SNORM"] = ED_format::R16G16B16A16_SNORM;
			format_str_to_value_map_["R16G16_SNORM"] = ED_format::R16G16_SNORM;
			format_str_to_value_map_["R16_SNORM"] = ED_format::R16_SNORM;
			format_str_to_value_map_["R8G8B8A8_SNORM"] = ED_format::R8G8B8A8_SNORM;
			format_str_to_value_map_["R8G8_SNORM"] = ED_format::R8G8_SNORM;
			format_str_to_value_map_["R8_SNORM"] = ED_format::R8_SNORM;
			format_str_to_value_map_["R32G32B32A32_UINT"] = ED_format::R32G32B32A32_UINT;
			format_str_to_value_map_["R32G32B32_UINT"] = ED_format::R32G32B32_UINT;
			format_str_to_value_map_["R32G32_UINT"] = ED_format::R32G32_UINT;
			format_str_to_value_map_["R32_UINT"] = ED_format::R32_UINT;
			format_str_to_value_map_["R16G16B16A16_UINT"] = ED_format::R16G16B16A16_UINT;
			format_str_to_value_map_["R16G16_UINT"] = ED_format::R16G16_UINT;
			format_str_to_value_map_["R16_UINT"] = ED_format::R16_UINT;
			format_str_to_value_map_["R8G8B8A8_UINT"] = ED_format::R8G8B8A8_UINT;
			format_str_to_value_map_["R8G8_UINT"] = ED_format::R8G8_UINT;
			format_str_to_value_map_["R8_UINT"] = ED_format::R8_UINT;
			format_str_to_value_map_["R32G32B32A32_SINT"] = ED_format::R32G32B32A32_SINT;
			format_str_to_value_map_["R32G32B32_SINT"] = ED_format::R32G32B32_SINT;
			format_str_to_value_map_["R32G32_SINT"] = ED_format::R32G32_SINT;
			format_str_to_value_map_["R32_SINT"] = ED_format::R32_SINT;
			format_str_to_value_map_["R16G16B16A16_SINT"] = ED_format::R16G16B16A16_SINT;
			format_str_to_value_map_["R16G16_SINT"] = ED_format::R16G16_SINT;
			format_str_to_value_map_["R16_SINT"] = ED_format::R16_SINT;
			format_str_to_value_map_["R8G8B8A8_SINT"] = ED_format::R8G8B8A8_SINT;
			format_str_to_value_map_["R8G8_SINT"] = ED_format::R8G8_SINT;
			format_str_to_value_map_["R8_SINT"] = ED_format::R8_SINT;
			format_str_to_value_map_["R32G32B32A32_TYPELESS"] = ED_format::R32G32B32A32_TYPELESS;
			format_str_to_value_map_["R32G32B32_TYPELESS"] = ED_format::R32G32B32_TYPELESS;
			format_str_to_value_map_["R32G32_TYPELESS"] = ED_format::R32G32_TYPELESS;
			format_str_to_value_map_["R32_TYPELESS"] = ED_format::R32_TYPELESS;
			format_str_to_value_map_["R16G16B16A16_TYPELESS"] = ED_format::R16G16B16A16_TYPELESS;
			format_str_to_value_map_["R16G16_TYPELESS"] = ED_format::R16G16_TYPELESS;
			format_str_to_value_map_["R16_TYPELESS"] = ED_format::R16_TYPELESS;
			format_str_to_value_map_["R8G8B8A8_TYPELESS"] = ED_format::R8G8B8A8_TYPELESS;
			format_str_to_value_map_["R8G8_TYPELESS"] = ED_format::R8G8_TYPELESS;
			format_str_to_value_map_["R8_TYPELESS"] = ED_format::R8_TYPELESS;

			// setup depth_comparison_func_str_to_value_map_
			depth_comparison_func_str_to_value_map_["NEVER"] = ED_depth_comparison_func::NEVER;
			depth_comparison_func_str_to_value_map_["LESS"] = ED_depth_comparison_func::LESS;
			depth_comparison_func_str_to_value_map_["EQUAL"] = ED_depth_comparison_func::EQUAL;
			depth_comparison_func_str_to_value_map_["LESS_EQUAL"] = ED_depth_comparison_func::LESS_EQUAL;
			depth_comparison_func_str_to_value_map_["GREATER"] = ED_depth_comparison_func::GREATER;
			depth_comparison_func_str_to_value_map_["NOT_EQUAL"] = ED_depth_comparison_func::NOT_EQUAL;
			depth_comparison_func_str_to_value_map_["GREATER_EQUAL"] = ED_depth_comparison_func::GREATER_EQUAL;
			depth_comparison_func_str_to_value_map_["ALWAYS"] = ED_depth_comparison_func::ALWAYS;

			// setup primitive_topology_str_to_value_map_
			primitive_topology_str_to_value_map_["NONE"] = ED_primitive_topology::NONE;
			primitive_topology_str_to_value_map_["LINE_LIST"] = ED_primitive_topology::LINE_LIST;
			primitive_topology_str_to_value_map_["TRIANGLE_LIST"] = ED_primitive_topology::TRIANGLE_LIST;
		}
	}
	F_nsl_info_tree_reader::~F_nsl_info_tree_reader() {
	}

	F_nsl_info_tree_reader::F_nsl_info_tree_reader(const F_nsl_info_tree_reader& x) :
		shader_compiler_p_(x.shader_compiler_p_),
		info_trees_(x.info_trees_),
		location_offset_to_save_(x.location_offset_to_save_),
		error_stack_p_(x.error_stack_p_)
	{
	}
	F_nsl_info_tree_reader& F_nsl_info_tree_reader::operator = (const F_nsl_info_tree_reader& x) {

		shader_compiler_p_ = x.shader_compiler_p_;
		info_trees_ = x.info_trees_;
		location_offset_to_save_ = x.location_offset_to_save_;
		error_stack_p_ = x.error_stack_p_;

		return *this;
	}
	F_nsl_info_tree_reader::F_nsl_info_tree_reader(F_nsl_info_tree_reader&& x) :
		shader_compiler_p_(x.shader_compiler_p_),
		info_trees_(std::move(x.info_trees_)),
		location_offset_to_save_(x.location_offset_to_save_),
		error_stack_p_(x.error_stack_p_)
	{
	}
	F_nsl_info_tree_reader& F_nsl_info_tree_reader::operator = (F_nsl_info_tree_reader&& x) {

		shader_compiler_p_ = x.shader_compiler_p_;
		info_trees_ = std::move(x.info_trees_);
		location_offset_to_save_ = x.location_offset_to_save_;
		error_stack_p_ = x.error_stack_p_;

		return *this;
	}

	G_string F_nsl_info_tree_reader::parse_value_str(const G_string& value_str) const {

		auto name_manager_p = shader_compiler_p_->name_manager_p();

		if(name_manager_p->is_name_has_target(value_str))
			return H_nsl_utilities::clear_space_head_tail(
				name_manager_p->target(value_str)
			);
		else return H_nsl_utilities::clear_space_head_tail(
				value_str
			);
	}

	b8 F_nsl_info_tree_reader::guarantee_not_empty(b8 is_required) const {

		if(info_trees_.size() == 0) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					location_offset_to_save_,
					"no info provided"
				);
			return false;
		}

		return true;
	}
	b8 F_nsl_info_tree_reader::guarantee_index(u32 index, b8 is_required) const {

		if(info_trees_.size() <= index) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					location_offset_to_save_,
					"not found argument at index \"" + G_to_string(index) + "\""
				);
			return false;
		}

		return true;
	}

	eastl::optional<b8> F_nsl_info_tree_reader::read_b8(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = b8_str_to_value_map_.find(value_str);

		if (it == b8_str_to_value_map_.end()) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<u8> F_nsl_info_tree_reader::read_u8(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		u8 value = 0;

		try{
			value = std::stoull(value_str.c_str());
		}
		catch(std::invalid_argument) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<u16> F_nsl_info_tree_reader::read_u16(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		u16 value = 0;

		try{
			value = std::stoull(value_str.c_str());
		}
		catch(std::invalid_argument) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<u32> F_nsl_info_tree_reader::read_u32(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		u32 value = 0;

		try{
			value = std::stoull(value_str.c_str());
		}
		catch(std::invalid_argument) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<u64> F_nsl_info_tree_reader::read_u64(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		u64 value = 0;

		try{
			value = std::stoull(value_str.c_str());
		}
		catch(std::invalid_argument) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<i8> F_nsl_info_tree_reader::read_i8(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		i8 value = 0;

		try{
			value = std::stoll(value_str.c_str());
		}
		catch(std::invalid_argument) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<i16> F_nsl_info_tree_reader::read_i16(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		i8 value = 0;

		try{
			value = std::stoll(value_str.c_str());
		}
		catch(std::invalid_argument) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<i32> F_nsl_info_tree_reader::read_i32(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		i8 value = 0;

		try{
			value = std::stoll(value_str.c_str());
		}
		catch(std::invalid_argument) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<i64> F_nsl_info_tree_reader::read_i64(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		i8 value = 0;

		try{
			value = std::stoll(value_str.c_str());
		}
		catch(std::invalid_argument) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<f32> F_nsl_info_tree_reader::read_f32(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		f32 value = 0.0f;

		try{
			value = std::stof(value_str.c_str());
		}
		catch(std::invalid_argument) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<f64> F_nsl_info_tree_reader::read_f64(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		f64 value = 0.0f;

		try{
			value = std::stod(value_str.c_str());
		}
		catch(std::invalid_argument) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}
		catch(std::out_of_range) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return value;
	}
	eastl::optional<G_string> F_nsl_info_tree_reader::read_string(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		return parse_value_str(info_trees_[index].name);
	}
	b8 F_nsl_info_tree_reader::guarantee_flag(const G_string& name, b8 is_required) const {

		for(const auto& info_tree : info_trees_) {

			if(parse_value_str(info_tree.name) == name)
				return true;
		}

		if(is_required)
			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				location_offset_to_save_,
				"not found flag \"" + name + "\""
			);
		return false;
	}
	eastl::optional<F_nsl_info_tree_reader> F_nsl_info_tree_reader::read_sub(const G_string& name, b8 is_required) const {

		for(const auto& info_tree : info_trees_) {

			if(parse_value_str(info_tree.name) == name) {

				return F_nsl_info_tree_reader(
					NCPP_FOH_VALID(shader_compiler_p_),
					info_tree.childs,
					info_tree.begin_location,
					error_stack_p_
				);
			}
		}

		if(is_required)
			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				location_offset_to_save_,
				"not found sub info tree \"" + name + "\""
			);
		return eastl::nullopt;
	}
	eastl::optional<E_nsl_element_format> F_nsl_info_tree_reader::read_element_format(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = element_format_str_to_value_map_.find(value_str);

		if (it == element_format_str_to_value_map_.end()) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<E_nsl_semantic_input_class> F_nsl_info_tree_reader::read_semantic_input_class(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = semantic_input_class_str_to_value_map_.find(value_str);

		if (it == semantic_input_class_str_to_value_map_.end()) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<ED_filter> F_nsl_info_tree_reader::read_filter(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = filter_str_to_value_map_.find(value_str);

		if (it == filter_str_to_value_map_.end()) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<ED_texcoord_address_mode> F_nsl_info_tree_reader::read_texcoord_address_mode(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = texcoord_address_mode_str_to_value_map_.find(value_str);

		if (it == texcoord_address_mode_str_to_value_map_.end()) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<ED_cull_mode> F_nsl_info_tree_reader::read_cull_mode(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = cull_mode_str_to_value_map_.find(value_str);

		if (it == cull_mode_str_to_value_map_.end()) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<ED_fill_mode> F_nsl_info_tree_reader::read_fill_mode(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = fill_mode_str_to_value_map_.find(value_str);

		if (it == fill_mode_str_to_value_map_.end()) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<ED_format> F_nsl_info_tree_reader::read_format(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = format_str_to_value_map_.find(value_str);

		if (it == format_str_to_value_map_.end()) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<ED_depth_comparison_func> F_nsl_info_tree_reader::read_depth_comparison_func(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = depth_comparison_func_str_to_value_map_.find(value_str);

		if (it == depth_comparison_func_str_to_value_map_.end()) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return it->second;
	}
	eastl::optional<ED_primitive_topology> F_nsl_info_tree_reader::read_primitive_topology(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = primitive_topology_str_to_value_map_.find(value_str);

		if (it == primitive_topology_str_to_value_map_.end()) {

			if(is_required)
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					error_stack_p_,
					info_trees_[index].begin_location,
					"invalid value \"" + value_str + "\""
				);
			return eastl::nullopt;
		}

		return it->second;
	}



	void F_nsl_reflection::sort_sampler_states(
		const TG_vector<TG_unordered_set<G_string>>& sampler_state_name_sets
	) {
		u32 sampler_state_count = sampler_states.size();
		u32 shader_count = shaders.size();

		// setup name_to_sampler_state_index
		TG_unordered_map<G_string, u32> name_to_sampler_state_index;
		{
			for(u32 i = 0; i < sampler_state_count; ++i) {

				const auto& sampler_state = sampler_states[i];

				name_to_sampler_state_index[sampler_state.name] = i;
			}
		}

		// build sampler_state_indices_vector
		TG_unordered_set<G_string> will_be_moved_sampler_state_set;
		TG_vector<TG_vector<u32>> will_be_moved_sampler_state_indices_vector;
		will_be_moved_sampler_state_indices_vector.reserve(sampler_state_name_sets.size());
		{
			u32 sampler_state_name_set_count = sampler_state_name_sets.size();
			for(u32 i = 0; i < sampler_state_name_set_count; ++i) {

				const auto& sampler_state_name_set = sampler_state_name_sets[i];

				TG_vector<G_string> sampler_state_names;
				sampler_state_names.reserve(sampler_state_name_set.size());

				// build sampler_state_names
				for(const auto& sampler_state_name : sampler_state_name_set) {

					NCPP_ASSERT(name_to_sampler_state_index.find(sampler_state_name) != name_to_sampler_state_index.end())
						<< "sampler_state not found";

					NCPP_ASSERT(will_be_moved_sampler_state_set.find(sampler_state_name) == will_be_moved_sampler_state_set.end())
						<< "a sampler_state name can only be in a unique sampler_state name set";

					sampler_state_names.push_back(sampler_state_name);
				}

				// sort sampler_state_names
				{
					auto sort_func = [](const G_string& a, const G_string& b) -> b8 {

						return TF_hash<G_string>{}(a) > TF_hash<G_string>{}(b);
					};
					eastl::sort(sampler_state_names.begin(), sampler_state_names.end(), sort_func);
				}

				// build and store will_be_moved_sampler_state_indices
				{
					TG_vector<u32> will_be_moved_sampler_state_indices;
					will_be_moved_sampler_state_indices.reserve(sampler_state_names.size());

					for(const auto& sampler_state_name : sampler_state_names) {

						will_be_moved_sampler_state_indices.push_back(
							name_to_sampler_state_index[sampler_state_name]
						);
					}

					will_be_moved_sampler_state_indices_vector.push_back(
						std::move(will_be_moved_sampler_state_indices)
					);
				}
			}
		}

		// bind new actual slots
		TG_vector<TG_vector<u32>> new_actual_slots_vector(sampler_states.size());
		{
			// setup new_actual_slots_vector
			for(auto& new_actual_slots : new_actual_slots_vector) {

				new_actual_slots.resize(shader_count);

				for(auto& new_actual_slot : new_actual_slots) {

					new_actual_slot = -1;
				}
			}

			// build new_actual_slots_vector
			for(u32 i = 0; i < shader_count; ++i) {

				u32 next_actual_slot = 0;

				// for sampler_states that name is in sampler_state name sets
				for(auto& will_be_moved_sampler_state_indices : will_be_moved_sampler_state_indices_vector) {

					for(auto will_be_moved_sampler_state_index : will_be_moved_sampler_state_indices) {

						auto& sampler_state = sampler_states[will_be_moved_sampler_state_index];

						if(sampler_state.actual_slots[i] == -1)
							continue;

						auto& new_actual_slot = new_actual_slots_vector[will_be_moved_sampler_state_index][i];

						new_actual_slot = next_actual_slot;

						++next_actual_slot;
					}
				}

				// for sampler_states that name is not in sampler_state name sets
				for(u32 j = 0; j < sampler_state_count; ++j) {

					auto& sampler_state = sampler_states[j];

					auto old_actual_slot = sampler_state.actual_slots[i];
					auto& new_actual_slot = new_actual_slots_vector[j][i];

					if(
						(old_actual_slot != -1)
						&& (new_actual_slot == -1)
					) {
						new_actual_slot = next_actual_slot;

						++next_actual_slot;
					}
				}
			}
		}

		// store new_actual_slots_vector
		for(u32 i = 0; i < sampler_state_count; ++i) {

			auto& sampler_state = sampler_states[i];

			sampler_state.actual_slots = new_actual_slots_vector[i];
		}
	}
	void F_nsl_reflection::sort_resources(
		const TG_vector<TG_unordered_set<G_string>>& resource_name_sets,
		E_nsl_resource_type_class type_class
	) {
		u32 resource_count = resources.size();
		u32 shader_count = shaders.size();

		// setup name_to_resource_index
		TG_unordered_map<G_string, u32> name_to_resource_index;
		{
			for(u32 i = 0; i < resource_count; ++i) {

				const auto& resource = resources[i];

				name_to_resource_index[resource.name] = i;
			}
		}

		// build resource_indices_vector
		TG_unordered_set<G_string> will_be_moved_resource_set;
		TG_vector<TG_vector<u32>> will_be_moved_resource_indices_vector;
		will_be_moved_resource_indices_vector.reserve(resource_name_sets.size());
		{
			u32 resource_name_set_count = resource_name_sets.size();
			for(u32 i = 0; i < resource_name_set_count; ++i) {

				const auto& resource_name_set = resource_name_sets[i];

				TG_vector<G_string> resource_names;
				resource_names.reserve(resource_name_set.size());

				// build resource_names
				for(const auto& resource_name : resource_name_set) {

					NCPP_ASSERT(name_to_resource_index.find(resource_name) != name_to_resource_index.end())
					<< "resource not found";

					NCPP_ASSERT(will_be_moved_resource_set.find(resource_name) == will_be_moved_resource_set.end())
						<< "a resource name can only be in a unique resource name set";

					NCPP_ASSERT(resources[name_to_resource_index[resource_name]].type_class == type_class)
						<< "invalid resource type class";

					resource_names.push_back(resource_name);
				}

				// sort resource_names
				{
					auto sort_func = [](const G_string& a, const G_string& b) -> b8 {

						return TF_hash<G_string>{}(a) > TF_hash<G_string>{}(b);
					};
					eastl::sort(resource_names.begin(), resource_names.end(), sort_func);
				}

				// build and store will_be_moved_resource_indices
				{
					TG_vector<u32> will_be_moved_resource_indices;
					will_be_moved_resource_indices.reserve(resource_names.size());

					for(const auto& resource_name : resource_names) {

						will_be_moved_resource_indices.push_back(
							name_to_resource_index[resource_name]
						);
					}

					will_be_moved_resource_indices_vector.push_back(
						std::move(will_be_moved_resource_indices)
					);
				}
			}
		}

		// bind new actual slots
		TG_vector<TG_vector<u32>> new_actual_slots_vector(resources.size());
		{
			// setup new_actual_slots_vector
			for(auto& new_actual_slots : new_actual_slots_vector) {

				new_actual_slots.resize(shader_count);

				for(auto& new_actual_slot : new_actual_slots) {

					new_actual_slot = -1;
				}
			}

			// build new_actual_slots_vector
			for(u32 i = 0; i < shader_count; ++i) {

				u32 next_actual_slot = 0;

				// for resources that name is in resource name sets
				for(auto& will_be_moved_resource_indices : will_be_moved_resource_indices_vector) {

					for(auto will_be_moved_resource_index : will_be_moved_resource_indices) {

						auto& resource = resources[will_be_moved_resource_index];

						if(resource.actual_slots[i] == -1)
							continue;

						auto& new_actual_slot = new_actual_slots_vector[will_be_moved_resource_index][i];

						new_actual_slot = next_actual_slot;

						++next_actual_slot;
					}
				}

				// for resources that name is not in resource name sets
				for(u32 j = 0; j < resource_count; ++j) {

					auto& resource = resources[j];

					auto old_actual_slot = resource.actual_slots[i];
					auto& new_actual_slot = new_actual_slots_vector[j][i];

					if(resource.type_class != type_class)
						continue;

					if(
						(old_actual_slot != -1)
						&& (new_actual_slot == -1)
					) {
						new_actual_slot = next_actual_slot;

						++next_actual_slot;
					}
				}
			}
		}

		// store new_actual_slots_vector
		for(u32 i = 0; i < resource_count; ++i) {

			auto& resource = resources[i];

			if(resource.type_class != type_class)
				continue;

			resource.actual_slots = new_actual_slots_vector[i];
		}
	}



	F_nsl_shader_module_manager::F_nsl_shader_module_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
		register_system_source(
			"nrhi",
"\n"
"define NRHI_VERSION(" + G_to_string(NRHI_VERSION_NUMBER) + ")\n"
"\n"
		);
	}
	F_nsl_shader_module_manager::~F_nsl_shader_module_manager() {
	}

	void F_nsl_shader_module_manager::register_system_source(const G_string& path, const G_string src_content) {

		NCPP_ASSERT(!is_has_system_source(path))
			<< "system source of path "
			<< T_cout_value(path)
			<< " already registered";

		system_source_map_[path] = src_content;
	}

	eastl::optional<F_nsl_shader_module_manager::F_load_src_content_result> F_nsl_shader_module_manager::load_src_content(
		TKPA_valid<F_nsl_translation_unit> from_unit_p,
		const G_string& path,
		const F_nsl_ast_tree& tree,
		G_string& out_src_content
	)
	{
		auto it = system_source_map_.find(path);

		if(it == system_source_map_.end())
			return eastl::nullopt;
		else
		{
			out_src_content = it->second;

			return F_load_src_content_result{
				.abs_path = path
			};
		}
	}
	TK<F_nsl_translation_unit> F_nsl_shader_module_manager::load(
		TKPA_valid<F_nsl_translation_unit> from_unit_p,
		const G_string& path,
		const F_nsl_ast_tree& tree
	)
	{
		G_string src_content;

		auto it = abs_path_to_translation_unit_p_.find(path);

		if(it == abs_path_to_translation_unit_p_.end())
		{
			auto load_result_opt = load_src_content(
				from_unit_p,
				path,
				tree,
				src_content
			);

			if (load_result_opt)
			{
				const auto& load_result = std::move(load_result_opt.value());
				const auto& abs_path = load_result.abs_path;

				it = abs_path_to_translation_unit_p_.find(abs_path);

				if (it == abs_path_to_translation_unit_p_.end())
				{
					return shader_compiler_p_->translation_unit_manager_p()->create_unit(
						src_content,
						abs_path
					).no_requirements();
				}
				else return it->second;
			}
		}
		else return it->second;

		NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
			&(from_unit_p->error_group_p()->stack()),
			tree.begin_location,
			"can't not load shader module at path \"" + path + "\""
		);
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
	eastl::optional<G_string> A_nsl_object::apply(
		const F_nsl_ast_tree& tree
	) {
		return G_string();
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
		auto& tree = trees[index];

		auto& object_implementation = tree.object_implementation;

		G_string path = H_nsl_utilities::clear_space_head_tail(object_implementation.bodies[0].content);

		auto shader_module_manager_p = shader_compiler_p()->shader_module_manager_p();
		auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();

		imported_unit_p_ = shader_module_manager_p->load(
			unit_p,
			path,
			tree
		);

		if(!imported_unit_p_)
			return eastl::nullopt;

		if(
			!(
				translation_unit_compiler_p->prepare_unit(
					NCPP_FOH_VALID(imported_unit_p_),
					context
				)
			)
		) {
			return eastl::nullopt;
		}

		unit_p->add_dependency(
			NCPP_FOH_VALID(imported_unit_p_)
		);

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_import_object_type::F_nsl_import_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"import",
			false,
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
	eastl::optional<G_string> F_nsl_define_object::apply(
		const F_nsl_ast_tree& tree
	) {
		auto output_language_p = shader_compiler_p()->output_language_p();

		return output_language_p->define_to_string(
			translation_unit_p(),
			name(),
			target
		);
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
	eastl::optional<G_string> F_nsl_undef_object::apply(
		const F_nsl_ast_tree& tree
	) {
		auto output_language_p = shader_compiler_p()->output_language_p();

		return output_language_p->undef_to_string(
			translation_unit_p(),
			name()
		);
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
				shader_compiler_p(),
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
		u32 element_count = data_type_manager_p->element_count(target_type);
		G_string target_binding = tree.object_implementation.name;
		{
			auto it = context.current_object_config.find("target_binding");
			if(it != context.current_object_config.end()) {

				auto value_opt = it->second.read_string(0);

				if(!value_opt)
					return eastl::nullopt;

				target_binding = value_opt.value();
			}
		}

		// register semantic
		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>(tree.object_implementation.name);
		data_type_manager_p->register_semantic(
			tree.object_implementation.name,
			F_nsl_semantic_info {
				.target_type = target_type,
				.target_binding = target_binding,
				.element_format = element_format,
				.element_count = element_count,
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

		structure_info.begin_location = tree.begin_location;
		structure_info.end_location = tree.end_location;
		structure_info.translation_unit_p = unit_p.no_requirements();

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
					shader_compiler_p(),
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

			F_nsl_info_tree_reader argument_child_info_tree_reader(
				shader_compiler_p(),
				argument_child_info_tree.childs,
				argument_child_info_tree.begin_childs_location,
				&(unit_p->error_group_p()->stack())
			);

			// check for count
			u32 count = 1;
			b8 is_array = false;
			if(argument_child_info_tree.childs.size() >= 2) {

				auto value_opt = argument_child_info_tree_reader.read_u32(1);

				if(!value_opt)
					return eastl::nullopt;

				count = value_opt.value();
				is_array = true;
			}

			auto type_opt = argument_child_info_tree_reader.read_string(0);

			if(!type_opt)
				return eastl::nullopt;

			structure_info.argument_members.push_back(
				F_nsl_data_argument_member {
					.argument = F_nsl_data_argument {
						.name = argument_child_info_tree.name,
						.type = type_opt.value(),
						.count = count,
						.is_array = is_array,
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

			data_argument_config_map = {};
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

		// register structure
		name_manager_p->template T_register_name<FE_nsl_name_types::STRUCTURE>(tree.object_implementation.name);
		data_type_manager_p->register_structure(
			tree.object_implementation.name,
			structure_info
		);

		return std::move(argument_childs);
	}
	eastl::optional<G_string> F_nsl_structure_object::apply(
		const F_nsl_ast_tree& tree
	) {
		auto output_language_p = shader_compiler_p()->output_language_p();

		const auto& structure_info = shader_compiler_p()->data_type_manager_p()->structure_info(
			name()
		);

		return output_language_p->structure_to_string(
			translation_unit_p(),
			{
				name(),
				structure_info
			}
		);
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

		enumeration_info.begin_location = tree.begin_location;
		enumeration_info.end_location = tree.end_location;
		enumeration_info.translation_unit_p = unit_p.no_requirements();

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

				F_nsl_info_tree_reader info_tree_reader(
					shader_compiler_p(),
					value_child_info_tree.childs,
					value_child_info_tree.begin_childs_location,
					&(unit_p->error_group_p()->stack())
				);

				auto value_opt = info_tree_reader.read_u64(0);

				if(!value_opt)
					return eastl::nullopt;

				value = value_opt.value();
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

		// register enumeration
		name_manager_p->template T_register_name<FE_nsl_name_types::STRUCTURE>(tree.object_implementation.name);
		data_type_manager_p->register_enumeration(
			tree.object_implementation.name,
			enumeration_info
		);

		return std::move(value_childs);
	}
	eastl::optional<G_string> F_nsl_enumeration_object::apply(
		const F_nsl_ast_tree& tree
	) {
		auto output_language_p = shader_compiler_p()->output_language_p();

		const auto& enumeration_info = shader_compiler_p()->data_type_manager_p()->enumeration_info(
			name()
		);

		return output_language_p->enumeration_to_string(
			translation_unit_p(),
			{
				name(),
				enumeration_info
			}
		);
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

		resource_info.begin_location = tree.begin_location;
		resource_info.end_location = tree.end_location;
		resource_info.translation_unit_p = unit_p.no_requirements();

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

		// setup child info trees reader
		F_nsl_info_tree_reader child_info_trees_reader(
			shader_compiler_p(),
			child_info_trees,
			object_implementation.bodies[0].begin_location,
			&(unit_p->error_group_p()->stack())
		);

		// get info about resource type,...
		auto& type_child_info_tree = child_info_trees[0];

		TG_vector<F_nsl_ast_tree> child_trees(child_info_trees.size());
		child_trees[0] = F_nsl_ast_tree {
			.type = E_nsl_ast_tree_type::INFO_TREE,
			.info_tree = type_child_info_tree,
			.begin_location = type_child_info_tree.begin_location,
			.end_location = type_child_info_tree.end_location
		};

		TG_vector<G_string> type_args(type_child_info_tree.childs.size());
		for(u32 i = 0; i < type_args.size(); ++i) {

			const auto& type_arg = type_child_info_tree.childs[i].name;

			if(name_manager_p->is_name_has_target(type_arg))
				type_args[i] = name_manager_p->target(type_arg);
			else type_args[i] = type_arg;
		}

		resource_info.type = name_manager_p->target(type_child_info_tree.name);
		resource_info.type_args = type_args;

		// check for dimension sizes and dimension count
		{
			u32 dimension_count = child_info_trees.size() - 1;

			for(u32 i = 1; i <= dimension_count; ++i) {

				auto& dimension_size_child_info_tree = child_info_trees[i];

				auto value_opt = child_info_trees_reader.read_u32(i);

				if(!value_opt)
					return eastl::nullopt;

				resource_info.dimension_sizes[i - 1] = value_opt.value();

				child_trees[i] = F_nsl_ast_tree {
					.type = E_nsl_ast_tree_type::INFO_TREE,
					.info_tree = dimension_size_child_info_tree,
					.begin_location = dimension_size_child_info_tree.begin_location,
					.end_location = dimension_size_child_info_tree.end_location
				};
			}

			if(dimension_count)
			{
				resource_info.dimension_count = dimension_count;
				resource_info.is_array = true;
			}
		}

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

				u32 shader_count = shaders_info_tree_reader.info_trees().size();

				for(u32 i = 0; i < shader_count; ++i) {

					auto value_opt = shaders_info_tree_reader.read_string(i);

					if(!value_opt)
						return eastl::nullopt;

					resource_info.shader_filters.insert(value_opt.value());
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
	eastl::optional<G_string> F_nsl_resource_object::apply(
		const F_nsl_ast_tree& tree
	) {
		auto output_language_p = shader_compiler_p()->output_language_p();

		const auto& resource_info = shader_compiler_p()->resource_manager_p()->resource_info(
			name()
		);

		return output_language_p->resource_to_string(
			translation_unit_p(),
			{
				name(),
				resource_info
			}
		);
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

		uniform_info.begin_location = tree.begin_location;
		uniform_info.end_location = tree.end_location;
		uniform_info.translation_unit_p = unit_p.no_requirements();

		auto info_trees_opt = H_nsl_utilities::build_info_trees(
			object_implementation.bodies[0].content,
			object_implementation.bodies[0].begin_location,
			&(unit_p->error_group_p()->stack())
		);

		if(!info_trees_opt) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				object_implementation.bodies[0].begin_location,
				"can't not parse uniform type"
			);
			return eastl::nullopt;
		}

		F_nsl_info_tree_reader info_tree_reader(
			shader_compiler_p(),
			info_trees_opt.value(),
			object_implementation.bodies[0].begin_location,
			&(unit_p->error_group_p()->stack())
		);

		// read uniform type
		{
			auto value_opt = info_tree_reader.read_string(0);

			if(!value_opt)
				return eastl::nullopt;

			uniform_info.type = value_opt.value();
		}

		// check for count
		u32 count = 1;
		b8 is_array = false;
		if(info_tree_reader.info_trees().size() >= 2) {

			auto value_opt = info_tree_reader.read_u32(1);

			if(!value_opt)
				return eastl::nullopt;

			count = value_opt.value();
			is_array = true;
		}

		// check for buffer annotation
		uniform_info.buffer = context.default_constant_buffer;
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



	F_nsl_default_constant_buffer_object::F_nsl_default_constant_buffer_object(
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
	F_nsl_default_constant_buffer_object::~F_nsl_default_constant_buffer_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_default_constant_buffer_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		context.parent_object_p = NCPP_KTHIS().no_requirements();

		context.default_constant_buffer = shader_compiler_p()->name_manager_p()->target(
			H_nsl_utilities::clear_space_head_tail(
				object_implementation.bodies[0].content
			)
		);

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_default_constant_buffer_object_type::F_nsl_default_constant_buffer_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"default_constant_buffer",
			false,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_default_constant_buffer_object_type::~F_nsl_default_constant_buffer_object_type() {
	}

	TK<A_nsl_object> F_nsl_default_constant_buffer_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_default_constant_buffer_object>()(
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

		sampler_state_info.begin_location = tree.begin_location;
		sampler_state_info.end_location = tree.end_location;
		sampler_state_info.translation_unit_p = unit_p.no_requirements();

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

				u32 shader_count = shaders_info_tree_reader.info_trees().size();

				for(u32 i = 0; i < shader_count; ++i) {

					auto value_opt = shaders_info_tree_reader.read_string(i);

					if(!value_opt)
						return eastl::nullopt;

					sampler_state_info.shader_filters.insert(value_opt.value());
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
		name_manager_p->template T_register_name<FE_nsl_name_types::SAMPLER_STATE>(tree.object_implementation.name);
		sampler_state_manager_p->register_sampler_state(
			tree.object_implementation.name,
			sampler_state_info
		);

		return TG_vector<F_nsl_ast_tree>();
	}
	eastl::optional<G_string> F_nsl_sampler_state_object::apply(
		const F_nsl_ast_tree& tree
	) {
		auto output_language_p = shader_compiler_p()->output_language_p();

		const auto& sampler_state_info = shader_compiler_p()->sampler_state_manager_p()->sampler_state_info(
			name()
		);

		return output_language_p->sampler_state_to_string(
			translation_unit_p(),
			{
			  	name(),
			  	sampler_state_info
			}
		);
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



	F_nsl_pipeline_state_object::F_nsl_pipeline_state_object(
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
	F_nsl_pipeline_state_object::~F_nsl_pipeline_state_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_pipeline_state_object::recursive_build_ast_tree(
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
		auto pipeline_state_manager_p = shader_compiler_p()->pipeline_state_manager_p();

		F_nsl_pipeline_state_info pipeline_state_info;
		pipeline_state_info.config_map = context.current_object_config;

		pipeline_state_info.options = context.default_pipeline_state_options;

		pipeline_state_info.begin_location = tree.begin_location;
		pipeline_state_info.end_location = tree.end_location;
		pipeline_state_info.translation_unit_p = unit_p.no_requirements();

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
				"can't not parse pipeline_state shaders"
			);
			return eastl::nullopt;
		}

		auto& child_info_trees = child_info_trees_opt.value();

		if(child_info_trees.size() == 0) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				object_implementation.bodies[0].begin_location,
				"require pipeline_state shaders"
			);
			return eastl::nullopt;
		}

		u32 shader_count = child_info_trees.size();

		pipeline_state_info.shaders.resize(shader_count);

		TG_vector<F_nsl_ast_tree> child_trees(shader_count);
		for(u32 i = 0; i < shader_count; ++i) {

			auto& child_info_tree = child_info_trees[i];

			pipeline_state_info.shaders[i] = name_manager_p->target(child_info_tree.name);

			child_trees[i] = F_nsl_ast_tree {
				.type = E_nsl_ast_tree_type::INFO_TREE,
				.info_tree = child_info_tree,
				.begin_location = child_info_tree.begin_location,
				.end_location = child_info_tree.end_location
			};
		}

		// check for color_formats annotation
		{
			auto it = context.current_object_config.find("color_formats");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				u32 color_format_count = info_tree_reader.info_trees().size();

				for(u32 i = 0; i < color_format_count; ++i) {

					auto value_opt = info_tree_reader.read_format(i);

					if(!value_opt)
						return eastl::nullopt;

					pipeline_state_info.options.graphics.color_formats[i] = value_opt.value();
				}
			}
		}

		// check for depth_stencil annotation
		{
			auto it = context.current_object_config.find("depth_stencil");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				// enable_depth_test attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("enable_depth_test", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_b8(0);

						if(!value_opt)
							return eastl::nullopt;

						pipeline_state_info.options.graphics.depth_stencil_desc.
						enable_depth_test = value_opt.value();
					}
				}

				// fill_mode attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("format", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_format(0);

						if(!value_opt)
							return eastl::nullopt;

						pipeline_state_info.options.graphics.depth_stencil_desc.format = value_opt.value();
					}
				}

				// depth_comparison_func attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("depth_comparison_func", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_depth_comparison_func(0);

						if(!value_opt)
							return eastl::nullopt;

						pipeline_state_info.options.graphics.depth_stencil_desc.depth_comparison_func = value_opt.value();
					}
				}

				// depth_buffer_write attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("depth_buffer_write", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_b8(0);

						if(!value_opt)
							return eastl::nullopt;

						pipeline_state_info.options.graphics.depth_stencil_desc.depth_buffer_write = value_opt.value();
					}
				}
			}
		}

		// check for rasterizer annotation
		{
			auto it = context.current_object_config.find("rasterizer");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				// cull_mode attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("cull_mode", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_cull_mode(0);

						if(!value_opt)
							return eastl::nullopt;

						pipeline_state_info.options.graphics.rasterizer_desc.cull_mode = value_opt.value();
					}
				}

				// fill_mode attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("fill_mode", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_fill_mode(0);

						if(!value_opt)
							return eastl::nullopt;

						pipeline_state_info.options.graphics.rasterizer_desc.fill_mode = value_opt.value();
					}
				}

				// front_counter_clock_wise attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("front_counter_clock_wise", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_b8(0);

						if(!value_opt)
							return eastl::nullopt;

						pipeline_state_info.options.graphics.rasterizer_desc.front_counter_clock_wise = value_opt.value();
					}
				}
			}
		}

		// check for primitive_topology annotation
		{
			auto it = context.current_object_config.find("primitive_topology");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_primitive_topology(0);

				if(!value_opt)
					return eastl::nullopt;

				pipeline_state_info.options.graphics.primitive_topology = value_opt.value();
			}
		}

		// register pipeline_state
		name_manager_p->template T_register_name<FE_nsl_name_types::PIPELINE_STATE>(tree.object_implementation.name);
		pipeline_state_manager_p->register_pipeline_state(
			tree.object_implementation.name,
			pipeline_state_info
		);

		return std::move(child_trees);
	}



	F_nsl_pipeline_state_object_type::F_nsl_pipeline_state_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"pipeline_state",
			true,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_pipeline_state_object_type::~F_nsl_pipeline_state_object_type() {
	}

	TK<A_nsl_object> F_nsl_pipeline_state_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_pipeline_state_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_default_pipeline_state_options_object::F_nsl_default_pipeline_state_options_object(
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
	F_nsl_default_pipeline_state_options_object::~F_nsl_default_pipeline_state_options_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_default_pipeline_state_options_object::recursive_build_ast_tree(
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

		F_general_pipeline_state_options default_pipeline_state_options;

		// check for color_formats annotation
		{
			auto it = context.current_object_config.find("color_formats");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				u32 color_format_count = info_tree_reader.info_trees().size();

				for(u32 i = 0; i < color_format_count; ++i) {

					auto value_opt = info_tree_reader.read_format(i);

					if(!value_opt)
						return eastl::nullopt;

					default_pipeline_state_options.graphics.color_formats[i] = value_opt.value();
				}
			}
		}

		// check for depth_stencil annotation
		{
			auto it = context.current_object_config.find("depth_stencil");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				// enable_depth_test attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("enable_depth_test", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_b8(0);

						if(!value_opt)
							return eastl::nullopt;

						default_pipeline_state_options.graphics.depth_stencil_desc.
						enable_depth_test = value_opt.value();
					}
				}

				// fill_mode attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("format", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_format(0);

						if(!value_opt)
							return eastl::nullopt;

						default_pipeline_state_options.graphics.depth_stencil_desc.format = value_opt.value();
					}
				}

				// depth_comparison_func attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("depth_comparison_func", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_depth_comparison_func(0);

						if(!value_opt)
							return eastl::nullopt;

						default_pipeline_state_options.graphics.depth_stencil_desc.depth_comparison_func = value_opt.value();
					}
				}

				// depth_buffer_write attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("depth_buffer_write", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_b8(0);

						if(!value_opt)
							return eastl::nullopt;

						default_pipeline_state_options.graphics.depth_stencil_desc.depth_buffer_write = value_opt.value();
					}
				}
			}
		}

		// check for rasterizer annotation
		{
			auto it = context.current_object_config.find("rasterizer");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				// cull_mode attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("cull_mode", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_cull_mode(0);

						if(!value_opt)
							return eastl::nullopt;

						default_pipeline_state_options.graphics.rasterizer_desc.cull_mode = value_opt.value();
					}
				}

				// fill_mode attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("fill_mode", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_fill_mode(0);

						if(!value_opt)
							return eastl::nullopt;

						default_pipeline_state_options.graphics.rasterizer_desc.fill_mode = value_opt.value();
					}
				}

				// front_counter_clock_wise attribute
				{
					auto sub_info_tree_reader_opt = info_tree_reader.read_sub("front_counter_clock_wise", false);

					if(sub_info_tree_reader_opt) {

						const auto& sub_info_tree_reader = sub_info_tree_reader_opt.value();

						auto value_opt = sub_info_tree_reader.read_b8(0);

						if(!value_opt)
							return eastl::nullopt;

						default_pipeline_state_options.graphics.rasterizer_desc.front_counter_clock_wise = value_opt.value();
					}
				}
			}
		}

		// check for primitive_topology annotation
		{
			auto it = context.current_object_config.find("primitive_topology");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_primitive_topology(0);

				if(!value_opt)
					return eastl::nullopt;

				default_pipeline_state_options.graphics.primitive_topology = value_opt.value();
			}
		}

		// store default_pipeline_state_options
		context.default_pipeline_state_options = default_pipeline_state_options;

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_default_pipeline_state_options_object_type::F_nsl_default_pipeline_state_options_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"default_pipeline_state_options",
			false,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_default_pipeline_state_options_object_type::~F_nsl_default_pipeline_state_options_object_type() {
	}

	TK<A_nsl_object> F_nsl_default_pipeline_state_options_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_default_pipeline_state_options_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_input_assembler_object::F_nsl_input_assembler_object(
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
	F_nsl_input_assembler_object::~F_nsl_input_assembler_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_input_assembler_object::recursive_build_ast_tree(
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
		auto input_assembler_manager_p = shader_compiler_p()->input_assembler_manager_p();

		F_nsl_input_assembler_info input_assembler_info;
		input_assembler_info.config_map = context.current_object_config;

		input_assembler_info.begin_location = tree.begin_location;
		input_assembler_info.end_location = tree.end_location;
		input_assembler_info.translation_unit_p = unit_p.no_requirements();

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
				"can't not parse input_assembler shaders"
			);
			return eastl::nullopt;
		}

		F_nsl_input_attribute_config_map attribute_config_map;

		auto& child_info_trees = child_info_trees_opt.value();

		if(child_info_trees.size() == 0) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				object_implementation.bodies[0].begin_location,
				"require input_assembler shaders"
			);
			return eastl::nullopt;
		}

		u32 shader_count = child_info_trees.size();

		TG_vector<F_nsl_ast_tree> child_trees(shader_count);
		for(u32 i = 0; i < shader_count; ++i) {

			auto& child_info_tree = child_info_trees[i];

			if(child_info_tree.name[0] == '@') {

				attribute_config_map[
					child_info_tree.name.substr(1, child_info_tree.name.length() - 1)
				] = F_nsl_info_tree_reader(
					shader_compiler_p(),
					child_info_tree.childs,
					child_info_tree.begin_childs_location,
					&(unit_p->error_group_p()->stack())
				);
				continue;
			}

			F_nsl_input_attribute attribute = {
				.semantic = name_manager_p->target(child_info_tree.name)
			};

			// @buffer annotation
			{
				auto it = attribute_config_map.find("buffer");
				if(it != attribute_config_map.end()) {

					auto value_opt = it->second.read_u32(0);

					if(!value_opt)
						return eastl::nullopt;

					attribute.buffer = value_opt.value();
				}
 			}

			// @offset annotation
			{
				auto it = attribute_config_map.find("offset");
				if(it != attribute_config_map.end()) {

					auto value_opt = it->second.read_i32(0);

					if(!value_opt)
						return eastl::nullopt;

					attribute.offset = value_opt.value();
				}
			}

			input_assembler_info.attributes.push_back(attribute);

			child_trees[i] = F_nsl_ast_tree {
				.type = E_nsl_ast_tree_type::INFO_TREE,
				.info_tree = child_info_tree,
				.begin_location = child_info_tree.begin_location,
				.end_location = child_info_tree.end_location
			};

			attribute_config_map = {};
		}

		// register input_assembler
		name_manager_p->template T_register_name<FE_nsl_name_types::PIPELINE_STATE>(tree.object_implementation.name);
		input_assembler_manager_p->register_input_assembler(
			tree.object_implementation.name,
			input_assembler_info
		);

		return std::move(child_trees);
	}



	F_nsl_input_assembler_object_type::F_nsl_input_assembler_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"input_assembler",
			true,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_input_assembler_object_type::~F_nsl_input_assembler_object_type() {
	}

	TK<A_nsl_object> F_nsl_input_assembler_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_input_assembler_object>()(
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
		ED_shader_type type,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		),
		type_(type)
	{
	}
	A_nsl_shader_object::~A_nsl_shader_object() {
	}

	eastl::optional<G_string> A_nsl_shader_object::apply_shader_with_customizations(
		const F_nsl_ast_tree& tree,
		const G_string& pre_shader_keyword
	) {
		auto output_language_p = shader_compiler_p()->output_language_p();
		auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();

		auto childs_to_string_opt = translation_unit_compiler_p->ast_trees_to_string(
			tree.childs
		);
		if(!childs_to_string_opt)
			return childs_to_string_opt.value();

		G_string childs_to_string = childs_to_string_opt.value();

		return output_language_p->shader_object_to_string(
			translation_unit_p(),
			NCPP_KTHIS(),
			childs_to_string,
			pre_shader_keyword
		);
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
		auto shader_manager_p = shader_compiler_p()->shader_manager_p();

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

			F_nsl_info_tree_reader param_child_info_tree_reader(
				shader_compiler_p(),
				param_child_info_tree.childs,
				param_child_info_tree.begin_childs_location,
				&(unit_p->error_group_p()->stack())
			);

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
				] = param_child_info_tree_reader;
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
				// check for count
				u32 count = 1;
				b8 is_array = false;
				if(param_child_info_tree_reader.info_trees().size() >= 2) {

					auto value_opt = param_child_info_tree_reader.read_u32(1);

					if(!value_opt)
						return eastl::nullopt;

					count = value_opt.value();
					is_array = true;
				}

				auto type_opt = param_child_info_tree_reader.read_string(0);

				if(!type_opt)
					return eastl::nullopt;

				data_params_.push_back(
					F_nsl_data_param {
						.argument = F_nsl_data_argument{
							.name = param_child_info_tree.name,
							.type = type_opt.value(),
							.count = count,
							.is_array = is_array,
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

			data_argument_config_map = {};
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
		shader_manager_p->register_shader(
			tree.object_implementation.name,
			NCPP_KTHIS()
		);

		return std::move(childs);
	}
	eastl::optional<G_string> A_nsl_shader_object::apply(
		const F_nsl_ast_tree& tree
	) {
		return apply_shader_with_customizations(
			tree
		);
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
			ED_shader_type::VERTEX,
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

		auto name_manager_p = shader_compiler_p()->name_manager_p();

		// @input_assembler annotation
		{
			auto it = context.current_object_config.find("input_assembler");
			if(it != context.current_object_config.end()) {

				auto value_opt = it->second.read_string(0);

				if(!value_opt)
					return eastl::nullopt;

				input_assembler_name_ = name_manager_p->target(value_opt.value());
			}
		}

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
			ED_shader_type::PIXEL,
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
			ED_shader_type::COMPUTE,
			name
		)
	{
	}
	F_nsl_compute_shader_object::~F_nsl_compute_shader_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_compute_shader_object::recursive_build_ast_tree(
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

		// @thread_group_size annotation
		{
			auto it = context.current_object_config.find("thread_group_size");
			if(it != context.current_object_config.end()) {

				{
					auto value_opt = it->second.read_u32(0);

					if(!value_opt)
						return eastl::nullopt;

					thread_group_size_.x = value_opt.value();
				}
				{
					auto value_opt = it->second.read_u32(1);

					if(!value_opt)
						return eastl::nullopt;

					thread_group_size_.y = value_opt.value();
				}
				{
					auto value_opt = it->second.read_u32(2);

					if(!value_opt)
						return eastl::nullopt;

					thread_group_size_.z = value_opt.value();
				}
			}
		}

		return std::move(childs);
	}
	eastl::optional<G_string> F_nsl_compute_shader_object::apply(
		const F_nsl_ast_tree& tree
	) {
		return apply_shader_with_customizations(
			tree,
			G_string("[numthreads(")
			+ G_to_string(thread_group_size_.x)
			+ ","
			+ G_to_string(thread_group_size_.y)
			+ ","
			+ G_to_string(thread_group_size_.z)
			+ ")]\n"
		);
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
			TU<F_nsl_default_constant_buffer_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_sampler_state_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_pipeline_state_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_default_pipeline_state_options_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_input_assembler_object_type>()(shader_compiler_p_)
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

	b8 F_nsl_translation_unit_compiler::sort_units_internal() {

		auto translation_unit_manager_p = shader_compiler_p_->translation_unit_manager_p();

		sorted_unit_p_vector_.resize(0);
		sorted_unit_p_vector_.reserve(
			translation_unit_manager_p->translation_unit_p_vector().size()
		);

		// BFS traversal to build up sorted_unit_p_vector_
		{
			TG_queue<TK<F_nsl_translation_unit>> unit_p_queue;
			TG_unordered_set<F_nsl_translation_unit*> processed_unit_p_set;

			// root node: main_unit_p_
			unit_p_queue.push(main_unit_p_);

			// traversal loop
			while (unit_p_queue.size())
			{
				// cache unit_p_queue and then release it
				TG_queue<TK<F_nsl_translation_unit>> temp_unit_p_queue = std::move(unit_p_queue);

				// iterate temp_unit_p_queue to process enqueued units
				while (temp_unit_p_queue.size())
				{
					TK <F_nsl_translation_unit> unit_p = temp_unit_p_queue.back();

					processed_unit_p_set.insert(unit_p.object_p());
					sorted_unit_p_vector_.push_back(unit_p);

					temp_unit_p_queue.pop();

					// enqueue dependencies
					for (auto dependency_p : unit_p->dependency_p_vector())
					{
						if (processed_unit_p_set.find(dependency_p.object_p()) == processed_unit_p_set.end())
							unit_p_queue.push(dependency_p.no_requirements());
					}
				}
			}
		}

		// Reverse sorted_unit_p_vector_
		{
			ptrd low_index = 0;
			ptrd height_index = ptrd(sorted_unit_p_vector_.size()) - 1;
			while(
				height_index
				> low_index
			) {
				eastl::swap(
					sorted_unit_p_vector_[low_index],
					sorted_unit_p_vector_[height_index]
				);
				++low_index;
				--height_index;
			}
		}

		return true;
	}
	b8 F_nsl_translation_unit_compiler::setup_shaders() {

		auto shader_manager_p = shader_compiler_p_->shader_manager_p();

		auto& name_to_shader_object_p_map = shader_manager_p->name_to_shader_object_p_map();

		u32 index = 0;
		for(auto& [_, shader_object_p] : name_to_shader_object_p_map) {

			shader_object_p->index = index;
			++index;
		}

		compiled_result_.shader_count = name_to_shader_object_p_map.size();

		return true;
	}
	b8 F_nsl_translation_unit_compiler::setup_sampler_state_actual_slots() {

		auto shader_manager_p = shader_compiler_p_->shader_manager_p();
		auto sampler_state_manager_p = shader_compiler_p_->sampler_state_manager_p();

		auto& name_to_shader_object_p_map = shader_manager_p->name_to_shader_object_p_map();
		auto& name_to_sampler_state_info_map = sampler_state_manager_p->name_to_sampler_state_info_map();

		u32 shader_count = name_to_shader_object_p_map.size();

		for(auto& sampler_state : name_to_sampler_state_info_map) {

			sampler_state.second.actual_slots.resize(shader_count);
			for(auto& actual_slot : sampler_state.second.actual_slots)
				actual_slot = -1;
		}

		using F_sampler_state_iterator = std::remove_reference_t<decltype(name_to_sampler_state_info_map)>::iterator;
		TG_unordered_map<G_string, TG_vector<F_sampler_state_iterator>> shader_name_to_sampler_state_iterators_map;

		// bind sampler states to shaders
		for(auto& [shader_name, shader_object_p] : name_to_shader_object_p_map) {

			shader_name_to_sampler_state_iterators_map[shader_name] = TG_vector<F_sampler_state_iterator>();
		}
		for(auto it = name_to_sampler_state_info_map.begin(); it != name_to_sampler_state_info_map.end(); ++it) {

			auto& sampler_state = *it;

			b8 is_all_shader = (sampler_state.second.shader_filters.find("*") != sampler_state.second.shader_filters.end());

			if(is_all_shader) {

				for(auto& [shader_name, sampler_state_iterators] : shader_name_to_sampler_state_iterators_map) {

					sampler_state_iterators.push_back(it);
				}
			}
			else {
				for(const auto& shader_name : sampler_state.second.shader_filters) {

					shader_name_to_sampler_state_iterators_map[shader_name].push_back(it);
				}
			}
		}

		// bind actual slots in shaders
		for(auto& [shader_name, sampler_state_iterators] : shader_name_to_sampler_state_iterators_map) {

			if(name_to_shader_object_p_map.find(shader_name) == name_to_shader_object_p_map.end()) {

				for(auto resource_it : sampler_state_iterators)
					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(resource_it->second.translation_unit_p->error_group_p()->stack()),
						resource_it->second.begin_location,
						"not found shader \"" + shader_name + "\""
					);
				return false;
			}

			auto shader_object_p = name_to_shader_object_p_map[shader_name];
			u32 shader_index = shader_object_p->index;

			u32 sampler_state_iterator_size = sampler_state_iterators.size();

			// sort sampler state iterators (to let manual slot sampler state iterators are on correct slot)
			for(u32 i = 0; i < sampler_state_iterator_size; ++i) {

				auto& sampler_state_it = sampler_state_iterators[i];
				auto& sampler_state = *sampler_state_it;

				if(
					(sampler_state.second.slot != -1)
					&& (sampler_state.second.slot != i)
				) {
					if(sampler_state.second.slot >= sampler_state_iterator_size) {

						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							&(sampler_state.second.translation_unit_p->error_group_p()->stack()),
							sampler_state.second.begin_location,
							"slot \"" + G_to_string(sampler_state.second.slot) + "\" out of bound"
						);
						return false;
					}

					auto& current_on_slot_sampler_state_it = sampler_state_iterators[sampler_state.second.slot];
					auto& current_on_slot_sampler_state = *current_on_slot_sampler_state_it;

					if(current_on_slot_sampler_state.second.slot == sampler_state.second.slot) {

						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							&(sampler_state.second.translation_unit_p->error_group_p()->stack()),
							sampler_state.second.begin_location,
							"sampler states \"" + sampler_state.first + "\" and \"" + current_on_slot_sampler_state.first + "\" have the same slot"
						);
						return false;
					}

					std::swap(current_on_slot_sampler_state_it, sampler_state_it);
				}
			}

			// bind actual slots
			for(u32 i = 0; i < sampler_state_iterator_size; ++i) {

				auto& sampler_state_it = sampler_state_iterators[i];
				auto& sampler_state = *sampler_state_it;

				sampler_state.second.actual_slots[shader_index] = i;
			}
		}

		return true;
	}
	b8 F_nsl_translation_unit_compiler::setup_srv_resource_actual_slots() {

		auto shader_manager_p = shader_compiler_p_->shader_manager_p();
		auto resource_manager_p = shader_compiler_p_->resource_manager_p();

		auto& name_to_shader_object_p_map = shader_manager_p->name_to_shader_object_p_map();
		auto& name_to_resource_info_map = resource_manager_p->name_to_resource_info_map();

		u32 shader_count = name_to_shader_object_p_map.size();

		for(auto& resource : name_to_resource_info_map) {

			if(resource.second.type_class != E_nsl_resource_type_class::SRV)
				continue;

			resource.second.actual_slots.resize(shader_count);
			for(auto& actual_slot : resource.second.actual_slots)
				actual_slot = -1;
		}

		using F_resource_iterator = std::remove_reference_t<decltype(name_to_resource_info_map)>::iterator;
		TG_unordered_map<G_string, TG_vector<F_resource_iterator>> shader_name_to_resource_iterators_map;

		// bind resources to shaders
		for(auto& [shader_name, shader_object_p] : name_to_shader_object_p_map) {

			shader_name_to_resource_iterators_map[shader_name] = TG_vector<F_resource_iterator>();
		}
		for(auto it = name_to_resource_info_map.begin(); it != name_to_resource_info_map.end(); ++it) {

			auto& resource = *it;

			if(resource.second.type_class != E_nsl_resource_type_class::SRV)
				continue;

			b8 is_all_shader = (resource.second.shader_filters.find("*") != resource.second.shader_filters.end());

			if(is_all_shader) {

				for(auto& [shader_name, resource_iterators] : shader_name_to_resource_iterators_map) {

					resource_iterators.push_back(it);
				}
			}
			else {
				for(const auto& shader_name : resource.second.shader_filters) {

					shader_name_to_resource_iterators_map[shader_name].push_back(it);
				}
			}
		}

		// bind actual slots in shaders
		for(auto& [shader_name, resource_iterators] : shader_name_to_resource_iterators_map) {

			if(name_to_shader_object_p_map.find(shader_name) == name_to_shader_object_p_map.end()) {

				for(auto resource_it : resource_iterators)
					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(resource_it->second.translation_unit_p->error_group_p()->stack()),
						resource_it->second.begin_location,
						"not found shader \"" + shader_name + "\""
					);
				return false;
			}

			auto shader_object_p = name_to_shader_object_p_map[shader_name];
			u32 shader_index = shader_object_p->index;

			u32 resource_iterator_size = resource_iterators.size();

			// sort resource iterators (to let manual slot resource iterators are on correct slot)
			for(u32 i = 0; i < resource_iterator_size; ++i) {

				auto& resource_it = resource_iterators[i];
				auto& resource = *resource_it;

				if(
					(resource.second.slot != -1)
					&& (resource.second.slot != i)
				) {
					if(resource.second.slot >= resource_iterator_size) {

						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							&(resource.second.translation_unit_p->error_group_p()->stack()),
							resource.second.begin_location,
							"slot \"" + G_to_string(resource.second.slot) + "\" out of bound"
						);
						return false;
					}

					auto& current_on_slot_resource_it = resource_iterators[resource.second.slot];
					auto& current_on_slot_resource = *current_on_slot_resource_it;

					if(current_on_slot_resource.second.slot == resource.second.slot) {

						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							&(resource.second.translation_unit_p->error_group_p()->stack()),
							resource.second.begin_location,
							"resources \"" + resource.first + "\" and \"" + current_on_slot_resource.first + "\" have the same slot"
						);
						return false;
					}

					std::swap(current_on_slot_resource_it, resource_it);
				}
			}

			// bind actual slots
			for(u32 i = 0; i < resource_iterator_size; ++i) {

				auto& resource_it = resource_iterators[i];
				auto& resource = *resource_it;

				resource.second.actual_slots[shader_index] = i;
			}
		}

		return true;
	}
	b8 F_nsl_translation_unit_compiler::setup_uav_resource_actual_slots() {

		auto shader_manager_p = shader_compiler_p_->shader_manager_p();
		auto resource_manager_p = shader_compiler_p_->resource_manager_p();

		auto& name_to_shader_object_p_map = shader_manager_p->name_to_shader_object_p_map();
		auto& name_to_resource_info_map = resource_manager_p->name_to_resource_info_map();

		u32 shader_count = name_to_shader_object_p_map.size();

		for(auto& resource : name_to_resource_info_map) {

			if(resource.second.type_class != E_nsl_resource_type_class::UAV)
				continue;

			resource.second.actual_slots.resize(shader_count);
			for(auto& actual_slot : resource.second.actual_slots)
				actual_slot = -1;
		}

		using F_resource_iterator = std::remove_reference_t<decltype(name_to_resource_info_map)>::iterator;
		TG_unordered_map<G_string, TG_vector<F_resource_iterator>> shader_name_to_resource_iterators_map;

		// bind resources to shaders
		for(auto& [shader_name, shader_object_p] : name_to_shader_object_p_map) {

			shader_name_to_resource_iterators_map[shader_name] = TG_vector<F_resource_iterator>();
		}
		for(auto it = name_to_resource_info_map.begin(); it != name_to_resource_info_map.end(); ++it) {

			auto& resource = *it;

			if(resource.second.type_class != E_nsl_resource_type_class::UAV)
				continue;

			b8 is_all_shader = (resource.second.shader_filters.find("*") != resource.second.shader_filters.end());

			if(is_all_shader) {

				for(auto& [shader_name, resource_iterators] : shader_name_to_resource_iterators_map) {

					resource_iterators.push_back(it);
				}
			}
			else {
				for(const auto& shader_name : resource.second.shader_filters) {

					shader_name_to_resource_iterators_map[shader_name].push_back(it);
				}
			}
		}

		// bind actual slots in shaders
		for(auto& [shader_name, resource_iterators] : shader_name_to_resource_iterators_map) {

			if(name_to_shader_object_p_map.find(shader_name) == name_to_shader_object_p_map.end()) {

				for(auto resource_it : resource_iterators)
					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(resource_it->second.translation_unit_p->error_group_p()->stack()),
						resource_it->second.begin_location,
						"not found shader \"" + shader_name + "\""
					);
				return false;
			}

			auto shader_object_p = name_to_shader_object_p_map[shader_name];
			u32 shader_index = shader_object_p->index;

			u32 resource_iterator_size = resource_iterators.size();

			// sort resource iterators (to let manual slot resource iterators are on correct slot)
			for(u32 i = 0; i < resource_iterator_size; ++i) {

				auto& resource_it = resource_iterators[i];
				auto& resource = *resource_it;

				if(
					(resource.second.slot != -1)
					&& (resource.second.slot != i)
				) {
					if(resource.second.slot >= resource_iterator_size) {

						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							&(resource.second.translation_unit_p->error_group_p()->stack()),
							resource.second.begin_location,
							"slot \"" + G_to_string(resource.second.slot) + "\" out of bound"
						);
						return false;
					}

					auto& current_on_slot_resource_it = resource_iterators[resource.second.slot];
					auto& current_on_slot_resource = *current_on_slot_resource_it;

					if(current_on_slot_resource.second.slot == resource.second.slot) {

						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							&(resource.second.translation_unit_p->error_group_p()->stack()),
							resource.second.begin_location,
							"resources \"" + resource.first + "\" and \"" + current_on_slot_resource.first + "\" have the same slot"
						);
						return false;
					}

					std::swap(current_on_slot_resource_it, resource_it);
				}
			}

			// bind actual slots
			for(u32 i = 0; i < resource_iterator_size; ++i) {

				auto& resource_it = resource_iterators[i];
				auto& resource = *resource_it;

				resource.second.actual_slots[shader_index] = i;
			}
		}

		return true;
	}
	b8 F_nsl_translation_unit_compiler::setup_cbv_resource_actual_slots() {

		auto shader_manager_p = shader_compiler_p_->shader_manager_p();
		auto resource_manager_p = shader_compiler_p_->resource_manager_p();

		auto& name_to_shader_object_p_map = shader_manager_p->name_to_shader_object_p_map();
		auto& name_to_resource_info_map = resource_manager_p->name_to_resource_info_map();

		u32 shader_count = name_to_shader_object_p_map.size();

		for(auto& resource : name_to_resource_info_map) {

			if(resource.second.type_class != E_nsl_resource_type_class::CBV)
				continue;

			resource.second.actual_slots.resize(shader_count);
			for(auto& actual_slot : resource.second.actual_slots)
				actual_slot = -1;
		}

		using F_resource_iterator = std::remove_reference_t<decltype(name_to_resource_info_map)>::iterator;
		TG_unordered_map<G_string, TG_vector<F_resource_iterator>> shader_name_to_resource_iterators_map;

		// bind resources to shaders
		for(auto& [shader_name, shader_object_p] : name_to_shader_object_p_map) {

			shader_name_to_resource_iterators_map[shader_name] = TG_vector<F_resource_iterator>();
		}
		for(auto it = name_to_resource_info_map.begin(); it != name_to_resource_info_map.end(); ++it) {

			auto& resource = *it;

			if(resource.second.type_class != E_nsl_resource_type_class::CBV)
				continue;

			b8 is_all_shader = (resource.second.shader_filters.find("*") != resource.second.shader_filters.end());

			if(is_all_shader) {

				for(auto& [shader_name, resource_iterators] : shader_name_to_resource_iterators_map) {

					resource_iterators.push_back(it);
				}
			}
			else {
				for(const auto& shader_name : resource.second.shader_filters) {

					shader_name_to_resource_iterators_map[shader_name].push_back(it);
				}
			}
		}

		// bind actual slots in shaders
		for(auto& [shader_name, resource_iterators] : shader_name_to_resource_iterators_map) {

			if(name_to_shader_object_p_map.find(shader_name) == name_to_shader_object_p_map.end()) {

				for(auto resource_it : resource_iterators)
					NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
						&(resource_it->second.translation_unit_p->error_group_p()->stack()),
						resource_it->second.begin_location,
						"not found shader \"" + shader_name + "\""
					);
				return false;
			}

			auto shader_object_p = name_to_shader_object_p_map[shader_name];
			u32 shader_index = shader_object_p->index;

			u32 resource_iterator_size = resource_iterators.size();

			// sort resource iterators (to let manual slot resource iterators are on correct slot)
			for(u32 i = 0; i < resource_iterator_size; ++i) {

				auto& resource_it = resource_iterators[i];
				auto& resource = *resource_it;

				if(
					(resource.second.slot != -1)
					&& (resource.second.slot != i)
				) {
					if(resource.second.slot >= resource_iterator_size) {

						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							&(resource.second.translation_unit_p->error_group_p()->stack()),
							resource.second.begin_location,
							"slot \"" + G_to_string(resource.second.slot) + "\" out of bound"
						);
						return false;
					}

					auto& current_on_slot_resource_it = resource_iterators[resource.second.slot];
					auto& current_on_slot_resource = *current_on_slot_resource_it;

					if(current_on_slot_resource.second.slot == resource.second.slot) {

						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							&(resource.second.translation_unit_p->error_group_p()->stack()),
							resource.second.begin_location,
							"resources \"" + resource.first + "\" and \"" + current_on_slot_resource.first + "\" have the same slot"
						);
						return false;
					}

					std::swap(current_on_slot_resource_it, resource_it);
				}
			}

			// bind actual slots
			for(u32 i = 0; i < resource_iterator_size; ++i) {

				auto& resource_it = resource_iterators[i];
				auto& resource = *resource_it;

				resource.second.actual_slots[shader_index] = i;
			}
		}

		return true;
	}
	b8 F_nsl_translation_unit_compiler::bind_uniforms_to_constant_buffers_internal() {

		auto uniform_manager_p = shader_compiler_p_->uniform_manager_p();
		auto resource_manager_p = shader_compiler_p_->resource_manager_p();

		auto& name_to_uniform_info_map = uniform_manager_p->name_to_uniform_info_map();

		for(auto& uniform : name_to_uniform_info_map) {

			if(
				!(resource_manager_p->is_name_has_resource_info(uniform.second.buffer))
			) {
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					&(uniform.second.translation_unit_p->error_group_p()->stack()),
					uniform.second.begin_location,
					"not found buffer \"" + uniform.second.buffer + "\""
				);
				return false;
			}

			auto& constant_buffer = resource_manager_p->resource_info(uniform.second.buffer);
			constant_buffer.uniforms.push_back(uniform.first);
		}

		return true;
	}
	b8 F_nsl_translation_unit_compiler::setup_constant_buffers_internal() {

		auto data_type_manager_p = shader_compiler_p_->data_type_manager_p();
		auto uniform_manager_p = shader_compiler_p_->uniform_manager_p();
		auto resource_manager_p = shader_compiler_p_->resource_manager_p();

		auto& name_to_uniform_info_map = uniform_manager_p->name_to_uniform_info_map();
		auto& name_to_resource_info_map = resource_manager_p->name_to_resource_info_map();

		for(auto& resource : name_to_resource_info_map) {

			if(resource.second.type == "ConstantBuffer") {

				const auto& uniform_names = resource.second.uniforms;

				u32 uniform_count = uniform_names.size();

				// initialize uniform_iterator_vector
				using F_uniform_iterator = std::remove_reference_t<decltype(name_to_uniform_info_map)>::iterator;
				TG_vector<F_uniform_iterator> uniform_iterator_vector(uniform_count);
				for(u32 i = 0; i < uniform_count; ++i) {

					const auto& uniform_name = uniform_names[i];

					uniform_iterator_vector[i] = name_to_uniform_info_map.find(uniform_name);
				}

				// sort uniform_iterator_vector by size
				{
					auto sort_func = [&](const F_uniform_iterator& a, const F_uniform_iterator& b) -> b8 {

						return (
							data_type_manager_p->size(a->second.type)
							> data_type_manager_p->size(b->second.type)
						);
					};
					eastl::sort(uniform_iterator_vector.begin(), uniform_iterator_vector.end(), sort_func);
				}

				// calculate buffer size and uniform offset
				{
					constexpr u32 buffer_alignment = 256;
					constexpr u32 min_pack_alignment = 16;

					u32 offset = 0;

					for(u32 i = 0; i < uniform_count; ++i) {

						F_uniform_iterator& uniform_iterator = uniform_iterator_vector[i];

						u32 member_alignment = data_type_manager_p->alignment(uniform_iterator->second.type);
						u32 member_element_count = uniform_iterator->second.count;

						u32 member_single_element_size = data_type_manager_p->size(uniform_iterator->second.type);
						u32 aligned_member_single_element_size = align_size(member_single_element_size, member_alignment);

						E_nsl_element_format member_type_element_format = data_type_manager_p->element_format(uniform_iterator->second.type);
						u32 member_type_element_count = data_type_manager_p->element_count(uniform_iterator->second.type);

						u32 member_size = (
							(member_element_count - 1) * eastl::max<u32>(aligned_member_single_element_size, min_pack_alignment)
							+ (
								(
									(member_type_element_count == 3)
									&& (
										(member_type_element_format != E_nsl_element_format::FLOAT_32)
										&& (member_type_element_format != E_nsl_element_format::UINT_32)
										&& (member_type_element_format != E_nsl_element_format::SINT_32)
										&& (member_type_element_format != E_nsl_element_format::TYPELESS_32)
									)
								)
								? aligned_member_single_element_size
								: member_single_element_size
							)
						);

						offset = align_size(offset, member_alignment);

						uniform_iterator->second.offset = offset;

						offset += member_size;
					}

					resource.second.constant_size = align_size(offset, buffer_alignment);
				}
			}
		}

		return true;
	}
	b8 F_nsl_translation_unit_compiler::apply_internal() {

		// output language src header
		{
			auto src_header_opt = shader_compiler_p_->output_language_p()->src_header();

			if(!src_header_opt)
				return false;

			compiled_result_.src_content += src_header_opt.value();
		}

		for(auto& unit_p : sorted_unit_p_vector_) {

			auto compiled_result_opt_from_ast_tree = ast_trees_to_string(
				unit_p->ast_trees()
			);

			if(!compiled_result_opt_from_ast_tree)
				return false;

			compiled_result_.src_content += compiled_result_opt_from_ast_tree.value();
		}

		return true;
	}

	b8 F_nsl_translation_unit_compiler::compile_minimal() {

		if(!sort_units_internal())
			return false;
		if(!setup_shaders())
			return false;
		if(!setup_sampler_state_actual_slots())
			return false;
		if(!setup_srv_resource_actual_slots())
			return false;
		if(!setup_uav_resource_actual_slots())
			return false;
		if(!setup_cbv_resource_actual_slots())
			return false;
		if(!bind_uniforms_to_constant_buffers_internal())
			return false;
		if(!setup_constant_buffers_internal())
			return false;
		if(!apply_internal())
			return false;

		return true;
	}

	b8 F_nsl_translation_unit_compiler::prepare_unit(TK_valid<F_nsl_translation_unit> unit_p, F_nsl_context& context) {

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
		F_nsl_context context;

		main_unit_p_ = shader_compiler_p_->translation_unit_manager_p()->create_unit(
			raw_src_content,
			abs_path
		);

		compiled_result_.output_language_enum = shader_compiler_p_->output_language_p()->as_enum();

		if(!main_unit_p_)
			return false;

		if(!prepare_unit(NCPP_FOH_VALID(main_unit_p_), context))
			return false;

		return true;
	}
	eastl::optional<F_nsl_compiled_result> F_nsl_translation_unit_compiler::compile() {

		if(!compile_minimal())
			return eastl::nullopt;

		compiled_result_.reflection = shader_compiler_p_->reflector_p()->reflect();

		return compiled_result_;
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
	eastl::optional<G_string> F_nsl_translation_unit_compiler::ast_trees_to_string(const TG_vector<F_nsl_ast_tree>& ast_trees) {

		G_string compiled_result_str;

		for(const F_nsl_ast_tree& ast_tree : ast_trees) {

			switch (ast_tree.type)
			{
			case E_nsl_ast_tree_type::PLAIN_TEXT:
				compiled_result_str += ast_tree.plain_text.content;
				break;
			case E_nsl_ast_tree_type::INFO_TREE:
				break;
			case E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION:
			{
				const auto& object_implementation = ast_tree.object_implementation;

				auto ast_tree_compiled_result_str_opt = object_implementation.attached_object_p->apply(
					ast_tree
				);

				if(!ast_tree_compiled_result_str_opt)
					return eastl::nullopt;

				compiled_result_str += ast_tree_compiled_result_str_opt.value();
			}
				break;
			};
		}

		return compiled_result_str;
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

		if(result.target_binding == "")
			result.target_binding = name;

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
		register_element_count(
			name,
			semantic_info.element_count
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

			E_nsl_element_format member_type_element_format = element_format(argument.type);
			u32 member_type_element_count = element_count(argument.type);

			u32 member_size = (
				(member_element_count - 1) * eastl::max<u32>(aligned_member_single_element_size, min_pack_alignment)
				+ (
					(
						(member_type_element_count == 3)
						&& (
							(member_type_element_format != E_nsl_element_format::FLOAT_32)
							&& (member_type_element_format != E_nsl_element_format::UINT_32)
							&& (member_type_element_format != E_nsl_element_format::SINT_32)
							&& (member_type_element_format != E_nsl_element_format::TYPELESS_32)
						)
					)
					? aligned_member_single_element_size
					: member_single_element_size
				)
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
		register_type_class(
			name,
			E_nsl_type_class::STRUCTURE
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

	G_string A_nsl_output_language::register_slot_macro(const G_string& name) {

		return "NSL_REGISTER_SLOT_" + name;
	}

	b8 A_nsl_output_language::is_support(E_nsl_feature feature) {

		return true;
	}



	A_nsl_output_hlsl::A_nsl_output_hlsl(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		A_nsl_output_language(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	A_nsl_output_hlsl::~A_nsl_output_hlsl() {
	}

	eastl::optional<G_string> A_nsl_output_hlsl::src_header() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();

		G_string result;

		result += "#define NSL_HLSL\n";
		result += "#define NSL_HLSL_MAJOR " + name_manager_p->target("NSL_HLSL_MAJOR") + "\n";
		result += "#define NSL_HLSL_MINOR " + name_manager_p->target("NSL_HLSL_MINOR") + "\n";

		result += G_string("#define b8 bool\n");
		result += G_string("#define i32 int\n");
		result += G_string("#define u32 uint\n");
		result += G_string("#define f16 half\n");
		result += G_string("#define f32 float\n");
		result += G_string("#define f64 double\n");

		result += G_string("#define F_vector2_b8 bool2\n");
		result += G_string("#define F_vector2_i32 int2\n");
		result += G_string("#define F_vector2_u32 uint2\n");
		result += G_string("#define F_vector2_f16 half2\n");
		result += G_string("#define F_vector2_f32 float2\n");
		result += G_string("#define F_vector2_f64 double2\n");

		result += G_string("#define b8x2 bool2\n");
		result += G_string("#define i32x2 int2\n");
		result += G_string("#define u32x2 uint2\n");
		result += G_string("#define f16x2 half2\n");
		result += G_string("#define f32x2 float2\n");
		result += G_string("#define f64x2 double2\n");

		result += G_string("#define F_vector3_b8 bool3\n");
		result += G_string("#define F_vector3_i32 int3\n");
		result += G_string("#define F_vector3_u32 uint3\n");
		result += G_string("#define F_vector3_f16 half3\n");
		result += G_string("#define F_vector3_f32 float3\n");
		result += G_string("#define F_vector3_f64 double3\n");

		result += G_string("#define b8x3 bool3\n");
		result += G_string("#define i32x3 int3\n");
		result += G_string("#define u32x3 uint3\n");
		result += G_string("#define f16x3 half3\n");
		result += G_string("#define f32x3 float3\n");
		result += G_string("#define f64x3 double3\n");

		result += G_string("#define F_vector4_b8 bool4\n");
		result += G_string("#define F_vector4_i32 int4\n");
		result += G_string("#define F_vector4_u32 uint4\n");
		result += G_string("#define F_vector4_f16 half4\n");
		result += G_string("#define F_vector4_f32 float4\n");
		result += G_string("#define F_vector4_f64 double4\n");

		result += G_string("#define b8x4 bool4\n");
		result += G_string("#define i32x4 int4\n");
		result += G_string("#define u32x4 uint4\n");
		result += G_string("#define f16x4 half4\n");
		result += G_string("#define f32x4 float4\n");
		result += G_string("#define f64x4 double4\n");

		result += G_string("#define F_matrix2x2_b8 bool2x2\n");
		result += G_string("#define F_matrix2x2_i32 int2x2\n");
		result += G_string("#define F_matrix2x2_u32 uint2x2\n");
		result += G_string("#define F_matrix2x2_f16 half2x2\n");
		result += G_string("#define F_matrix2x2_f32 float2x2\n");
		result += G_string("#define F_matrix2x2_f64 double2x2\n");

		result += G_string("#define b8x2x2 bool2x2\n");
		result += G_string("#define i32x2x2 int2x2\n");
		result += G_string("#define u32x2x2 uint2x2\n");
		result += G_string("#define f16x2x2 half2x2\n");
		result += G_string("#define f32x2x2 float2x2\n");
		result += G_string("#define f64x2x2 double2x2\n");

		result += G_string("#define F_matrix3x3_b8 bool3x3\n");
		result += G_string("#define F_matrix3x3_i32 int3x3\n");
		result += G_string("#define F_matrix3x3_u32 uint3x3\n");
		result += G_string("#define F_matrix3x3_f16 half3x3\n");
		result += G_string("#define F_matrix3x3_f32 float3x3\n");
		result += G_string("#define F_matrix3x3_f64 double3x3\n");

		result += G_string("#define b8x3x3 bool3x3\n");
		result += G_string("#define i32x3x3 int3x3\n");
		result += G_string("#define u32x3x3 uint3x3\n");
		result += G_string("#define f16x3x3 half3x3\n");
		result += G_string("#define f32x3x3 float3x3\n");
		result += G_string("#define f64x3x3 double3x3\n");

		result += G_string("#define F_matrix4x4_b8 bool4x4\n");
		result += G_string("#define F_matrix4x4_i32 int4x4\n");
		result += G_string("#define F_matrix4x4_u32 uint4x4\n");
		result += G_string("#define F_matrix4x4_f16 half4x4\n");
		result += G_string("#define F_matrix4x4_f32 float4x4\n");
		result += G_string("#define F_matrix4x4_f64 double4x4\n");

		result += G_string("#define b8x4x4 bool4x4\n");
		result += G_string("#define i32x4x4 int4x4\n");
		result += G_string("#define u32x4x4 uint4x4\n");
		result += G_string("#define f16x4x4 half4x4\n");
		result += G_string("#define f32x4x4 float4x4\n");
		result += G_string("#define f64x4x4 double4x4\n");

		result += G_string("#define F_vector2 F_vector2_f32\n");
		result += G_string("#define F_vector3 F_vector3_f32\n");
		result += G_string("#define F_vector4 F_vector4_f32\n");
		result += G_string("#define F_matrix2x2 F_matrix2x2_f32\n");
		result += G_string("#define F_matrix3x3 F_matrix3x3_f32\n");
		result += G_string("#define F_matrix4x4 F_matrix4x4_f32\n");

		result += G_string("#define SV_POSITION SV_Position\n");
		result += G_string("#define SV_TARGET SV_Target\n");

		return std::move(result);
	}

	void A_nsl_output_hlsl::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->register_name("NSL_HLSL");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "0");
		name_manager_p->register_name("NSL_HLSL_MINOR", "0");

		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("ConstantBuffer");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("Buffer");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("ByteAddressBuffer");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("StructuredBuffer");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("Texture1D");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("Texture1DArray");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("Texture2D");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("Texture2DArray");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("Texture3D");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("TextureCube");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("TextureCubeArray");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RWBuffer");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RWByteAddressBuffer");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RWStructuredBuffer");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RWTexture1D");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RWTexture1DArray");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RWTexture2D");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RWTexture2DArray");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RWTexture3D");

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

		data_type_manager_p->register_primitive_data_type("bool", E_nsl_primitive_data_type::B8);
		data_type_manager_p->register_primitive_data_type("int", E_nsl_primitive_data_type::I32);
		data_type_manager_p->register_primitive_data_type("uint", E_nsl_primitive_data_type::U32);
		data_type_manager_p->register_primitive_data_type("half", E_nsl_primitive_data_type::F16);
		data_type_manager_p->register_primitive_data_type("float", E_nsl_primitive_data_type::F32);
		data_type_manager_p->register_primitive_data_type("double", E_nsl_primitive_data_type::F64);

		data_type_manager_p->register_type_class("bool", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("int", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("uint", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("half", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("float", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("double", E_nsl_type_class::PRIMITIVE);

		data_type_manager_p->register_element_format("bool", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double", E_nsl_element_format::FLOAT_64);

		data_type_manager_p->register_element_count("bool", 1);
		data_type_manager_p->register_element_count("int", 1);
		data_type_manager_p->register_element_count("uint", 1);
		data_type_manager_p->register_element_count("half", 1);
		data_type_manager_p->register_element_count("float", 1);
		data_type_manager_p->register_element_count("double", 1);

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

		name_manager_p->register_name("b8x2", "bool2");
		name_manager_p->register_name("i32x2", "int2");
		name_manager_p->register_name("u32x2", "uint2");
		name_manager_p->register_name("f16x2", "half2");
		name_manager_p->register_name("f32x2", "float2");
		name_manager_p->register_name("f64x2", "double2");

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

		data_type_manager_p->register_primitive_data_type("bool2", E_nsl_primitive_data_type::B8X2);
		data_type_manager_p->register_primitive_data_type("int2", E_nsl_primitive_data_type::I32X2);
		data_type_manager_p->register_primitive_data_type("uint2", E_nsl_primitive_data_type::U32X2);
		data_type_manager_p->register_primitive_data_type("half2", E_nsl_primitive_data_type::F16X2);
		data_type_manager_p->register_primitive_data_type("float2", E_nsl_primitive_data_type::F32X2);
		data_type_manager_p->register_primitive_data_type("double2", E_nsl_primitive_data_type::F64X2);

		data_type_manager_p->register_type_class("bool2", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("int2", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("uint2", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("half2", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("float2", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("double2", E_nsl_type_class::PRIMITIVE);

		data_type_manager_p->register_element_format("bool2", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int2", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint2", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half2", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float2", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double2", E_nsl_element_format::FLOAT_64);

		data_type_manager_p->register_element_count("bool2", 2);
		data_type_manager_p->register_element_count("int2", 2);
		data_type_manager_p->register_element_count("uint2", 2);
		data_type_manager_p->register_element_count("half2", 2);
		data_type_manager_p->register_element_count("float2", 2);
		data_type_manager_p->register_element_count("double2", 2);

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

		name_manager_p->register_name("b8x3", "bool3");
		name_manager_p->register_name("i32x3", "int3");
		name_manager_p->register_name("u32x3", "uint3");
		name_manager_p->register_name("f16x3", "half3");
		name_manager_p->register_name("f32x3", "float3");
		name_manager_p->register_name("f64x3", "double3");

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

		data_type_manager_p->register_primitive_data_type("bool3", E_nsl_primitive_data_type::B8X3);
		data_type_manager_p->register_primitive_data_type("int3", E_nsl_primitive_data_type::I32X3);
		data_type_manager_p->register_primitive_data_type("uint3", E_nsl_primitive_data_type::U32X3);
		data_type_manager_p->register_primitive_data_type("half3", E_nsl_primitive_data_type::F16X3);
		data_type_manager_p->register_primitive_data_type("float3", E_nsl_primitive_data_type::F32X3);
		data_type_manager_p->register_primitive_data_type("double3", E_nsl_primitive_data_type::F64X3);

		data_type_manager_p->register_type_class("bool3", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("int3", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("uint3", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("half3", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("float3", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("double3", E_nsl_type_class::PRIMITIVE);

		data_type_manager_p->register_element_format("bool3", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int3", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint3", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half3", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float3", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double3", E_nsl_element_format::FLOAT_64);

		data_type_manager_p->register_element_count("bool3", 3);
		data_type_manager_p->register_element_count("int3", 3);
		data_type_manager_p->register_element_count("uint3", 3);
		data_type_manager_p->register_element_count("half3", 3);
		data_type_manager_p->register_element_count("float3", 3);
		data_type_manager_p->register_element_count("double3", 3);

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

		name_manager_p->register_name("b8x4", "bool4");
		name_manager_p->register_name("i32x4", "int4");
		name_manager_p->register_name("u32x4", "uint4");
		name_manager_p->register_name("f16x4", "half4");
		name_manager_p->register_name("f32x4", "float4");
		name_manager_p->register_name("f64x4", "double4");

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

		data_type_manager_p->register_primitive_data_type("bool4", E_nsl_primitive_data_type::B8X4);
		data_type_manager_p->register_primitive_data_type("int4", E_nsl_primitive_data_type::I32X4);
		data_type_manager_p->register_primitive_data_type("uint4", E_nsl_primitive_data_type::U32X4);
		data_type_manager_p->register_primitive_data_type("half4", E_nsl_primitive_data_type::F16X4);
		data_type_manager_p->register_primitive_data_type("float4", E_nsl_primitive_data_type::F32X4);
		data_type_manager_p->register_primitive_data_type("double4", E_nsl_primitive_data_type::F64X4);

		data_type_manager_p->register_type_class("bool4", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("int4", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("uint4", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("half4", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("float4", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("double4", E_nsl_type_class::PRIMITIVE);

		data_type_manager_p->register_element_format("bool4", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int4", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint4", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half4", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float4", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double4", E_nsl_element_format::FLOAT_64);

		data_type_manager_p->register_element_count("bool4", 4);
		data_type_manager_p->register_element_count("int4", 4);
		data_type_manager_p->register_element_count("uint4", 4);
		data_type_manager_p->register_element_count("half4", 4);
		data_type_manager_p->register_element_count("float4", 4);
		data_type_manager_p->register_element_count("double4", 4);

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

		name_manager_p->register_name("b8x2x2", "bool2x2");
		name_manager_p->register_name("i32x2x2", "int2x2");
		name_manager_p->register_name("u32x2x2", "uint2x2");
		name_manager_p->register_name("f16x2x2", "half2x2");
		name_manager_p->register_name("f32x2x2", "float2x2");
		name_manager_p->register_name("f64x2x2", "double2x2");

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

		data_type_manager_p->register_primitive_data_type("bool2x2", E_nsl_primitive_data_type::B8X2X2);
		data_type_manager_p->register_primitive_data_type("int2x2", E_nsl_primitive_data_type::I32X2X2);
		data_type_manager_p->register_primitive_data_type("uint2x2", E_nsl_primitive_data_type::U32X2X2);
		data_type_manager_p->register_primitive_data_type("half2x2", E_nsl_primitive_data_type::F16X2X2);
		data_type_manager_p->register_primitive_data_type("float2x2", E_nsl_primitive_data_type::F32X2X2);
		data_type_manager_p->register_primitive_data_type("double2x2", E_nsl_primitive_data_type::F64X2X2);

		data_type_manager_p->register_type_class("bool2x2", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("int2x2", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("uint2x2", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("half2x2", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("float2x2", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("double2x2", E_nsl_type_class::PRIMITIVE);

		data_type_manager_p->register_element_format("bool2x2", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int2x2", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint2x2", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half2x2", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float2x2", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double2x2", E_nsl_element_format::FLOAT_64);

		data_type_manager_p->register_element_count("bool2x2", 2 * 2);
		data_type_manager_p->register_element_count("int2x2", 2 * 2);
		data_type_manager_p->register_element_count("uint2x2", 2 * 2);
		data_type_manager_p->register_element_count("half2x2", 2 * 2);
		data_type_manager_p->register_element_count("float2x2", 2 * 2);
		data_type_manager_p->register_element_count("double2x2", 2 * 2);

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

		name_manager_p->register_name("b8x3x3", "bool3x3");
		name_manager_p->register_name("i32x3x3", "int3x3");
		name_manager_p->register_name("u32x3x3", "uint3x3");
		name_manager_p->register_name("f16x3x3", "half3x3");
		name_manager_p->register_name("f32x3x3", "float3x3");
		name_manager_p->register_name("f64x3x3", "double3x3");

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

		data_type_manager_p->register_primitive_data_type("bool3x3", E_nsl_primitive_data_type::B8X3X3);
		data_type_manager_p->register_primitive_data_type("int3x3", E_nsl_primitive_data_type::I32X3X3);
		data_type_manager_p->register_primitive_data_type("uint3x3", E_nsl_primitive_data_type::U32X3X3);
		data_type_manager_p->register_primitive_data_type("half3x3", E_nsl_primitive_data_type::F16X3X3);
		data_type_manager_p->register_primitive_data_type("float3x3", E_nsl_primitive_data_type::F32X3X3);
		data_type_manager_p->register_primitive_data_type("double3x3", E_nsl_primitive_data_type::F64X3X3);

		data_type_manager_p->register_type_class("bool3x3", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("int3x3", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("uint3x3", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("half3x3", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("float3x3", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("double3x3", E_nsl_type_class::PRIMITIVE);

		data_type_manager_p->register_element_format("bool3x3", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int3x3", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint3x3", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half3x3", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float3x3", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double3x3", E_nsl_element_format::FLOAT_64);

		data_type_manager_p->register_element_count("bool3x3", 3 * 3);
		data_type_manager_p->register_element_count("int3x3", 3 * 3);
		data_type_manager_p->register_element_count("uint3x3", 3 * 3);
		data_type_manager_p->register_element_count("half3x3", 3 * 3);
		data_type_manager_p->register_element_count("float3x3", 3 * 3);
		data_type_manager_p->register_element_count("double3x3", 3 * 3);

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

		name_manager_p->register_name("b8x4x4", "bool4x4");
		name_manager_p->register_name("i32x4x4", "int4x4");
		name_manager_p->register_name("u32x4x4", "uint4x4");
		name_manager_p->register_name("f16x4x4", "half4x4");
		name_manager_p->register_name("f32x4x4", "float4x4");
		name_manager_p->register_name("f64x4x4", "double4x4");

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

		data_type_manager_p->register_primitive_data_type("bool4x4", E_nsl_primitive_data_type::B8X4X4);
		data_type_manager_p->register_primitive_data_type("int4x4", E_nsl_primitive_data_type::I32X4X4);
		data_type_manager_p->register_primitive_data_type("uint4x4", E_nsl_primitive_data_type::U32X4X4);
		data_type_manager_p->register_primitive_data_type("half4x4", E_nsl_primitive_data_type::F16X4X4);
		data_type_manager_p->register_primitive_data_type("float4x4", E_nsl_primitive_data_type::F32X4X4);
		data_type_manager_p->register_primitive_data_type("double4x4", E_nsl_primitive_data_type::F64X4X4);

		data_type_manager_p->register_type_class("bool4x4", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("int4x4", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("uint4x4", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("half4x4", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("float4x4", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_type_class("double4x4", E_nsl_type_class::PRIMITIVE);

		data_type_manager_p->register_element_format("bool4x4", E_nsl_element_format::UINT_8);
		data_type_manager_p->register_element_format("int4x4", E_nsl_element_format::SINT_32);
		data_type_manager_p->register_element_format("uint4x4", E_nsl_element_format::UINT_32);
		data_type_manager_p->register_element_format("half4x4", E_nsl_element_format::FLOAT_16);
		data_type_manager_p->register_element_format("float4x4", E_nsl_element_format::FLOAT_32);
		data_type_manager_p->register_element_format("double4x4", E_nsl_element_format::FLOAT_64);

		data_type_manager_p->register_element_count("bool4x4", 4 * 4);
		data_type_manager_p->register_element_count("int4x4", 4 * 4);
		data_type_manager_p->register_element_count("uint4x4", 4 * 4);
		data_type_manager_p->register_element_count("half4x4", 4 * 4);
		data_type_manager_p->register_element_count("float4x4", 4 * 4);
		data_type_manager_p->register_element_count("double4x4", 4 * 4);

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
			F_nsl_semantic_info("float4")
		);
		data_type_manager_p->register_semantic(
			"SV_Target",
			F_nsl_semantic_info("float4")
		);
	}

	eastl::optional<G_string> A_nsl_output_hlsl::define_to_string(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name,
		const G_string& target
	) {
		return "#define " + name + " " + G_replace_all(target, "\n", "\\\n") + "\n";
	}
	eastl::optional<G_string> A_nsl_output_hlsl::undef_to_string(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) {
		return "#undef " + name;
	}
	eastl::optional<G_string> A_nsl_output_hlsl::sampler_state_to_string(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const F_nsl_sampler_state& sampler_state
	) {
		u32 shader_count = sampler_state.second.actual_slots.size();

		G_string result;

		G_string sampler_state_register_slot_macro = register_slot_macro(sampler_state.first);

		result += (
			G_string("\n#ifdef ")
			+ sampler_state_register_slot_macro
			+ "\n"

			+ "#define NSL_REGISTER_"
			+ sampler_state.first
			+ " register(s##"
			+ sampler_state_register_slot_macro
			+ ")\n"

			+ "SamplerState "
			+ sampler_state.first
			+ " : "
			+ "NSL_REGISTER_"
			+ sampler_state.first
			+ ";\n"

			+ "#endif\n"
		);

		return std::move(result);
	}
	eastl::optional<G_string> A_nsl_output_hlsl::resource_to_string(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const F_nsl_resource& resource
	) {
		return resource_to_string_with_customization(translation_unit_p, resource);
	}
	eastl::optional<G_string> A_nsl_output_hlsl::structure_to_string(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const F_nsl_structure& structure
	) {
		G_string argument_member_declarations;

		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		for(const auto& argument_member : structure.second.argument_members) {

			G_string argument_type = argument_member.argument.type;

			b8 is_semantic = data_type_manager_p->is_name_has_semantic_info(argument_member.argument.type);

			G_string semantic_option;

			if(is_semantic) {

				const auto& semantic_info = data_type_manager_p->semantic_info(argument_type);

				semantic_option = ": " + semantic_info.target_binding;

				argument_type = semantic_info.target_type;
			}

			argument_member_declarations += (
				argument_type
				+ " "
				+ argument_member.argument.name
				+ semantic_option
				+ ";\n"
			);
		}

		return (
			"struct "
			+ structure.first
			+ " {\n"
			+ argument_member_declarations
			+ "\n};\n"
		);
	}
	eastl::optional<G_string> A_nsl_output_hlsl::enumeration_to_string(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const F_nsl_enumeration& enumeration
	) {
		G_string value_declarations;

		u32 value_count = enumeration.second.values.size();

		for(u32 i = 0; i < value_count; ++i) {

			const auto& value = enumeration.second.values[i];

			if(i != 0)
				value_declarations += "\n";

			value_declarations += (
				"#define "
				+ value.first()
				+ " "
				+ G_to_string(value.second())
			);
		}

		return (
			"#define " + enumeration.first + " " + enumeration.second.value_type
			+ "\n"
			+ value_declarations
			+ "\n"
		);
	}
	eastl::optional<G_string> A_nsl_output_hlsl::shader_object_to_string(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		TKPA_valid<A_nsl_shader_object> shader_object_p,
		const G_string& body,
		const G_string& pre_shader_keyword
	) {
		G_string data_param_declarations;

		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		b8 is_first_data_param = true;
		for(const auto& data_param : shader_object_p->data_params()) {

			G_string argument_type = data_param.argument.type;

			b8 is_semantic = data_type_manager_p->is_name_has_semantic_info(argument_type);

			G_string semantic_option;

			if(is_semantic) {

				const auto& semantic_info = data_type_manager_p->semantic_info(argument_type);

				semantic_option = ": " + semantic_info.target_binding;

				argument_type = semantic_info.target_type;
			}

			if(!is_first_data_param) {

				data_param_declarations += ",\n";
			}
			is_first_data_param = false;

			if(data_param.is_out && data_param.is_in)
				data_param_declarations += "inout ";
			if(data_param.is_out && !data_param.is_in)
				data_param_declarations += "out ";

			data_param_declarations += (
				argument_type
				+ " "
				+ data_param.argument.name
				+ semantic_option
			);
		}

		return (
			"#ifdef NSL_SHADER_" + shader_object_p->name() + "\n"
			+ pre_shader_keyword
			+ "void main(\n"
			+ data_param_declarations
			+ "\n){\n"
			+ body + "\n"
			+ "}\n"
			+ "#endif\n"
		);
	}

	eastl::optional<G_string> A_nsl_output_hlsl::resource_to_string_with_customization(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const F_nsl_resource& resource,
		const G_string& sub_name_keyword
	) {
		char register_type;
		switch (resource.second.type_class)
		{
		case E_nsl_resource_type_class::SRV:
			register_type = 't';
			break;
		case E_nsl_resource_type_class::UAV:
			register_type = 'u';
			break;
		case E_nsl_resource_type_class::CBV:
			register_type = 'b';
			break;
		case E_nsl_resource_type_class::NONE:
		default:
			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(resource.second.translation_unit_p->error_group_p()->stack()),
				resource.second.begin_location,
				"invalid resource type \"" + resource.second.type + "\""
			);
			return eastl::nullopt;
			break;
		}

		G_string parsed_type = resource.second.type;

		// if type has template args
		{
			u32 type_arg_count = resource.second.type_args.size();

			if (type_arg_count)
				parsed_type += "<";

			for (u32 i = 0; i < type_arg_count; ++i)
			{
				if(i != 0)
					parsed_type += ",";

				parsed_type += resource.second.type_args[i];
			}

			if (type_arg_count)
				parsed_type += ">";
		}

		u32 shader_count = resource.second.actual_slots.size();

		G_string result;

		G_string resource_register_slot_macro = register_slot_macro(resource.first);

		G_string uniform_declarations;
		auto uniform_manager_p = shader_compiler_p()->uniform_manager_p();
		for(const auto& uniform : resource.second.uniforms) {

			const auto& uniform_info = uniform_manager_p->uniform_info(uniform);

			uniform_declarations += uniform_info.type + " " + uniform + ";\n";
		}

		result += (
			G_string("\n#ifdef ")
			+ resource_register_slot_macro
			+ "\n"

			+ "#define NSL_REGISTER_"
			+ resource.first
			+ " register("
		  	+ G_string(1, register_type)
			+ "##"
			+ resource_register_slot_macro
			+ ")\n"
		);

		if(resource.second.type != "ConstantBuffer")
			result += (
				// resource type
				parsed_type
				+ " "

				// resource name
				+ resource.first

				// sub name keyword
				+ sub_name_keyword

				// register
				+ " : NSL_REGISTER_"
				+ resource.first
				+ ";\n"
			);
		else {
			result += (
				"cbuffer "

				// resource name
				+ resource.first

				// sub name keyword
				+ sub_name_keyword

				// register
				+ " : NSL_REGISTER_"
				+ resource.first
				+ "{\n"
				+ uniform_declarations
				+ "\n};\n"
			);
		}

		result += "#endif\n";

		return std::move(result);
	}



	F_nsl_output_hlsl_4::F_nsl_output_hlsl_4(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		A_nsl_output_hlsl(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl_4::~F_nsl_output_hlsl_4() {
	}

	void F_nsl_output_hlsl_4::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->deregister_name("NSL_HLSL_MAJOR");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "4");
	}



	F_nsl_output_hlsl_5::F_nsl_output_hlsl_5(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		F_nsl_output_hlsl_4(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl_5::~F_nsl_output_hlsl_5() {
	}

	void F_nsl_output_hlsl_5::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->deregister_name("NSL_HLSL_MAJOR");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "5");
	}



	F_nsl_output_hlsl_5_1::F_nsl_output_hlsl_5_1(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		F_nsl_output_hlsl_5(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl_5_1::~F_nsl_output_hlsl_5_1() {
	}

	void F_nsl_output_hlsl_5_1::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "1");
	}

	eastl::optional<G_string> F_nsl_output_hlsl_5_1::resource_to_string(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const F_nsl_resource& resource
	) {
		G_string sub_name_keyword;

		const auto& resource_info = resource.second;

		if(resource_info.is_array)
			for(u32 i = 0; i < resource_info.dimension_count; ++i) {

				sub_name_keyword += "[";
				sub_name_keyword += G_to_string(resource_info.dimension_sizes[i]);
				sub_name_keyword += "]";
			}

		return resource_to_string_with_customization(
			translation_unit_p,
			resource,
			sub_name_keyword
		);
	}



	F_nsl_shader_manager::F_nsl_shader_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_shader_manager::~F_nsl_shader_manager() {
	}



	F_nsl_resource_manager::F_nsl_resource_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
		// setup name_to_resource_type_class_map_
		name_to_resource_type_class_map_["ConstantBuffer"] = E_nsl_resource_type_class::CBV;
		name_to_resource_type_class_map_["Buffer"] = E_nsl_resource_type_class::SRV;
		name_to_resource_type_class_map_["ByteAddressBuffer"] = E_nsl_resource_type_class::SRV;
		name_to_resource_type_class_map_["StructuredBuffer"] = E_nsl_resource_type_class::SRV;
		name_to_resource_type_class_map_["Texture1D"] = E_nsl_resource_type_class::SRV;
		name_to_resource_type_class_map_["Texture1DArray"] = E_nsl_resource_type_class::SRV;
		name_to_resource_type_class_map_["Texture2D"] = E_nsl_resource_type_class::SRV;
		name_to_resource_type_class_map_["Texture2DArray"] = E_nsl_resource_type_class::SRV;
		name_to_resource_type_class_map_["Texture3D"] = E_nsl_resource_type_class::SRV;
		name_to_resource_type_class_map_["TextureCube"] = E_nsl_resource_type_class::SRV;
		name_to_resource_type_class_map_["TextureCubeArray"] = E_nsl_resource_type_class::SRV;
		name_to_resource_type_class_map_["RWBuffer"] = E_nsl_resource_type_class::UAV;
		name_to_resource_type_class_map_["RWByteAddressBuffer"] = E_nsl_resource_type_class::UAV;
		name_to_resource_type_class_map_["RWStructuredBuffer"] = E_nsl_resource_type_class::UAV;
		name_to_resource_type_class_map_["RWTexture1D"] = E_nsl_resource_type_class::UAV;
		name_to_resource_type_class_map_["RWTexture1DArray"] = E_nsl_resource_type_class::UAV;
		name_to_resource_type_class_map_["RWTexture2D"] = E_nsl_resource_type_class::UAV;
		name_to_resource_type_class_map_["RWTexture2DArray"] = E_nsl_resource_type_class::UAV;
		name_to_resource_type_class_map_["RWTexture3D"] = E_nsl_resource_type_class::UAV;

		// setup name_to_resource_type_map_
		name_to_resource_type_map_["ConstantBuffer"] = E_nsl_resource_type::ConstantBuffer;
		name_to_resource_type_map_["Buffer"] = E_nsl_resource_type::Buffer;
		name_to_resource_type_map_["ByteAddressBuffer"] = E_nsl_resource_type::ByteAddressBuffer;
		name_to_resource_type_map_["StructuredBuffer"] = E_nsl_resource_type::StructuredBuffer;
		name_to_resource_type_map_["Texture1D"] = E_nsl_resource_type::Texture1D;
		name_to_resource_type_map_["Texture1DArray"] = E_nsl_resource_type::Texture1DArray;
		name_to_resource_type_map_["Texture2D"] = E_nsl_resource_type::Texture2D;
		name_to_resource_type_map_["Texture2DArray"] = E_nsl_resource_type::Texture2DArray;
		name_to_resource_type_map_["Texture3D"] = E_nsl_resource_type::Texture3D;
		name_to_resource_type_map_["TextureCube"] = E_nsl_resource_type::TextureCube;
		name_to_resource_type_map_["TextureCubeArray"] = E_nsl_resource_type::TextureCubeArray;
		name_to_resource_type_map_["RWBuffer"] = E_nsl_resource_type::RWBuffer;
		name_to_resource_type_map_["RWByteAddressBuffer"] = E_nsl_resource_type::RWByteAddressBuffer;
		name_to_resource_type_map_["RWStructuredBuffer"] = E_nsl_resource_type::RWStructuredBuffer;
		name_to_resource_type_map_["RWTexture1D"] = E_nsl_resource_type::RWTexture1D;
		name_to_resource_type_map_["RWTexture1DArray"] = E_nsl_resource_type::RWTexture1DArray;
		name_to_resource_type_map_["RWTexture2D"] = E_nsl_resource_type::RWTexture2D;
		name_to_resource_type_map_["RWTexture2DArray"] = E_nsl_resource_type::RWTexture2DArray;
		name_to_resource_type_map_["RWTexture3D"] = E_nsl_resource_type::RWTexture3D;
	}
	F_nsl_resource_manager::~F_nsl_resource_manager() {
	}

	F_nsl_resource_info F_nsl_resource_manager::process_resource_info(const G_string& name, const F_nsl_resource_info& resource_info) {
		
		F_nsl_resource_info result = resource_info;

		// bind type class
		{
			auto it = name_to_resource_type_class_map_.find(result.type);

			if(it != name_to_resource_type_class_map_.end()) {

				result.type_class = it->second;
			}
		}

		// bind type as enum
		{
			result.type_as_enum = name_to_resource_type_map_[result.type];
		}

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



	F_nsl_pipeline_state_manager::F_nsl_pipeline_state_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_pipeline_state_manager::~F_nsl_pipeline_state_manager() {
	}

	F_nsl_pipeline_state_info F_nsl_pipeline_state_manager::process_pipeline_state_info(const G_string& name, const F_nsl_pipeline_state_info& pipeline_state_info) {
		
		F_nsl_pipeline_state_info result = pipeline_state_info;

		auto shader_manager_p = shader_compiler_p_->shader_manager_p();

		u32 shader_count = result.shaders.size();

		TG_vector<TK<A_nsl_shader_object>> shader_object_p_vector;
		shader_object_p_vector.reserve(shader_count);

		// setup shader object p vector
		for(const G_string& shader_name : result.shaders) {

			shader_object_p_vector.push_back(
				shader_manager_p->name_to_shader_object_p_map().find(shader_name)->second
			);
		}

		// check pipeline state type
		for(const auto& shader_object_p : shader_object_p_vector) {

			if(shader_object_p->type() == ED_shader_type::COMPUTE) {

				result.type = ED_pipeline_state_type::COMPUTE;
				break;
			}
			if(shader_object_p->type() == ED_shader_type::VERTEX) {

				result.type = ED_pipeline_state_type::GRAPHICS;
				break;
			}
		}

		return std::move(result);
	}



	F_nsl_input_assembler_manager::F_nsl_input_assembler_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_input_assembler_manager::~F_nsl_input_assembler_manager() {
	}

	F_nsl_input_assembler_info F_nsl_input_assembler_manager::process_input_assembler_info(const G_string& name, const F_nsl_input_assembler_info& input_assembler_info) {

		F_nsl_input_assembler_info result = input_assembler_info;

		auto data_type_manager_p = shader_compiler_p_->data_type_manager_p();

		// build desc
		{
			u32 attribute_count = result.attributes.size();

			F_input_assembler_desc& desc = result.desc;

			for(u32 i = 0; i < attribute_count; ++i) {

				auto& attribute = result.attributes[i];

				const auto& semantic_info = data_type_manager_p->semantic_info(attribute.semantic);

				ED_format format = ED_format::NONE;
				u32 duplicate_count = 0;

				switch (semantic_info.element_format)
				{
				case E_nsl_element_format::FLOAT_64:
					{
						static ED_format formats[] = {
							ED_format::R32G32B32A32_FLOAT,
							ED_format::R32G32_FLOAT
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								2
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 8.0f
							)
						);
					}
					break;
				case E_nsl_element_format::UINT_64:
					{
						static ED_format formats[] = {
							ED_format::R32G32B32A32_UINT,
							ED_format::R32G32_UINT
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								2
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 8.0f
							)
						);
					}
					break;
				case E_nsl_element_format::SINT_64:
					{
						static ED_format formats[] = {
							ED_format::R32G32B32A32_SINT,
							ED_format::R32G32_SINT
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								2
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 8.0f
							)
						);
					}
					break;
				case E_nsl_element_format::TYPELESS_64:
					{
						static ED_format formats[] = {
							ED_format::R32G32B32A32_TYPELESS,
							ED_format::R32G32_TYPELESS
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								2
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 8.0f
							)
						);
					}
					break;

				case E_nsl_element_format::FLOAT_32:
					{
						static ED_format formats[] = {
							ED_format::R32G32B32A32_FLOAT,
							ED_format::R32G32B32_FLOAT,
							ED_format::R32G32_FLOAT,
							ED_format::R32_FLOAT
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;
				case E_nsl_element_format::FLOAT_16:
					{
						static ED_format formats[] = {
							ED_format::R16G16B16A16_FLOAT,
							ED_format::R16G16B16A16_FLOAT,
							ED_format::R16G16_FLOAT,
							ED_format::R16_FLOAT
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;

				case E_nsl_element_format::UNORM_16:
					{
						static ED_format formats[] = {
							ED_format::R16G16B16A16_UNORM,
							ED_format::R16G16B16A16_UNORM,
							ED_format::R16G16_UNORM,
							ED_format::R16_UNORM
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;
				case E_nsl_element_format::UNORM_8:
					{
						static ED_format formats[] = {
							ED_format::R8G8B8A8_UNORM,
							ED_format::R8G8B8A8_UNORM,
							ED_format::R8G8_UNORM,
							ED_format::R8_UNORM
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;

				case E_nsl_element_format::SNORM_16:
					{
						static ED_format formats[] = {
							ED_format::R16G16B16A16_SNORM,
							ED_format::R16G16B16A16_SNORM,
							ED_format::R16G16_SNORM,
							ED_format::R16_SNORM
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;
				case E_nsl_element_format::SNORM_8:
					{
						static ED_format formats[] = {
							ED_format::R8G8B8A8_SNORM,
							ED_format::R8G8B8A8_SNORM,
							ED_format::R8G8_SNORM,
							ED_format::R8_SNORM
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;

				case E_nsl_element_format::UINT_32:
					{
						static ED_format formats[] = {
							ED_format::R32G32B32A32_UINT,
							ED_format::R32G32B32_UINT,
							ED_format::R32G32_UINT,
							ED_format::R32_UINT
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;
				case E_nsl_element_format::UINT_16:
					{
						static ED_format formats[] = {
							ED_format::R16G16B16A16_UINT,
							ED_format::R16G16B16A16_UINT,
							ED_format::R16G16_UINT,
							ED_format::R16_UINT
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;
				case E_nsl_element_format::UINT_8:
					{
						static ED_format formats[] = {
							ED_format::R8G8B8A8_UINT,
							ED_format::R8G8B8A8_UINT,
							ED_format::R8G8_UINT,
							ED_format::R8_UINT
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;

				case E_nsl_element_format::SINT_32:
					{
						static ED_format formats[] = {
							ED_format::R32G32B32A32_SINT,
							ED_format::R32G32B32_SINT,
							ED_format::R32G32_SINT,
							ED_format::R32_SINT
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;
				case E_nsl_element_format::SINT_16:
					{
						static ED_format formats[] = {
							ED_format::R16G16B16A16_SINT,
							ED_format::R16G16B16A16_SINT,
							ED_format::R16G16_SINT,
							ED_format::R16_SINT
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;
				case E_nsl_element_format::SINT_8:
					{
						static ED_format formats[] = {
							ED_format::R8G8B8A8_SINT,
							ED_format::R8G8B8A8_SINT,
							ED_format::R8G8_SINT,
							ED_format::R8_SINT
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;

				case E_nsl_element_format::TYPELESS_32:
					{
						static ED_format formats[] = {
							ED_format::R32G32B32A32_TYPELESS,
							ED_format::R32G32B32_TYPELESS,
							ED_format::R32G32_TYPELESS,
							ED_format::R32_TYPELESS
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;
				case E_nsl_element_format::TYPELESS_16:
					{
						static ED_format formats[] = {
							ED_format::R16G16B16A16_TYPELESS,
							ED_format::R16G16B16A16_TYPELESS,
							ED_format::R16G16_TYPELESS,
							ED_format::R16_TYPELESS
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;
				case E_nsl_element_format::TYPELESS_8:
					{
						static ED_format formats[] = {
							ED_format::R8G8B8A8_TYPELESS,
							ED_format::R8G8B8A8_TYPELESS,
							ED_format::R8G8_TYPELESS,
							ED_format::R8_TYPELESS
						};
						format = formats[
							4 - eastl::min<u32>(
								semantic_info.element_count,
								4
							)
						];
						duplicate_count = u32(
							ceil(
								f32(semantic_info.element_count)
								/ 4.0f
							)
						);
					}
					break;
				}

				switch (semantic_info.input_class)
				{
				case E_nsl_semantic_input_class::PER_VERTEX:
					desc.vertex_attribute_groups.resize(
						eastl::max<u32>(
							desc.vertex_attribute_groups.size(),
							attribute.buffer + 1
						)
					);
					desc.vertex_attribute_groups[attribute.buffer].push_back(
						F_vertex_attribute {

							.name = attribute.semantic,
							.format = format,
							.duplicate_count = duplicate_count,
							.offset = attribute.offset

						}
					);
					break;
				case E_nsl_semantic_input_class::PER_INSTANCE:
					desc.instance_attribute_groups.resize(
						eastl::max<u32>(
							desc.instance_attribute_groups.size(),
							attribute.buffer + 1
						)
					);
					desc.instance_attribute_groups[attribute.buffer].push_back(
						F_instance_attribute {

							.name = attribute.semantic,
							.format = format,
							.duplicate_count = duplicate_count,
							.offset = attribute.offset

						}
					);
					break;
				}

			}
		}

		return std::move(result);
	}



	F_nsl_reflector::F_nsl_reflector(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_reflector::~F_nsl_reflector() {
	}

	F_nsl_reflection F_nsl_reflector::reflect() {

		F_nsl_reflection reflection;

		auto shader_manager_p = shader_compiler_p_->shader_manager_p();
		auto pipeline_state_manager_p = shader_compiler_p_->pipeline_state_manager_p();
		auto data_type_manager_p = shader_compiler_p_->data_type_manager_p();
		auto sampler_state_manager_p = shader_compiler_p_->sampler_state_manager_p();
		auto resource_manager_p = shader_compiler_p_->resource_manager_p();
		auto uniform_manager_p = shader_compiler_p_->uniform_manager_p();
		auto input_assembler_manager_p = shader_compiler_p_->input_assembler_manager_p();

		auto& name_to_primitive_data_type_map = data_type_manager_p->name_to_primitive_data_type_map();
		auto& name_to_structure_info_map = data_type_manager_p->name_to_structure_info_map();
		auto& name_to_shader_object_p_map = shader_manager_p->name_to_shader_object_p_map();
		auto& name_to_pipeline_state_info_map = pipeline_state_manager_p->name_to_pipeline_state_info_map();
		auto& name_to_sampler_state_info_map = sampler_state_manager_p->name_to_sampler_state_info_map();
		auto& name_to_resource_info_map = resource_manager_p->name_to_resource_info_map();
		auto& name_to_input_assembler_info_map = input_assembler_manager_p->name_to_input_assembler_info_map();

		TG_unordered_map<G_string, u32> name_to_type_index_map;
		TG_unordered_map<G_string, u32> name_to_input_assembler_index_map;

		// input assemblers
		{
			u32 input_assembler_count = name_to_input_assembler_info_map.size();

			auto it = name_to_input_assembler_info_map.begin();

			for(u32 i = 0; i < input_assembler_count; ++i) {

				auto& input_assembler_info = it->second;

				name_to_input_assembler_index_map[it->first] = i;

				reflection.input_assemblers.push_back(
					F_nsl_input_assembler_reflection {

						.name = it->first,
						.desc = input_assembler_info.desc

					}
				);

				++it;
			}
		}

		// primitive data types
		{
			u32 primitive_data_type_count = name_to_primitive_data_type_map.size();

			auto it = name_to_primitive_data_type_map.begin();

			for(u32 i = 0; i < primitive_data_type_count; ++i) {

				auto primitive_data_type = it->second;

				name_to_type_index_map[it->first] = reflection.types.size();

				reflection.types.push_back(
					F_nsl_type_reflection {

						.name = it->first,
						.type_class = E_nsl_type_class::PRIMITIVE,
						.primitive_data_type = primitive_data_type,

						.size = data_type_manager_p->size(it->first),
						.alignment = data_type_manager_p->alignment(it->first)

					}
				);

				++it;
			}
		}

		// structures
		{
			u32 structure_count = name_to_structure_info_map.size();

			auto it = name_to_structure_info_map.begin();

			// push structures
			for(u32 i = 0; i < structure_count; ++i) {

				auto& structure_info = it->second;

				name_to_type_index_map[it->first] = reflection.types.size();

				reflection.types.push_back(
					F_nsl_type_reflection {

						.name = it->first,
						.type_class = E_nsl_type_class::STRUCTURE,

						.size = data_type_manager_p->size(it->first),
						.alignment = data_type_manager_p->alignment(it->first)

					}
				);

				++it;
			}
		}

		// structure data args
		{
			u32 structure_count = name_to_structure_info_map.size();

			auto it = name_to_structure_info_map.begin();

			for(u32 i = 0; i < structure_count; ++i) {

				auto& structure_info = it->second;

				u32 type_index = name_to_type_index_map[it->first];

				auto& structure_type = reflection.types[type_index];
				auto& structure = structure_type.structure;

				u32 data_arg_count = structure_info.argument_members.size();
				for(u32 j = 0; j < data_arg_count; ++j) {

					auto& argument_member = structure_info.argument_members[j];
					auto& argument = argument_member.argument;

					G_string actual_type = argument.type;

					if(data_type_manager_p->is_name_has_semantic_info(actual_type)) {

						const auto& semantic_info = data_type_manager_p->semantic_info(actual_type);
						actual_type = semantic_info.target_type;
					}
					if(data_type_manager_p->is_name_has_enumeration_info(actual_type)) {

						const auto& enumeration_info = data_type_manager_p->enumeration_info(actual_type);
						actual_type = enumeration_info.value_type;
					}

					structure.data_arguments.push_back(
						F_nsl_data_argument_reflection {

							.name = argument.name,
							.type_index = name_to_type_index_map[
								actual_type
							],
							.count = argument.count,
							.is_array = argument.is_array,
							.offset = argument_member.offset

						}
					);
				}

				++it;
			}
		}

		// shaders
		{
			u32 shader_count = name_to_shader_object_p_map.size();

			reflection.shaders.resize(shader_count);

			auto it = name_to_shader_object_p_map.begin();

			for(u32 i = 0; i < shader_count; ++i) {

				auto& shader_object_p = it->second;

				u32 input_assembler_index = -1;

				TK<F_nsl_vertex_shader_object> vertex_shader_object_p;
				if(shader_object_p.T_try_interface<F_nsl_vertex_shader_object>(vertex_shader_object_p)) {

					input_assembler_index = name_to_input_assembler_index_map[
						vertex_shader_object_p->input_assembler_name()
					];
				}

				reflection.shaders[i] = F_nsl_shader_reflection {

					.name = shader_object_p->name(),
					.type = shader_object_p->type(),
					.input_assembler_index = input_assembler_index

				};

				++it;
			}
		}

		// pipeline states
		{
			u32 pipeline_state_count = name_to_pipeline_state_info_map.size();

			reflection.pipeline_states.resize(pipeline_state_count);

			auto it = name_to_pipeline_state_info_map.begin();

			for(u32 i = 0; i < pipeline_state_count; ++i) {

				auto& pipeline_state_info = it->second;

				TG_vector<u32> shader_indices;
				shader_indices.reserve(pipeline_state_info.shaders.size());
				for(const auto& shader_name : pipeline_state_info.shaders) {

					shader_indices.push_back(
						name_to_shader_object_p_map[shader_name]->index
					);
				}

				reflection.pipeline_states[i] = F_nsl_pipeline_state_reflection {

					.name = it->first,
					.type = pipeline_state_info.type,
					.options = pipeline_state_info.options,
					.shader_indices = shader_indices

				};

				++it;
			}
		}

		// sampler states
		{
			u32 sampler_state_count = name_to_sampler_state_info_map.size();

			reflection.sampler_states.resize(sampler_state_count);

			auto it = name_to_sampler_state_info_map.begin();

			for(u32 i = 0; i < sampler_state_count; ++i) {

				auto& sampler_state_info = it->second;

				reflection.sampler_states[i] = F_nsl_sampler_state_reflection {

					.name = it->first,
					.desc = sampler_state_info.desc,
					.actual_slots = sampler_state_info.actual_slots

				};

				++it;
			}
		}

		// resources
		{
			u32 resource_count = name_to_resource_info_map.size();

			reflection.resources.resize(resource_count);

			auto it = name_to_resource_info_map.begin();

			for(u32 i = 0; i < resource_count; ++i) {

				auto& resource_info = it->second;

				// build data args
				u32 data_arg_count = resource_info.uniforms.size();
				TG_vector<F_nsl_data_argument_reflection> data_arguments;
				data_arguments.reserve(data_arg_count);
				for(u32 j = 0; j < data_arg_count; ++j)
				{
					const G_string& uniform_name = resource_info.uniforms[j];

					auto& uniform_info = uniform_manager_p->uniform_info(uniform_name);

					G_string actual_type = uniform_info.type;

					if (data_type_manager_p->is_name_has_semantic_info(actual_type))
					{
						const auto& semantic_info = data_type_manager_p->semantic_info(actual_type);
						actual_type = semantic_info.target_type;
					}
					if (data_type_manager_p->is_name_has_enumeration_info(actual_type))
					{
						const auto& enumeration_info = data_type_manager_p->enumeration_info(actual_type);
						actual_type = enumeration_info.value_type;
					}

					data_arguments.push_back(
						F_nsl_data_argument_reflection{

							.name = uniform_name,
							.type_index = name_to_type_index_map[
								actual_type
							],
							.count = uniform_info.count,
							.is_array = uniform_info.is_array,
							.offset = uniform_info.offset

						}
					);
				}

				// store resource
				reflection.resources[i] = F_nsl_resource_reflection {

					.name = it->first,
					.type = resource_info.type_as_enum,
					.type_class = resource_info.type_class,
					.type_args = resource_info.type_args,
					.dimension_sizes = resource_info.dimension_sizes,
					.dimension_count = resource_info.dimension_count,
					.is_array = resource_info.is_array,
					.actual_slots = resource_info.actual_slots,
					.data_arguments = std::move(data_arguments),
					.constant_size = resource_info.constant_size

				};

				++it;
			}
		}

		return std::move(reflection);
	}



	F_nsl_shader_compiler::F_nsl_shader_compiler(
		const F_nsl_shader_compiler_customizer& customizer
	) :
		shader_module_manager_p_(customizer.shader_module_manager_creator(NCPP_KTHIS())),
		translation_unit_manager_p_(customizer.translation_unit_manager_creator(NCPP_KTHIS())),
		translation_unit_compiler_p_(customizer.translation_unit_compiler_creator(NCPP_KTHIS())),
		error_storage_p_(customizer.error_storage_creator(NCPP_KTHIS())),
		object_manager_p_(customizer.object_manager_creator(NCPP_KTHIS())),
		name_manager_p_(customizer.name_manager_creator(NCPP_KTHIS())),
		data_type_manager_p_(customizer.data_type_manager_creator(NCPP_KTHIS())),
		shader_manager_p_(customizer.shader_manager_creator(NCPP_KTHIS())),
		resource_manager_p_(customizer.resource_manager_creator(NCPP_KTHIS())),
		uniform_manager_p_(customizer.uniform_manager_creator(NCPP_KTHIS())),
		sampler_state_manager_p_(customizer.sampler_state_manager_creator(NCPP_KTHIS())),
		pipeline_state_manager_p_(customizer.pipeline_state_manager_creator(NCPP_KTHIS())),
		input_assembler_manager_p_(customizer.input_assembler_manager_creator(NCPP_KTHIS())),
		reflector_p_(customizer.reflector_creator(NCPP_KTHIS()))
	{
	}
	F_nsl_shader_compiler::~F_nsl_shader_compiler() {
	}

	TU<A_nsl_output_language> F_nsl_shader_compiler::create_output_language(E_nsl_output_language output_language_enum) {

		switch (output_language_enum)
		{
		case E_nsl_output_language::HLSL_4:
			return TU<F_nsl_output_hlsl_4>()(
				NCPP_KTHIS()
			);
		case E_nsl_output_language::HLSL_5:
			return TU<F_nsl_output_hlsl_5>()(
				NCPP_KTHIS()
			);
		case E_nsl_output_language::HLSL_5_1:
			return TU<F_nsl_output_hlsl_5_1>()(
				NCPP_KTHIS()
			);
		default:
			return null;
		}
	}

	eastl::optional<F_nsl_compiled_result> F_nsl_shader_compiler::compile(
		const G_string& raw_src_content,
		E_nsl_output_language output_language_enum,
		const G_string& abs_path
	) {
		if(output_language_enum == E_nsl_output_language::NONE) {

#ifdef NRHI_DRIVER_DIRECTX_12
			if(driver_index() == NRHI_DRIVER_INDEX_DIRECTX_12)
				output_language_enum = E_nsl_output_language::HLSL_5_1;
#endif
#ifdef NRHI_DRIVER_DIRECTX_11
			if(driver_index() == NRHI_DRIVER_INDEX_DIRECTX_11)
				output_language_enum = E_nsl_output_language::HLSL_5;
#endif
#ifdef NRHI_DRIVER_VULKAN
			#error "Vulkan is not supported"
//			if(driver_index() == NRHI_DRIVER_INDEX_VULKAN)
//				output_language_enum = E_nsl_output_language::HLSL_5;
#endif
#ifdef NRHI_DRIVER_METAL
			#error "Metal is not supported"
//			if(driver_index() == NRHI_DRIVER_INDEX_METAL)
//				output_language_enum = E_nsl_output_language::HLSL_5;
#endif
#ifdef NRHI_DRIVER_OPENGL
			#error "OpenGL is not supported"
//			if(driver_index() == NRHI_DRIVER_INDEXOPENGL)
//				output_language_enum = E_nsl_output_language::HLSL_5;
#endif
		}

		output_language_p_ = create_output_language(output_language_enum);

		if(
			!(
				translation_unit_compiler_p_->prepare_units(
					raw_src_content,
					abs_path
				)
			)
		)
		{
			is_compiled_ = true;
			is_compile_success_ = false;
			return eastl::nullopt;
		}

		auto compile_result_opt = translation_unit_compiler_p_->compile();

		is_compiled_ = true;
		is_compile_success_ = static_cast<b8>(compile_result_opt);

		return compile_result_opt;
	}

}