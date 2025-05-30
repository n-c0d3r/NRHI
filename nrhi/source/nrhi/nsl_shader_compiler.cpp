#include <nrhi/nsl_shader_compiler.hpp>
#include <nrhi/shader_compiler.hpp>



namespace nrhi
{
	G_string F_nsl_compiled_result::build(u32 shader_index) const
	{
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

				if(actual_slot == -1)
					continue;

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

		// override sampler state actual slot spaces
		{
			u32 sample_state_count = reflection.sampler_states.size();

			for (u32 i = 0; i < sample_state_count; ++i)
			{
				const auto& sampler_state = reflection.sampler_states[i];

				u32 actual_slot_space = sampler_state.actual_slot_spaces[shader_index];

				if(actual_slot_space == -1)
					continue;

				G_string register_slot_space_macro = (
					"NSL_REGISTER_SLOT_SPACE_"
					+ sampler_state.name
				);

				result += (
					"#define "
					+ register_slot_space_macro
					+ " "
					+ G_to_string(actual_slot_space)
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

		// override resource actual slot spaces
		{
			u32 sample_state_count = reflection.resources.size();

			for (u32 i = 0; i < sample_state_count; ++i)
			{
				const auto& resource = reflection.resources[i];

				u32 actual_slot_space = resource.actual_slot_spaces[shader_index];

				if(actual_slot_space == -1)
					continue;

				G_string register_slot_space_macro = (
					"NSL_REGISTER_SLOT_SPACE_"
					+ resource.name
				);

				result += (
					"#define "
					+ register_slot_space_macro
					+ " "
					+ G_to_string(actual_slot_space)
					+ "\n"
				);
			}
		}

		result += src_content;

		return result;
	}
	G_string F_nsl_compiled_result::build_library() const
	{
		G_string result;

		auto& shader_reflections = reflection.shaders;

		u32 shader_count = reflection.shaders.size();
		if(!shader_count)
			return {};

		// define shaders
		for(u32 shader_index = 0; shader_index < shader_count; ++shader_index)
		{
			NCPP_ASSERT(shader_index < shader_count)
				<< "shader index out of bound";

			result += (
				"#define NSL_SHADER_"
				+ shader_reflections[shader_index].name
				+ "\n"
			);
		}

		// override sampler state actual slots
		{
			u32 sample_state_count = reflection.sampler_states.size();

			for (u32 i = 0; i < sample_state_count; ++i)
			{
				const auto& sampler_state = reflection.sampler_states[i];

				u32 actual_slot = sampler_state.actual_slots[0];

#ifdef NCPP_ENABLE_ASSERT
				for(u32 shader_index = 1; shader_index < shader_count; ++shader_index)
				{
					NCPP_ASSERT(sampler_state.actual_slots[shader_index] == actual_slot) << "invalid slots in shader library";
				}
#endif

				if(actual_slot == -1)
					continue;

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

		// override sampler state actual slot spaces
		{
			u32 sample_state_count = reflection.sampler_states.size();

			for (u32 i = 0; i < sample_state_count; ++i)
			{
				const auto& sampler_state = reflection.sampler_states[i];

				u32 actual_slot_space = sampler_state.actual_slot_spaces[0];

#ifdef NCPP_ENABLE_ASSERT
				for(u32 shader_index = 1; shader_index < shader_count; ++shader_index)
				{
					NCPP_ASSERT(sampler_state.actual_slot_spaces[shader_index] == actual_slot_space) << "invalid slot spaces in shader library";
				}
#endif

				if(actual_slot_space == -1)
					continue;

				G_string register_slot_space_macro = (
					"NSL_REGISTER_SLOT_SPACE_"
					+ sampler_state.name
				);

				result += (
					"#define "
					+ register_slot_space_macro
					+ " "
					+ G_to_string(actual_slot_space)
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

				u32 actual_slot = resource.actual_slots[0];

#ifdef NCPP_ENABLE_ASSERT
				for(u32 shader_index = 1; shader_index < shader_count; ++shader_index)
				{
					NCPP_ASSERT(resource.actual_slots[shader_index] == actual_slot) << "invalid slots in shader library";
				}
#endif

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

		// override resource actual slot spaces
		{
			u32 sample_state_count = reflection.resources.size();

			for (u32 i = 0; i < sample_state_count; ++i)
			{
				const auto& resource = reflection.resources[i];

				u32 actual_slot_space = resource.actual_slot_spaces[0];

#ifdef NCPP_ENABLE_ASSERT
				for(u32 shader_index = 1; shader_index < shader_count; ++shader_index)
				{
					NCPP_ASSERT(resource.actual_slot_spaces[shader_index] == actual_slot_space) << "invalid slot spaces in shader library";
				}
#endif

				if(actual_slot_space == -1)
					continue;

				G_string register_slot_space_macro = (
					"NSL_REGISTER_SLOT_SPACE_"
					+ resource.name
				);

				result += (
					"#define "
					+ register_slot_space_macro
					+ " "
					+ G_to_string(actual_slot_space)
					+ "\n"
				);
			}
		}

		result += src_content;

		return result;
	}
	void F_nsl_compiled_result::finalize() {

		const auto& shader_reflections = reflection.shaders;
		u32 shader_count = shader_reflections.size();

		shader_binaries.resize(shader_count);

		for(u32 i = 0; i < shader_count; ++i) {

			shader_binaries[i] = H_shader_compiler::compile_nsl(
				*this,
				i
			);
		}
	}
	void F_nsl_compiled_result::finalize_and_release_src_content() {

		finalize();

		src_content.clear();
	}
	void F_nsl_compiled_result::finalize_library()
	{
		library_binary = H_shader_compiler::compile_nsl_library(*this);
	}
	void F_nsl_compiled_result::finalize_library_and_release_src_content()
	{
		finalize_library();

		src_content.clear();
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

		for(ptrdiff_t i = 0; i < src_length; ++i)
		{
			if((i + 1) < src_length)
			{
				if(
					(src_content[i] == '/')
					&& (src_content[i + 1] == '/')
				)
				{
					i += 2;
					for(; i < src_length; ++i)
					{
						if(src_content[i] == '\n')
						{
							break;
						}
					}
				}

				if(
					(src_content[i] == '/')
					&& (src_content[i + 1] == '*')
				)
				{
					i += 2;
					b8 is_end = false;
					for(; (i + 1) < src_length; ++i)
					{
						if(
							(src_content[i] == '*')
							&& (src_content[i + 1] == '/')
						)
						{
							i += 2;
							break;
						}
					}
					if(!is_end)
					{
						if(error_stack_p)
							error_stack_p->push({
								"multi-line comment is not closed"
								-1,
								src_length,
							});

						return eastl::nullopt;
					}
				}
			}

			if(i < src_length)
			{
				result.push_back(src_content[i]);
				raw_locations.push_back(i);
			}
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
	TG_map<G_string, ED_input_classification> F_nsl_info_tree_reader::input_classification_str_to_value_map_;
	TG_map<G_string, ED_filter> F_nsl_info_tree_reader::filter_str_to_value_map_;
	TG_map<G_string, ED_texcoord_address_mode> F_nsl_info_tree_reader::texcoord_address_mode_str_to_value_map_;
	TG_map<G_string, ED_cull_mode> F_nsl_info_tree_reader::cull_mode_str_to_value_map_;
	TG_map<G_string, ED_fill_mode> F_nsl_info_tree_reader::fill_mode_str_to_value_map_;
	TG_map<G_string, ED_format> F_nsl_info_tree_reader::format_str_to_value_map_;
	TG_map<G_string, ED_comparison_func> F_nsl_info_tree_reader::comparison_func_str_to_value_map_;
	TG_map<G_string, ED_primitive_topology> F_nsl_info_tree_reader::primitive_topology_str_to_value_map_;
	TG_map<G_string, ED_shader_visibility> F_nsl_info_tree_reader::shader_visibility_str_to_value_map_;
#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
	TG_map<G_string, ED_state_object_flag> F_nsl_info_tree_reader::state_object_flag_str_to_value_map_;
#endif
#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
	TG_map<G_string, ED_work_graph_flag> F_nsl_info_tree_reader::work_graph_flag_str_to_value_map_;
	TG_map<G_string, E_nsl_node_launch> F_nsl_info_tree_reader::node_launch_str_to_value_map_;
#endif
	TG_map<G_string, ED_blend_factor> F_nsl_info_tree_reader::blend_factor_str_to_value_map_;
	TG_map<G_string, ED_blend_operation> F_nsl_info_tree_reader::blend_operation_str_to_value_map_;
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	TG_map<G_string, ED_logic_operation> F_nsl_info_tree_reader::logic_operation_str_to_value_map_;
#endif

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

			// setup input_classification_str_to_value_map_
			input_classification_str_to_value_map_["PER_VERTEX_DATA"] = ED_input_classification::PER_VERTEX_DATA;
			input_classification_str_to_value_map_["PER_INSTANCE_DATA"] = ED_input_classification::PER_INSTANCE_DATA;

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

			// setup comparison_func_str_to_value_map_
			comparison_func_str_to_value_map_["NEVER"] = ED_comparison_func::NEVER;
			comparison_func_str_to_value_map_["LESS"] = ED_comparison_func::LESS;
			comparison_func_str_to_value_map_["EQUAL"] = ED_comparison_func::EQUAL;
			comparison_func_str_to_value_map_["LESS_EQUAL"] = ED_comparison_func::LESS_EQUAL;
			comparison_func_str_to_value_map_["GREATER"] = ED_comparison_func::GREATER;
			comparison_func_str_to_value_map_["NOT_EQUAL"] = ED_comparison_func::NOT_EQUAL;
			comparison_func_str_to_value_map_["GREATER_EQUAL"] = ED_comparison_func::GREATER_EQUAL;
			comparison_func_str_to_value_map_["ALWAYS"] = ED_comparison_func::ALWAYS;

			// setup primitive_topology_str_to_value_map_
			primitive_topology_str_to_value_map_["NONE"] = ED_primitive_topology::NONE;
			primitive_topology_str_to_value_map_["LINE_LIST"] = ED_primitive_topology::LINE_LIST;
			primitive_topology_str_to_value_map_["TRIANGLE_LIST"] = ED_primitive_topology::TRIANGLE_LIST;

			// setup shader_visibility_str_to_value_map_
			shader_visibility_str_to_value_map_["ALL"] = ED_shader_visibility::ALL;
			shader_visibility_str_to_value_map_["VERTEX"] = ED_shader_visibility::VERTEX;
			shader_visibility_str_to_value_map_["TESSELLATION_CONTROL"] = ED_shader_visibility::TESSELLATION_CONTROL;
			shader_visibility_str_to_value_map_["TESSELLATION_EVALUATION"] = ED_shader_visibility::TESSELLATION_EVALUATION;
			shader_visibility_str_to_value_map_["GEOMETRY"] = ED_shader_visibility::GEOMETRY;
			shader_visibility_str_to_value_map_["PIXEL"] = ED_shader_visibility::PIXEL;
			shader_visibility_str_to_value_map_["AMPLIFICATION"] = ED_shader_visibility::AMPLIFICATION;
			shader_visibility_str_to_value_map_["MESH"] = ED_shader_visibility::MESH;

#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
			// setup state_object_flag_str_to_value_map_
			state_object_flag_str_to_value_map_["NONE"] = ED_state_object_flag::NONE;
			state_object_flag_str_to_value_map_["ALLOW_STATE_OBJECT_ADDITIONS"] = ED_state_object_flag::ALLOW_STATE_OBJECT_ADDITIONS;
			state_object_flag_str_to_value_map_["ALLOW_EXTERNAL_DEPENDENCIES_ON_LOCAL_DEFINITIONS"] = ED_state_object_flag::ALLOW_EXTERNAL_DEPENDENCIES_ON_LOCAL_DEFINITIONS;
			state_object_flag_str_to_value_map_["ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS"] = ED_state_object_flag::ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS;
#endif

#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
			// setup work_graph_flag_str_to_value_map_
			work_graph_flag_str_to_value_map_["NONE"] = ED_work_graph_flag::NONE;
			work_graph_flag_str_to_value_map_["INCLUDE_ALL_AVAILABLE_NODES"] = ED_work_graph_flag::INCLUDE_ALL_AVAILABLE_NODES;

			// setup node_launch_str_to_value_map_
			node_launch_str_to_value_map_["BROADCASTING"] = E_nsl_node_launch::BROADCASTING;
			node_launch_str_to_value_map_["COALESCING"] = E_nsl_node_launch::COALESCING;
			node_launch_str_to_value_map_["THREAD"] = E_nsl_node_launch::THREAD;
#endif

			// setup blend_factor_str_to_value_map_
			blend_factor_str_to_value_map_["ZERO"] = ED_blend_factor::ZERO;
			blend_factor_str_to_value_map_["ONE"] = ED_blend_factor::ONE;
			blend_factor_str_to_value_map_["SRC_COLOR"] = ED_blend_factor::SRC_COLOR;
			blend_factor_str_to_value_map_["INV_SRC_COLOR"] = ED_blend_factor::INV_SRC_COLOR;
			blend_factor_str_to_value_map_["SRC_ALPHA"] = ED_blend_factor::SRC_ALPHA;
			blend_factor_str_to_value_map_["INV_SRC_ALPHA"] = ED_blend_factor::INV_SRC_ALPHA;
			blend_factor_str_to_value_map_["DEST_ALPHA"] = ED_blend_factor::DEST_ALPHA;
			blend_factor_str_to_value_map_["INV_DEST_ALPHA"] = ED_blend_factor::INV_DEST_ALPHA;
			blend_factor_str_to_value_map_["DEST_COLOR"] = ED_blend_factor::DEST_COLOR;
			blend_factor_str_to_value_map_["INV_DEST_COLOR"] = ED_blend_factor::INV_DEST_COLOR;
			blend_factor_str_to_value_map_["SRC_ALPHA_SAT"] = ED_blend_factor::SRC_ALPHA_SAT;
			blend_factor_str_to_value_map_["BLEND_FACTOR"] = ED_blend_factor::BLEND_FACTOR;
			blend_factor_str_to_value_map_["INV_BLEND_FACTOR"] = ED_blend_factor::INV_BLEND_FACTOR;
			blend_factor_str_to_value_map_["SRC1_COLOR"] = ED_blend_factor::SRC1_COLOR;
			blend_factor_str_to_value_map_["INV_SRC1_COLOR"] = ED_blend_factor::INV_SRC1_COLOR;
			blend_factor_str_to_value_map_["SRC1_ALPHA"] = ED_blend_factor::SRC1_ALPHA;
			blend_factor_str_to_value_map_["INV_SRC1_ALPHA"] = ED_blend_factor::INV_SRC1_ALPHA;
			blend_factor_str_to_value_map_["ALPHA_FACTOR"] = ED_blend_factor::ALPHA_FACTOR;
			blend_factor_str_to_value_map_["INV_ALPHA_FACTOR"] = ED_blend_factor::INV_ALPHA_FACTOR;

			// setup blend_operation_str_to_value_map_
			blend_operation_str_to_value_map_["ADD"] = ED_blend_operation::ADD;
			blend_operation_str_to_value_map_["SUBTRACT"] = ED_blend_operation::SUBTRACT;
			blend_operation_str_to_value_map_["REV_SUBTRACT"] = ED_blend_operation::REV_SUBTRACT;
			blend_operation_str_to_value_map_["MIN"] = ED_blend_operation::MIN;
			blend_operation_str_to_value_map_["MAX"] = ED_blend_operation::MAX;

			// setup logic_operation_str_to_value_map_
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
			logic_operation_str_to_value_map_["CLEAR"] = ED_logic_operation::CLEAR;
			logic_operation_str_to_value_map_["SET"] = ED_logic_operation::SET;
			logic_operation_str_to_value_map_["COPY"] = ED_logic_operation::COPY;
			logic_operation_str_to_value_map_["COPY_INVERTED"] = ED_logic_operation::COPY_INVERTED;
			logic_operation_str_to_value_map_["NOOP"] = ED_logic_operation::NOOP;
			logic_operation_str_to_value_map_["INVERT"] = ED_logic_operation::INVERT;
			logic_operation_str_to_value_map_["AND"] = ED_logic_operation::AND;
			logic_operation_str_to_value_map_["NAND"] = ED_logic_operation::NAND;
			logic_operation_str_to_value_map_["OR"] = ED_logic_operation::OR;
			logic_operation_str_to_value_map_["NOR"] = ED_logic_operation::NOR;
			logic_operation_str_to_value_map_["XOR"] = ED_logic_operation::XOR;
			logic_operation_str_to_value_map_["EQUIV"] = ED_logic_operation::EQUIV;
			logic_operation_str_to_value_map_["AND_REVERSE"] = ED_logic_operation::AND_REVERSE;
			logic_operation_str_to_value_map_["AND_INVERTED"] = ED_logic_operation::AND_INVERTED;
			logic_operation_str_to_value_map_["OR_REVERSE"] = ED_logic_operation::OR_REVERSE;
			logic_operation_str_to_value_map_["OR_INVERTED"] = ED_logic_operation::OR_INVERTED;
#endif
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
	eastl::optional<ED_input_classification> F_nsl_info_tree_reader::read_input_classification(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = input_classification_str_to_value_map_.find(value_str);

		if (it == input_classification_str_to_value_map_.end()) {

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
	eastl::optional<ED_comparison_func> F_nsl_info_tree_reader::read_comparison_func(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = comparison_func_str_to_value_map_.find(value_str);

		if (it == comparison_func_str_to_value_map_.end()) {

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
	eastl::optional<E_nsl_output_topology> F_nsl_info_tree_reader::read_output_topology(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		if(value_str == "triangle")
		{
			return E_nsl_output_topology::TRIANGLE;
		}
		if(value_str == "line")
		{
			return E_nsl_output_topology::LINE;
		}

		if(is_required)
		{
			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				error_stack_p_,
				info_trees_[index].begin_location,
				"invalid value \"" + value_str + "\""
			);
		}

		return eastl::nullopt;
	}
	eastl::optional<ED_shader_visibility> F_nsl_info_tree_reader::read_shader_visibility(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = shader_visibility_str_to_value_map_.find(value_str);

		if (it == shader_visibility_str_to_value_map_.end()) {

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
	eastl::optional<F_nsl_data_type_selection> F_nsl_info_tree_reader::read_data_type_selection(u32 index, b8 is_required) const
	{
		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		const auto& info_tree = info_trees_[index];

		G_string value_str = parse_value_str(info_tree.name);

		F_nsl_info_tree_reader child_reader(
			NCPP_FOH_VALID(shader_compiler_p_),
			info_tree.childs,
			info_tree.begin_childs_location,
			error_stack_p_
		);

		TG_vector<F_nsl_data_type_selection> childs(info_tree.childs.size());
		for(u32 i = 0; i < info_tree.childs.size(); ++i)
		{
			auto child_opt = child_reader.read_data_type_selection(i);
			if(!child_opt)
				return child_opt;

			childs[i] = eastl::move(child_opt.value());
		}

		return F_nsl_data_type_selection {
			.name = value_str,
			.childs = eastl::move(childs)
		};
	}
	eastl::optional<ED_blend_factor> F_nsl_info_tree_reader::read_blend_factor(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = blend_factor_str_to_value_map_.find(value_str);

		if (it == blend_factor_str_to_value_map_.end()) {

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
	eastl::optional<ED_blend_operation> F_nsl_info_tree_reader::read_blend_operation(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = blend_operation_str_to_value_map_.find(value_str);

		if (it == blend_operation_str_to_value_map_.end()) {

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
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	eastl::optional<ED_logic_operation> F_nsl_info_tree_reader::read_logic_operation(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = logic_operation_str_to_value_map_.find(value_str);

		if (it == logic_operation_str_to_value_map_.end()) {

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
#endif
	eastl::optional<ED_color_write_mode> F_nsl_info_tree_reader::read_color_write_mode(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		if(value_str == "RED")
		{
			return ED_color_write_mode::RED;
		}
		if(value_str == "GREEN")
		{
			return ED_color_write_mode::GREEN;
		}
		if(value_str == "BLUE")
		{
			return ED_color_write_mode::BLUE;
		}
		if(value_str == "ALPHA")
		{
			return ED_color_write_mode::ALPHA;
		}
		if(value_str == "ALL")
		{
			return ED_color_write_mode::ALL;
		}

		return eastl::nullopt;
	}
#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
	eastl::optional<ED_state_object_flag> F_nsl_info_tree_reader::read_state_object_flag(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = state_object_flag_str_to_value_map_.find(value_str);

		if (it == state_object_flag_str_to_value_map_.end()) {

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
#endif
#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
	eastl::optional<ED_work_graph_flag> F_nsl_info_tree_reader::read_work_graph_flag(u32 index, b8 is_required) const {

		if(!guarantee_index(index, is_required)) {

			return eastl::nullopt;
		}

		G_string value_str = parse_value_str(info_trees_[index].name);

		auto it = work_graph_flag_str_to_value_map_.find(value_str);

		if (it == work_graph_flag_str_to_value_map_.end()) {

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
#endif
	b8 F_nsl_info_tree_reader::read_configurable_elements(
		const eastl::function<
			b8(
				const F_nsl_info_tree& element_info_tree,
				const F_nsl_info_tree_reader& element_info_tree_reader,
				TG_unordered_map<G_string, F_nsl_info_tree_reader>& config_map
			)
		>& callback
	) const
	{
		TG_unordered_map<G_string, F_nsl_info_tree_reader> config_map;

		for(auto& info_tree : info_trees_)
		{
			if(info_tree.name[0] == '@')
			{
				config_map[
					info_tree.name.substr(1, info_tree.name.length() - 1)
				] = F_nsl_info_tree_reader(
					NCPP_FOH_VALID(shader_compiler_p_),
					info_tree.childs,
					info_tree.begin_childs_location,
					error_stack_p_
				);
				continue;
			}

			F_nsl_info_tree_reader element_info_tree_reader(
				NCPP_FOH_VALID(shader_compiler_p_),
				info_tree.childs,
				info_tree.begin_childs_location,
				error_stack_p_
			);

			if(!callback(info_tree, element_info_tree_reader, config_map))
			{
				return false;
			}

			config_map = {};
		}

		return true;
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

		if(it != system_source_map_.end())
		{
			out_src_content = it->second;

			return F_load_src_content_result{
				.abs_path = path
			};
		}

		return eastl::nullopt;
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

			if(load_result_opt)
			{
				const auto& load_result = std::move(load_result_opt.value());
				const auto& abs_path = load_result.abs_path;

				it = abs_path_to_translation_unit_p_.find(abs_path);

				if(it == abs_path_to_translation_unit_p_.end())
				{
					auto unit_p = shader_compiler_p_->translation_unit_manager_p()->create_unit(
						src_content,
						abs_path
					).no_requirements();

					abs_path_to_translation_unit_p_[abs_path] = unit_p;

					return unit_p;
				}
				else return it->second;
			}
		}
		else return it->second;

		NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
			&(from_unit_p->error_group_p()->stack()),
			tree.begin_location,
			"can't load shader module at path \"" + path + "\""
		);
		return null;
	}
	TK<F_nsl_translation_unit> F_nsl_shader_module_manager::make_virtual(
		const G_string& abs_path,
		const G_string& src_content,
		TKPA_valid<F_nsl_translation_unit> from_unit_p,
		const F_nsl_ast_tree& tree
	)
	{
		auto it = abs_path_to_translation_unit_p_.find(abs_path);

		if(it == abs_path_to_translation_unit_p_.end())
		{
			auto unit_p = shader_compiler_p_->translation_unit_manager_p()->create_unit(
				src_content,
				abs_path
			).no_requirements();

			abs_path_to_translation_unit_p_[abs_path] = unit_p;

			return unit_p;
		}

		NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
			&(from_unit_p->error_group_p()->stack()),
			tree.begin_location,
			"virtual shader module at absolute path \"" + abs_path + "\" was already created"
		);
		return null;
	}

	void F_nsl_shader_module_manager::register_translation_unit(const G_string& abs_path, TKPA_valid<F_nsl_translation_unit> translation_unit_p)
	{
		NCPP_ASSERT(abs_path_to_translation_unit_p_.find(abs_path) == abs_path_to_translation_unit_p_.end());
		abs_path_to_translation_unit_p_[abs_path] = translation_unit_p.no_requirements();
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

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto shader_module_manager_p = shader_compiler_p()->shader_module_manager_p();
		auto submodule_manager_p = shader_compiler_p()->submodule_manager_p();
		auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();

		if(name_manager_p->is_name_registered(path))
		{
			G_string target = name_manager_p->target(path);

			if(
				name_manager_p->name_type(target)
				!= T_type_hash_code<FE_nsl_name_types::SUBMODULE>
			)
			{
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					&(unit_p->error_group_p()->stack()),
					object_implementation.bodies[0].begin_location,
					"\"" + path + "\" is not a submodule"
				);
				return eastl::nullopt;
			}

			auto submodule_object_p = submodule_manager_p->submodule_object_p(target);

			imported_unit_p_ = shader_module_manager_p->make_virtual(
				submodule_object_p->translation_unit_p()->abs_path()
				+ "::"
				+ submodule_object_p->name()
				+ " {"
				+ G_to_string(submodule_object_p->next_id++)
				+ "}",
				submodule_object_p->content(),
				submodule_object_p->translation_unit_p(),
				submodule_object_p->ast_tree()
			);

			if(!imported_unit_p_)
				return eastl::nullopt;
		}
		else
		{
			imported_unit_p_ = shader_module_manager_p->load(
				unit_p,
				path,
				tree
			);

			if(!imported_unit_p_)
				return eastl::nullopt;
		}

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
	eastl::optional<G_string> F_nsl_import_object::apply(
		const F_nsl_ast_tree& tree
	)
	{
		return shader_compiler_p()->translation_unit_compiler_p()->compile_unit(
			NCPP_FOH_VALID(imported_unit_p_)
		);
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



	F_nsl_submodule_object::F_nsl_submodule_object(
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
	F_nsl_submodule_object::~F_nsl_submodule_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_submodule_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];

		auto& object_implementation = tree.object_implementation;

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto shader_module_manager_p = shader_compiler_p()->shader_module_manager_p();
		auto submodule_manager_p = shader_compiler_p()->submodule_manager_p();
		auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();

		if(name_manager_p->is_name_registered(object_implementation.name))
		{
			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				object_implementation.begin_name_location,
				"\"" + object_implementation.name + "\" was already registered"
			);
			return eastl::nullopt;
		}

		name_manager_p->T_register_name<FE_nsl_name_types::SUBMODULE>(object_implementation.name);

		content_ = object_implementation.bodies[0].content;
		ast_tree_ = tree;

		submodule_manager_p->register_submodule(
			object_implementation.name,
			NCPP_KTHIS()
		);

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_submodule_object_type::F_nsl_submodule_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"submodule",
			true,
			1,
			1
		)
	{
	}
	F_nsl_submodule_object_type::~F_nsl_submodule_object_type() {
	}

	TK<A_nsl_object> F_nsl_submodule_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_submodule_object>()(
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
					"can't parse require body"
				);
				return eastl::nullopt;
			}

			auto childs = std::move(childs_opt.value());

			return std::move(childs);
		}

		return TG_vector<F_nsl_ast_tree>();
	}
	eastl::optional<G_string> F_nsl_require_object::apply(const F_nsl_ast_tree& tree)
	{
		return shader_compiler_p()->translation_unit_compiler_p()->ast_trees_to_string(
			tree.childs
		);
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
					"can't parse require body"
				);
				return eastl::nullopt;
			}

			auto childs = std::move(childs_opt.value());

			return std::move(childs);
		}

		return TG_vector<F_nsl_ast_tree>();
	}
	eastl::optional<G_string> F_nsl_otherwise_object::apply(const F_nsl_ast_tree& tree)
	{
		return shader_compiler_p()->translation_unit_compiler_p()->ast_trees_to_string(
			tree.childs
		);
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
					"can't parse annotation info trees"
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
				.element_count = element_count
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
				"can't parse struct arguments"
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

			auto type_selection_opt = argument_child_info_tree_reader.read_data_type_selection(0);
			if(!type_selection_opt)
				return eastl::nullopt;

			structure_info.argument_members.push_back(
				F_nsl_data_argument_member {
					.argument = F_nsl_data_argument {
						.name = argument_child_info_tree.name,
						.type_selection = type_selection_opt.value(),
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
				"can't parse enum values"
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
				"can't parse resource type"
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

		if(name_manager_p->is_name_registered(type_child_info_tree.name))
		{
			if(
				name_manager_p->name_type(
					name_manager_p->target(type_child_info_tree.name)
				)
				!= T_type_hash_code<FE_nsl_name_types::RESOURCE_TYPE>
			)
			{
				NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
					&(unit_p->error_group_p()->stack()),
					object_implementation.bodies[0].begin_location,
					"\"" + type_child_info_tree.name + "\" is not a resource type"
				);
				return eastl::nullopt;
			}
		}
		else
		{
			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				object_implementation.bodies[0].begin_location,
				"unknown resource type \"" + type_child_info_tree.name + "\""
			);
			return eastl::nullopt;
		}

		resource_info.type = name_manager_p->target(type_child_info_tree.name);
		resource_info.type_args = type_args;

		// check for dimension sizes and dimension count
		{
			u32 dimension_count = child_info_trees.size() - 1;

			for(u32 i = 1; i <= dimension_count; ++i) {

				auto& dimension_size_child_info_tree = child_info_trees[i];

				auto value_opt = child_info_trees_reader.read_i32(i);

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

				auto value_opt = info_tree_reader.read_i32(0);

				if(!value_opt)
					return eastl::nullopt;

				resource_info.slot = value_opt.value();

				if(info_tree_reader.info_trees().size() > 1)
				{
					auto value2_opt = info_tree_reader.read_u32(1);

					if(!value2_opt)
						return eastl::nullopt;

					resource_info.slot_space = value2_opt.value();
				}
			}
		}

		// check for slot_space annotation
		{
			auto it = context.current_object_config.find("slot_space");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_u32(0);

				if(!value_opt)
					return eastl::nullopt;

				resource_info.slot_space = value_opt.value();
			}
		}

		// check for sort_uniforms annotation
		{
			auto it = context.current_object_config.find("sort_uniforms");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_b8(0);

				if(!value_opt)
					return eastl::nullopt;

				resource_info.sort_uniforms = value_opt.value();
			}
		}

		// check for globally_coherent annotation
		{
			auto it = context.current_object_config.find("globally_coherent");
			if(it != context.current_object_config.end()) {

				resource_info.flags = flag_combine(
					resource_info.flags,
					E_nsl_resource_flag::GLOBALLY_COHERENT
				);
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

					auto& value = value_opt.value();

					auto value_childs_opt = shaders_info_tree_reader.read_sub(value);
					if(!value_childs_opt)
						return eastl::nullopt;

					auto& value_childs = value_childs_opt.value();

					if(value_childs.info_trees().size() == 0)
					{
						resource_info.shader_filters[value] = {};
					}
					else
					{
						auto value_register_space_opt = value_childs.read_u32(0);
						if(!value_register_space_opt)
							return eastl::nullopt;

						auto& value_register_space = value_register_space_opt.value();

						resource_info.shader_filters[value] = { value_register_space };
					}
				}
			}
		}

		context.default_constant_buffer = tree.object_implementation.name;

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
				"can't parse uniform type"
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
			auto value_opt = info_tree_reader.read_data_type_selection(0);
			if(!value_opt)
				return eastl::nullopt;

			uniform_info.type_selection = value_opt.value();
		}

		// check for count
		if(info_tree_reader.info_trees().size() >= 2) {

			auto value_opt = info_tree_reader.read_u32(1);

			if(!value_opt)
				return eastl::nullopt;

			uniform_info.count = value_opt.value();
			uniform_info.is_array = true;
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

				if(info_tree_reader.info_trees().size() > 1)
				{
					auto value2_opt = info_tree_reader.read_u32(1);

					if(!value2_opt)
						return eastl::nullopt;

					sampler_state_info.slot_space = value2_opt.value();
				}
			}
		}

		// check for slot_space annotation
		{
			auto it = context.current_object_config.find("slot_space");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_u32(0);

				if(!value_opt)
					return eastl::nullopt;

				sampler_state_info.slot_space = value_opt.value();
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

					auto& value = value_opt.value();

					auto value_childs_opt = shaders_info_tree_reader.read_sub(value);
					if(!value_childs_opt)
						return eastl::nullopt;

					auto& value_childs = value_childs_opt.value();

					if(value_childs.info_trees().size() == 0)
					{
						sampler_state_info.shader_filters[value] = {};
					}
					else
					{
						auto value_register_space_opt = value_childs.read_u32(0);
						if(!value_register_space_opt)
							return eastl::nullopt;

						auto& value_register_space = value_register_space_opt.value();

						sampler_state_info.shader_filters[value] = { value_register_space };
					}
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

		// check for is_static annotation
		{
			auto it = context.current_object_config.find("is_static");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto value_opt = info_tree_reader.read_b8(0);

				if(!value_opt)
					return eastl::nullopt;

				sampler_state_info.is_static = value_opt.value();
			}
		}

		// check for is_comparison_state annotation
		auto it = context.current_object_config.find("is_comparison_state");
		if(it != context.current_object_config.end()) {

			const auto& info_tree_reader = it->second;

			auto value_opt = info_tree_reader.read_b8(0);

			if(!value_opt)
				return eastl::nullopt;

			sampler_state_info.is_comparison_state = value_opt.value();
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
				"can't parse pipeline_state shaders"
			);
			return eastl::nullopt;
		}

		auto& child_info_trees = child_info_trees_opt.value();

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

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		// check for root_signature annotation
		{
			auto it = context.current_object_config.find("root_signature");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				auto root_signature_name_value_opt = info_tree_reader.read_string(0);
				if(!root_signature_name_value_opt)
					return eastl::nullopt;

				auto& root_signature_name_value = root_signature_name_value_opt.value();

				if(name_manager_p->is_name_registered(root_signature_name_value))
				{
					G_string root_signature_name = name_manager_p->target(root_signature_name_value);

					if(name_manager_p->name_type(root_signature_name) != T_type_hash_code<FE_nsl_name_types::ROOT_SIGNATURE>)
					{
						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							&(unit_p->error_group_p()->stack()),
							object_implementation.bodies[0].begin_location,
							"\"" + info_tree_reader.info_trees()[0].name + "\" is not a root signature"
						);
						return eastl::nullopt;
					}

					pipeline_state_info.root_signature_selection = {
						.name = root_signature_name,
						.type = E_nsl_root_signature_selection_type::EMBEDDED
					};
				}
				else
				{
					pipeline_state_info.root_signature_selection = {
						.name = root_signature_name_value,
						.type = E_nsl_root_signature_selection_type::EXTERNAL
					};
				}
			}
		}
#endif

		// check for color_formats annotation
		{
			auto it = context.current_object_config.find("color_formats");
			if(it != context.current_object_config.end()) {

				const auto& info_tree_reader = it->second;

				u32 color_format_count = info_tree_reader.info_trees().size();

				pipeline_state_info.options.graphics.color_formats.resize(color_format_count);

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

				// format attribute
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

						auto value_opt = sub_info_tree_reader.read_comparison_func(0);

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

		// @input_assembler annotation
		{
			auto it = context.current_object_config.find("input_assembler");
			if(it != context.current_object_config.end()) {

				auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

				F_nsl_input_attribute_config_map attribute_config_map;

				auto& input_child_info_trees = it->second.info_trees();

				F_nsl_input_assembler_info input_assembler_info;

				u32 child_info_tree_count = input_child_info_trees.size();

				// build input assembler info
				for(u32 i = 0; i < child_info_tree_count; ++i) {

					auto& child_info_tree = input_child_info_trees[i];

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

					G_string semantic = child_info_tree.name;

					if(!(name_manager_p->is_name_registered(semantic))) {

						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							&(unit_p->error_group_p()->stack()),
							child_info_tree.begin_location,
							"not found semantic \"" + semantic + "\""
						);
					}

					semantic = name_manager_p->target(semantic);

					if(!(data_type_manager_p->is_name_has_semantic_info(semantic))) {

						NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
							&(unit_p->error_group_p()->stack()),
							child_info_tree.begin_location,
							"not found semantic \"" + semantic + "\""
						);
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

					// @classification annotation
					{
						auto it = attribute_config_map.find("classification");
						if(it != attribute_config_map.end()) {

							auto value_opt = it->second.read_input_classification(0);

							if(!value_opt)
								return eastl::nullopt;

							attribute.classification = value_opt.value();
						}
					}

					input_assembler_info.attributes.push_back(attribute);

					attribute_config_map = {};
				}

				// convert to input assembler desc and then store pipeline state
				pipeline_state_info.options.graphics.input_assembler_desc = convert_to_input_assembler_desc(
					input_assembler_info
				);
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

		// check for blend annotation
		{
			auto it = context.current_object_config.find("blend");
			if(it != context.current_object_config.end())
			{
				auto& blend_info_tree_reader = it->second;

				u32 rt_index = 0;

				auto& options = pipeline_state_info.options;

				if(
					!blend_info_tree_reader.read_configurable_elements(
						[&](
							const F_nsl_info_tree& blend_rt_info_tree,
							const F_nsl_info_tree_reader& blend_rt_info_tree_reader,
							TG_unordered_map<G_string, F_nsl_info_tree_reader>& config_map
						)
						{
							F_blend_render_target_desc blend_rt_desc;
							blend_rt_desc.enable_blend = true;

							if(config_map.find("disable_blend") != config_map.end())
							{
								blend_rt_desc.enable_blend = false;
							}
							if(config_map.find("enable_blend") != config_map.end())
							{
								blend_rt_desc.enable_blend = true;
							}
							if(config_map.find("disable_logic_operation") != config_map.end())
							{
								blend_rt_desc.enable_logic_operation = false;
							}
							if(config_map.find("enable_logic_operation") != config_map.end())
							{
								blend_rt_desc.enable_logic_operation = true;
							}

							{
								auto it = config_map.find("src_blend_factor");
								if(it != config_map.end())
								{
									auto value_opt = it->second.read_blend_factor(0);
									if(!value_opt)
									{
										return false;
									}

									blend_rt_desc.src_blend_factor = value_opt.value();
								}
							}

							{
								auto it = config_map.find("dst_blend_factor");
								if(it != config_map.end())
								{
									auto value_opt = it->second.read_blend_factor(0);
									if(!value_opt)
									{
										return false;
									}

									blend_rt_desc.dst_blend_factor = value_opt.value();
								}
							}

							{
								auto it = config_map.find("blend_operation");
								if(it != config_map.end())
								{
									auto value_opt = it->second.read_blend_operation(0);
									if(!value_opt)
									{
										return false;
									}

									blend_rt_desc.blend_operation = value_opt.value();
								}
							}

							{
								auto it = config_map.find("src_alpha_blend_factor");
								if(it != config_map.end())
								{
									auto value_opt = it->second.read_blend_factor(0);
									if(!value_opt)
									{
										return false;
									}

									blend_rt_desc.src_alpha_blend_factor = value_opt.value();
								}
							}

							{
								auto it = config_map.find("dst_alpha_blend_factor");
								if(it != config_map.end())
								{
									auto value_opt = it->second.read_blend_factor(0);
									if(!value_opt)
									{
										return false;
									}

									blend_rt_desc.dst_alpha_blend_factor = value_opt.value();
								}
							}

							{
								auto it = config_map.find("alpha_blend_operation");
								if(it != config_map.end())
								{
									auto value_opt = it->second.read_blend_operation(0);
									if(!value_opt)
									{
										return false;
									}

									blend_rt_desc.alpha_blend_operation = value_opt.value();
								}
							}

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
							{
								auto it = config_map.find("logic_operation");
								if(it != config_map.end())
								{
									auto value_opt = it->second.read_logic_operation(0);
									if(!value_opt)
									{
										return false;
									}

									blend_rt_desc.logic_operation = value_opt.value();
								}
							}
#endif

							{
								auto it = config_map.find("write_mode");
								if(it != config_map.end())
								{
									auto value_opt = it->second.read_color_write_mode(0);
									if(!value_opt)
									{
										return false;
									}

									blend_rt_desc.write_mode = value_opt.value();
								}
							}

							options.graphics.blend_desc.render_targets[rt_index] = blend_rt_desc;

							++rt_index;
							return true;
						}
					)
				)
				{
					return eastl::nullopt;
				}

				options.graphics.blend_desc.enable_independent_blend = (rt_index > 1);
			}
		}

		//
		options_ = pipeline_state_info.options;

		// register pipeline_state
		if(!is_fake_)
		{
			name_manager_p->template T_register_name<FE_nsl_name_types::PIPELINE_STATE>(tree.object_implementation.name);
			pipeline_state_manager_p->register_pipeline_state(
				tree.object_implementation.name,
				pipeline_state_info
			);
		}

		return std::move(child_trees);
	}

	F_input_assembler_desc F_nsl_pipeline_state_object::convert_to_input_assembler_desc(const F_nsl_input_assembler_info& input_assembler_info) {

		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		F_input_assembler_desc desc;

		// build desc
		{
			u32 attribute_count = input_assembler_info.attributes.size();

			for(u32 i = 0; i < attribute_count; ++i) {

				auto& attribute = input_assembler_info.attributes[i];

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

				// push back attribute
				desc.attribute_groups.resize(
					eastl::max<u32>(
						desc.attribute_groups.size(),
						attribute.buffer + 1
					)
				);
				desc.attribute_groups[attribute.buffer].push_back(
					F_input_attribute {

						.name = attribute.semantic,
						.format = format,
						.duplicate_count = duplicate_count,
						.offset = attribute.offset,
						.classification = attribute.classification

					}
				);

			}
		}

		return std::move(desc);
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



	F_nsl_default_pipeline_state_object::F_nsl_default_pipeline_state_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		F_nsl_pipeline_state_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_default_pipeline_state_object::~F_nsl_default_pipeline_state_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_default_pipeline_state_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		is_fake_ = true;

		auto result_opt = F_nsl_pipeline_state_object::recursive_build_ast_tree(
			context,
			unit_p,
			trees,
			index,
			error_stack_p
		);

		if(!result_opt)
			return result_opt;

		context.default_pipeline_state_options = options();

		return result_opt;
	}



	F_nsl_default_pipeline_state_object_type::F_nsl_default_pipeline_state_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"default_pipeline_state",
			false,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_default_pipeline_state_object_type::~F_nsl_default_pipeline_state_object_type() {
	}

	TK<A_nsl_object> F_nsl_default_pipeline_state_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_default_pipeline_state_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	F_nsl_root_signature_object::F_nsl_root_signature_object(
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
	F_nsl_root_signature_object::~F_nsl_root_signature_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_root_signature_object::recursive_build_ast_tree(
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
		auto root_signature_manager_p = shader_compiler_p()->root_signature_manager_p();
		auto sampler_state_manager_p = shader_compiler_p()->sampler_state_manager_p();

		//
		F_nsl_root_signature_info root_signature_info;
		root_signature_info.config_map = context.current_object_config;

		TG_unordered_map<G_string, F_nsl_info_tree_reader> root_param_config_map;

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
				"can't parse root params"
			);
			return eastl::nullopt;
		}

		//
		auto& child_info_trees = child_info_trees_opt.value();
		F_nsl_info_tree_reader child_info_tree_reader(
			shader_compiler_p(),
			child_info_trees,
			object_implementation.bodies[0].begin_location,
			&(unit_p->error_group_p()->stack())
		);

		//
		if(
			!child_info_tree_reader.read_configurable_elements(
				[&](
					const F_nsl_info_tree& element_info_tree,
					const F_nsl_info_tree_reader& element_info_tree_reader,
					TG_unordered_map<G_string, F_nsl_info_tree_reader>& config_map
				)
				{
					if(element_info_tree.name == "STATIC_SAMPLER")
					{
						F_static_sampler_state_desc static_sampler_state_desc;

						// parse shader visiblity
						{
							auto it = config_map.find("shader_visibility");
							if(it != config_map.end())
							{
								auto shader_visibility_opt = it->second.read_shader_visibility(0);
								if(!shader_visibility_opt)
								{
									return false;
								}

								static_sampler_state_desc.shader_visibility = shader_visibility_opt.value();
							}
						}

						// slot
						{
							auto it = config_map.find("slot");
							if(it != config_map.end())
							{
								const auto& slot_info_tree_reader = it->second;

								auto slot_value_opt = slot_info_tree_reader.read_u32(0);
								if(!slot_value_opt)
								{
									return false;
								}
								static_sampler_state_desc.shader_register = slot_value_opt.value();

								if(slot_info_tree_reader.info_trees().size() > 1)
								{
									auto slot_space_value_opt = slot_info_tree_reader.read_u32(1);
									if(!slot_space_value_opt)
									{
										return false;
									}
									static_sampler_state_desc.register_space = slot_space_value_opt.value();
								}
							}
						}

						// desc
						{
							auto name_opt = element_info_tree_reader.read_string(0);
							if(!name_opt)
							{
								return false;
							}

							auto& name = name_opt.value();

							if(!(name_manager_p->is_name_registered(name)))
							{
								return false;
							}

							if(name_manager_p->name_type(name) != T_type_hash_code<FE_nsl_name_types::SAMPLER_STATE>)
							{
								NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
									&(unit_p->error_group_p()->stack()),
									object_implementation.bodies[0].begin_location,
									"\"" + element_info_tree_reader.info_trees()[0].name + "\" is not a sampler state"
								);
								return false;
							}

							auto& sampler_state_info = sampler_state_manager_p->sampler_state_info(name);

							static_sampler_state_desc.sampler_state_desc = sampler_state_info.desc;
						}

						//
						root_signature_info.desc.static_sampler_state_descs.push_back(
							static_sampler_state_desc
						);
					}
					else
					{
						F_root_param_desc param_desc;

						// parse shader visiblity
						{
							auto it = config_map.find("shader_visibility");
							if(it != config_map.end())
							{
								auto shader_visibility_opt = it->second.read_shader_visibility(0);
								if(!shader_visibility_opt)
								{
									return false;
								}

								param_desc.shader_visibility = shader_visibility_opt.value();
							}
						}

						// parse type
						const G_string& param_type_name = element_info_tree.name;
						if(param_type_name == "DESCRIPTOR_TABLE")
						{
							param_desc.type = ED_root_param_type::DESCRIPTOR_TABLE;

							// parse ranges
							if(
								!element_info_tree_reader.read_configurable_elements(
									[&](
										const F_nsl_info_tree& range_element_info_tree,
										const F_nsl_info_tree_reader& range_element_info_tree_reader,
										TG_unordered_map<G_string, F_nsl_info_tree_reader>& range_config_map
									)
									{
										F_descriptor_range_desc range_desc;

										// parse descriptor count
										if(range_element_info_tree_reader.info_trees().size() > 0)
										{
											auto descriptor_count_opt = range_element_info_tree_reader.read_i32(0);
											if(!descriptor_count_opt)
											{
												return false;
											}
											range_desc.descriptor_count = descriptor_count_opt.value();
										}
										else
										{
											range_desc.descriptor_count = 1;
										}

										// parse offset in descriptors from table start
										if(range_element_info_tree_reader.info_trees().size() > 1)
										{
											auto offset_in_descriptors_from_table_start_opt = range_element_info_tree_reader.read_i32(1);
											if(!offset_in_descriptors_from_table_start_opt)
											{
												return false;
											}
											range_desc.offset_in_descriptors_from_table_start = offset_in_descriptors_from_table_start_opt.value();
										}

										// ranges
										const G_string& range_param_type_name = range_element_info_tree.name;
										if(range_param_type_name == "CBV")
										{
											range_desc.type = ED_descriptor_range_type::CONSTANT_BUFFER;

											// slot
											{
												auto range_it = range_config_map.find("slot");
												if(range_it != range_config_map.end())
												{
													const auto& range_slot_info_tree_reader = range_it->second;

													auto range_slot_value_opt = range_slot_info_tree_reader.read_u32(0);
													if(!range_slot_value_opt)
													{
														return false;
													}
													range_desc.base_register = range_slot_value_opt.value();

													if(range_slot_info_tree_reader.info_trees().size() > 1)
													{
														auto range_slot_space_value_opt = range_slot_info_tree_reader.read_u32(1);
														if(!range_slot_space_value_opt)
														{
															return false;
														}
														range_desc.register_space = range_slot_space_value_opt.value();
													}
												}
											}
										}
										else if(range_param_type_name == "SRV")
										{
											range_desc.type = ED_descriptor_range_type::SHADER_RESOURCE;

											// slot
											{
												auto range_it = range_config_map.find("slot");
												if(range_it != range_config_map.end())
												{
													const auto& range_slot_info_tree_reader = range_it->second;

													auto range_slot_value_opt = range_slot_info_tree_reader.read_u32(0);
													if(!range_slot_value_opt)
													{
														return false;
													}
													range_desc.base_register = range_slot_value_opt.value();

													if(range_slot_info_tree_reader.info_trees().size() > 1)
													{
														auto range_slot_space_value_opt = range_slot_info_tree_reader.read_u32(1);
														if(!range_slot_space_value_opt)
														{
															return false;
														}
														range_desc.register_space = range_slot_space_value_opt.value();
													}
												}
											}
										}
										else if(range_param_type_name == "UAV")
										{
											range_desc.type = ED_descriptor_range_type::UNORDERED_ACCESS;

											// slot
											{
												auto range_it = range_config_map.find("slot");
												if(range_it != range_config_map.end())
												{
													const auto& range_slot_info_tree_reader = range_it->second;

													auto range_slot_value_opt = range_slot_info_tree_reader.read_u32(0);
													if(!range_slot_value_opt)
													{
														return false;
													}
													range_desc.base_register = range_slot_value_opt.value();

													if(range_slot_info_tree_reader.info_trees().size() > 1)
													{
														auto range_slot_space_value_opt = range_slot_info_tree_reader.read_u32(1);
														if(!range_slot_space_value_opt)
														{
															return false;
														}
														range_desc.register_space = range_slot_space_value_opt.value();
													}
												}
											}
										}
										else if(range_param_type_name == "SAMPLER")
										{
											range_desc.type = ED_descriptor_range_type::SAMPLER;

											// slot
											{
												auto range_it = range_config_map.find("slot");
												if(range_it != range_config_map.end())
												{
													const auto& range_slot_info_tree_reader = range_it->second;

													auto range_slot_value_opt = range_slot_info_tree_reader.read_u32(0);
													if(!range_slot_value_opt)
													{
														return false;
													}
													range_desc.base_register = range_slot_value_opt.value();

													if(range_slot_info_tree_reader.info_trees().size() > 1)
													{
														auto range_slot_space_value_opt = range_slot_info_tree_reader.read_u32(1);
														if(!range_slot_space_value_opt)
														{
															return false;
														}
														range_desc.register_space = range_slot_space_value_opt.value();
													}
												}
											}
										}

										param_desc.descriptor_table_desc.range_descs.push_back(range_desc);

										return true;
									}
								)
							)
							{
								return false;
							}
						}
						else if(param_type_name == "CONSTANTS")
						{
							param_desc.type = ED_root_param_type::CONSTANTS;

							// parse count
							auto count_opt = element_info_tree_reader.read_u32(0);
							if(!count_opt)
							{
								return false;
							}
							param_desc.constants_desc.constant_count = count_opt.value();

							// slot
							{
								auto it = config_map.find("slot");
								if(it != config_map.end())
								{
									const auto& slot_info_tree_reader = it->second;

									auto slot_value_opt = slot_info_tree_reader.read_u32(0);
									if(!slot_value_opt)
									{
										return false;
									}
									param_desc.constants_desc.base_register = slot_value_opt.value();

									if(slot_info_tree_reader.info_trees().size() > 1)
									{
										auto slot_space_value_opt = slot_info_tree_reader.read_u32(1);
										if(!slot_space_value_opt)
										{
											return false;
										}
										param_desc.constants_desc.register_space = slot_space_value_opt.value();
									}
								}
							}
						}
						else if(param_type_name == "CBV")
						{
							param_desc.type = ED_root_param_type::CONSTANT_BUFFER;

							// slot
							{
								auto it = config_map.find("slot");
								if(it != config_map.end())
								{
									const auto& slot_info_tree_reader = it->second;

									auto slot_value_opt = slot_info_tree_reader.read_u32(0);
									if(!slot_value_opt)
									{
										return false;
									}
									param_desc.descriptor_desc.shader_register = slot_value_opt.value();

									if(slot_info_tree_reader.info_trees().size() > 1)
									{
										auto slot_space_value_opt = slot_info_tree_reader.read_u32(1);
										if(!slot_space_value_opt)
										{
											return false;
										}
										param_desc.descriptor_desc.register_space = slot_space_value_opt.value();
									}
								}
							}
						}
						else if(param_type_name == "SRV")
						{
							param_desc.type = ED_root_param_type::SHADER_RESOURCE;

							// slot
							{
								auto it = config_map.find("slot");
								if(it != config_map.end())
								{
									const auto& slot_info_tree_reader = it->second;

									auto slot_value_opt = slot_info_tree_reader.read_u32(0);
									if(!slot_value_opt)
									{
										return false;
									}
									param_desc.descriptor_desc.shader_register = slot_value_opt.value();

									if(slot_info_tree_reader.info_trees().size() > 1)
									{
										auto slot_space_value_opt = slot_info_tree_reader.read_u32(1);
										if(!slot_space_value_opt)
										{
											return false;
										}
										param_desc.descriptor_desc.register_space = slot_space_value_opt.value();
									}
								}
							}
						}
						else if(param_type_name == "UAV")
						{
							param_desc.type = ED_root_param_type::UNORDERED_ACCESS;

							// slot
							{
								auto it = config_map.find("slot");
								if(it != config_map.end())
								{
									const auto& slot_info_tree_reader = it->second;

									auto slot_value_opt = slot_info_tree_reader.read_u32(0);
									if(!slot_value_opt)
									{
										return false;
									}
									param_desc.descriptor_desc.shader_register = slot_value_opt.value();

									if(slot_info_tree_reader.info_trees().size() > 1)
									{
										auto slot_space_value_opt = slot_info_tree_reader.read_u32(1);
										if(!slot_space_value_opt)
										{
											return false;
										}
										param_desc.descriptor_desc.register_space = slot_space_value_opt.value();
									}
								}
							}
						}

						//
						root_signature_info.desc.param_descs.push_back(
							eastl::move(param_desc)
						);
					}

					return true;
				}
			)
		)
		{
			return eastl::nullopt;
		}

		// allow_input_assembler annotation
		if(context.current_object_config.find("allow_input_assembler") != context.current_object_config.end())
		{
			root_signature_info.desc.flags = flag_combine(
				root_signature_info.desc.flags,
				ED_root_signature_flag::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
			);
		}

		// cbv_srv_uav_heap_directly_indexed annotation
		if(context.current_object_config.find("cbv_srv_uav_heap_directly_indexed") != context.current_object_config.end())
		{
			root_signature_info.desc.flags = flag_combine(
				root_signature_info.desc.flags,
				ED_root_signature_flag::CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED
			);
		}

		// sampler_heap_directly_indexed annotation
		if(context.current_object_config.find("sampler_heap_directly_indexed") != context.current_object_config.end())
		{
			root_signature_info.desc.flags = flag_combine(
				root_signature_info.desc.flags,
				ED_root_signature_flag::SAMPLER_HEAP_DIRECTLY_INDEXED
			);
		}

		// bindless annotation
		if(context.current_object_config.find("bindless") != context.current_object_config.end())
		{
			root_signature_info.desc.flags = flag_combine(
				root_signature_info.desc.flags,
				ED_root_signature_flag::CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED,
				ED_root_signature_flag::SAMPLER_HEAP_DIRECTLY_INDEXED
			);
		}

		// register root_signature
		name_manager_p->template T_register_name<FE_nsl_name_types::ROOT_SIGNATURE>(tree.object_implementation.name);
		root_signature_manager_p->register_root_signature(
			tree.object_implementation.name,
			root_signature_info
		);

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_root_signature_object_type::F_nsl_root_signature_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		const G_string& name
	) :
		A_nsl_object_type(
			shader_compiler_p,
			name,
			true,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_root_signature_object_type::~F_nsl_root_signature_object_type() {
	}

	TK<A_nsl_object> F_nsl_root_signature_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_root_signature_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_global_root_signature_object::F_nsl_global_root_signature_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		F_nsl_root_signature_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_global_root_signature_object::~F_nsl_global_root_signature_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_global_root_signature_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		if(context.current_object_config.find("external") == context.current_object_config.end())
		{
			auto result_opt = F_nsl_root_signature_object::recursive_build_ast_tree(
				context,
				unit_p,
				trees,
				index,
				error_stack_p
			);
			if(!result_opt)
				return eastl::nullopt;

			shader_compiler_p()->root_signature_manager_p()->set_global_root_signature_selection({
				.name = object_implementation.name,
				.type = E_nsl_root_signature_selection_type::EMBEDDED
			});
			return result_opt.value();
		}

		shader_compiler_p()->root_signature_manager_p()->set_global_root_signature_selection({
			.name = object_implementation.name,
			.type = E_nsl_root_signature_selection_type::EXTERNAL
		});
		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_global_root_signature_object_type::F_nsl_global_root_signature_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		F_nsl_root_signature_object_type(
			shader_compiler_p,
			"global_root_signature"
		)
	{
	}
	F_nsl_global_root_signature_object_type::~F_nsl_global_root_signature_object_type() {
	}

	TK<A_nsl_object> F_nsl_global_root_signature_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_global_root_signature_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_local_root_signature_object::F_nsl_local_root_signature_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		F_nsl_root_signature_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		)
	{
	}
	F_nsl_local_root_signature_object::~F_nsl_local_root_signature_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_local_root_signature_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		if(context.current_object_config.find("external") == context.current_object_config.end())
		{
			auto result_opt = F_nsl_root_signature_object::recursive_build_ast_tree(
				context,
				unit_p,
				trees,
				index,
				error_stack_p
			);
			if(!result_opt)
				return eastl::nullopt;

			shader_compiler_p()->root_signature_manager_p()->set_local_root_signature_selection({
				.name = object_implementation.name,
				.type = E_nsl_root_signature_selection_type::EMBEDDED
			});
			return result_opt.value();
		}

		shader_compiler_p()->root_signature_manager_p()->set_local_root_signature_selection({
			.name = object_implementation.name,
			.type = E_nsl_root_signature_selection_type::EXTERNAL
		});
		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_local_root_signature_object_type::F_nsl_local_root_signature_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		F_nsl_root_signature_object_type(
			shader_compiler_p,
			"local_root_signature"
		)
	{
	}
	F_nsl_local_root_signature_object_type::~F_nsl_local_root_signature_object_type() {
	}

	TK<A_nsl_object> F_nsl_local_root_signature_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_local_root_signature_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}
#endif



#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
	F_nsl_work_graph_object::F_nsl_work_graph_object(
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
	F_nsl_work_graph_object::~F_nsl_work_graph_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_work_graph_object::recursive_build_ast_tree(
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
		auto work_graph_manager_p = shader_compiler_p()->work_graph_manager_p();

		//
		F_nsl_work_graph_info work_graph_info;
		work_graph_info.config_map = context.current_object_config;

		// include_all_available_nodes annotation
		if(context.current_object_config.find("include_all_available_nodes") != context.current_object_config.end())
		{
			work_graph_info.desc.flags = flag_combine(
				work_graph_info.desc.flags,
				ED_work_graph_flag::INCLUDE_ALL_AVAILABLE_NODES
			);
		}

		// register root_signature
		name_manager_p->template T_register_name<FE_nsl_name_types::WORK_GRAPH>(tree.object_implementation.name);
		work_graph_manager_p->register_work_graph(
			tree.object_implementation.name,
			work_graph_info
		);

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_work_graph_object_type::F_nsl_work_graph_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"work_graph",
			true,
			1,
			1,
			nsl_global_object_type_channel_mask
		)
	{
	}
	F_nsl_work_graph_object_type::~F_nsl_work_graph_object_type() {
	}

	TK<A_nsl_object> F_nsl_work_graph_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_work_graph_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}
#endif



#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
	F_nsl_state_object_config_object::F_nsl_state_object_config_object(
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
	F_nsl_state_object_config_object::~F_nsl_state_object_config_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_state_object_config_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	) {
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		F_state_object_config config;

		// allow_state_object_additions annotation
		if(context.current_object_config.find("allow_state_object_additions") != context.current_object_config.end())
		{
			config.flags = flag_combine(
				config.flags,
				ED_state_object_flag::ALLOW_STATE_OBJECT_ADDITIONS
			);
		}

		// allow_external_dependencies_on_local_definitions annotation
		if(context.current_object_config.find("allow_external_dependencies_on_local_definitions") != context.current_object_config.end())
		{
			config.flags = flag_combine(
				config.flags,
				ED_state_object_flag::ALLOW_EXTERNAL_DEPENDENCIES_ON_LOCAL_DEFINITIONS
			);
		}

		// allow_local_dependencies_on_external_definitions annotation
		if(context.current_object_config.find("allow_local_dependencies_on_external_definitions") != context.current_object_config.end())
		{
			config.flags = flag_combine(
				config.flags,
				ED_state_object_flag::ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS
			);
		}

		shader_compiler_p()->state_object_p()->set_config(config);

		return TG_vector<F_nsl_ast_tree>();
	}



	F_nsl_state_object_config_object_type::F_nsl_state_object_config_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"state_object_config",
			false,
			0,
			0
		)
	{
	}
	F_nsl_state_object_config_object_type::~F_nsl_state_object_config_object_type() {
	}

	TK<A_nsl_object> F_nsl_state_object_config_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_state_object_config_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}
#endif



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	F_nsl_descriptor_heap_getter_object::F_nsl_descriptor_heap_getter_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		ED_descriptor_heap_type descriptor_heap_type,
		E_nsl_resource_type resource_type,
		const G_string& name
	) :
		A_nsl_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			name
		),
		descriptor_heap_type_(descriptor_heap_type),
		resource_type_(resource_type)
	{
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_descriptor_heap_getter_object::recursive_build_ast_tree(
		F_nsl_context& context,
		TK_valid<F_nsl_translation_unit> unit_p,
		TG_vector<F_nsl_ast_tree>& trees,
		sz index,
		F_nsl_error_stack* error_stack_p
	)
	{
		auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();

		//
		auto& tree = trees[index];
		auto& object_implementation = tree.object_implementation;

		// parse body
		context.object_type_channel_mask_stack.push(nsl_function_body_object_type_channel_mask);
		u32 index_body = type_p()->max_object_body_count() - 1;
		auto body_childs_opt = translation_unit_compiler_p->parse(
			unit_p,
			object_implementation.bodies[index_body].content,
			context,
			object_implementation.bodies[index_body].begin_location
		);
		if(!body_childs_opt) {

			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				object_implementation.bodies[index_body].begin_location,
				"can't parse $(...) block"
			);
			return eastl::nullopt;
		}
		context.object_type_channel_mask_stack.pop();

		//
		TG_vector<F_nsl_ast_tree> child_trees = eastl::move(body_childs_opt.value());

		//
		return eastl::move(child_trees);
	}
	eastl::optional<G_string> F_nsl_descriptor_heap_getter_object::apply(
		const F_nsl_ast_tree& tree
	)
	{
		auto output_language_p = shader_compiler_p()->output_language_p();
		auto translation_unit_compiler_p = shader_compiler_p()->translation_unit_compiler_p();

		auto childs_to_string_opt = translation_unit_compiler_p->ast_trees_to_string(
			tree.childs
		);
		if(!childs_to_string_opt)
			return childs_to_string_opt;

		return output_language_p->descriptor_heap_getter_to_string(
			translation_unit_p(),
			tree,
			childs_to_string_opt.value()
		);
	}



	F_nsl_descriptor_heap_getter_object_type::F_nsl_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		const G_string& name,
		b8 is_object_name_required,
		u32 min_body_count,
		u32 max_body_count
	) :
		A_nsl_object_type(
			shader_compiler_p,
			name,
			is_object_name_required,
			min_body_count,
			max_body_count
		)
	{
	}

	TK<A_nsl_object> F_nsl_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::NONE,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_sampler_state_descriptor_heap_getter_object_type::F_nsl_sampler_state_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$SamplerState",
			false,
			1,
			1
		)
	{
	}

	TK<A_nsl_object> F_nsl_sampler_state_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::SAMPLER,
				E_nsl_resource_type::NONE,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_constant_buffer_descriptor_heap_getter_object_type::F_nsl_constant_buffer_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$ConstantBuffer",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_constant_buffer_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::CONSTANT_BUFFER,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_buffer_descriptor_heap_getter_object_type::F_nsl_buffer_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$Buffer",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_buffer_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::BUFFER,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_byte_address_buffer_descriptor_heap_getter_object_type::F_nsl_byte_address_buffer_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$ByteAddressBuffer",
			false,
			1,
			1
		)
	{
	}

	TK<A_nsl_object> F_nsl_byte_address_buffer_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::BYTE_ADDRESS_BUFFER,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_structured_buffer_descriptor_heap_getter_object_type::F_nsl_structured_buffer_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$StructuredBuffer",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_structured_buffer_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::STRUCTURED_BUFFER,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_texture_1d_descriptor_heap_getter_object_type::F_nsl_texture_1d_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$Texture1D",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_texture_1d_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::TEXTURE_1D,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_texture_1d_array_descriptor_heap_getter_object_type::F_nsl_texture_1d_array_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$Texture1DArray",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_texture_1d_array_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::TEXTURE_1D_ARRAY,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_texture_2d_descriptor_heap_getter_object_type::F_nsl_texture_2d_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$Texture2D",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_texture_2d_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::TEXTURE_2D,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_texture_2d_array_descriptor_heap_getter_object_type::F_nsl_texture_2d_array_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$Texture2DArray",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_texture_2d_array_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::TEXTURE_2D_ARRAY,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_texture_3d_descriptor_heap_getter_object_type::F_nsl_texture_3d_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$Texture3D",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_texture_3d_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::TEXTURE_3D,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_texture_cube_descriptor_heap_getter_object_type::F_nsl_texture_cube_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$TextureCube",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_texture_cube_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::TEXTURE_CUBE,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_texture_cube_array_descriptor_heap_getter_object_type::F_nsl_texture_cube_array_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$TextureCubeArray",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_texture_cube_array_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::TEXTURE_CUBE_ARRAY,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rw_buffer_descriptor_heap_getter_object_type::F_nsl_rw_buffer_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RWBuffer",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rw_buffer_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RW_BUFFER,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rw_byte_address_buffer_descriptor_heap_getter_object_type::F_nsl_rw_byte_address_buffer_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RWByteAddressBuffer",
			false,
			1,
			1
		)
	{
	}

	TK<A_nsl_object> F_nsl_rw_byte_address_buffer_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RW_BYTE_ADDRESS_BUFFER,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rw_structured_buffer_descriptor_heap_getter_object_type::F_nsl_rw_structured_buffer_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RWStructuredBuffer",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rw_structured_buffer_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RW_STRUCTURED_BUFFER,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rw_texture_1d_descriptor_heap_getter_object_type::F_nsl_rw_texture_1d_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RWTexture1D",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rw_texture_1d_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RW_TEXTURE_1D,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rw_texture_1d_array_descriptor_heap_getter_object_type::F_nsl_rw_texture_1d_array_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RWTexture1DArray",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rw_texture_1d_array_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RW_TEXTURE_1D_ARRAY,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rw_texture_2d_descriptor_heap_getter_object_type::F_nsl_rw_texture_2d_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RWTexture2D",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rw_texture_2d_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RW_TEXTURE_2D,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rw_texture_2d_array_descriptor_heap_getter_object_type::F_nsl_rw_texture_2d_array_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RWTexture2DArray",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rw_texture_2d_array_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RW_TEXTURE_2D_ARRAY,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rw_texture_3d_descriptor_heap_getter_object_type::F_nsl_rw_texture_3d_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RWTexture3D",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rw_texture_3d_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RW_TEXTURE_3D,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rasterizer_ordered_buffer_descriptor_heap_getter_object_type::F_nsl_rasterizer_ordered_buffer_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RasterizerOrderedBuffer",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rasterizer_ordered_buffer_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RASTERIZER_ORDERED_BUFFER,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rasterizer_ordered_byte_address_buffer_descriptor_heap_getter_object_type::F_nsl_rasterizer_ordered_byte_address_buffer_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RasterizerOrderedByteAddressBuffer",
			false,
			1,
			1
		)
	{
	}

	TK<A_nsl_object> F_nsl_rasterizer_ordered_byte_address_buffer_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RASTERIZER_ORDERED_BYTE_ADDRESS_BUFFER,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rasterizer_ordered_structured_buffer_descriptor_heap_getter_object_type::F_nsl_rasterizer_ordered_structured_buffer_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RasterizerOrderedStructuredBuffer",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rasterizer_ordered_structured_buffer_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RASTERIZER_ORDERED_STRUCTURED_BUFFER,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rasterizer_ordered_texture_1d_descriptor_heap_getter_object_type::F_nsl_rasterizer_ordered_texture_1d_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RasterizerOrderedTexture1D",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rasterizer_ordered_texture_1d_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_1D,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rasterizer_ordered_texture_1d_array_descriptor_heap_getter_object_type::F_nsl_rasterizer_ordered_texture_1d_array_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RasterizerOrderedTexture1DArray",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rasterizer_ordered_texture_1d_array_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_1D_ARRAY,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rasterizer_ordered_texture_2d_descriptor_heap_getter_object_type::F_nsl_rasterizer_ordered_texture_2d_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RasterizerOrderedTexture2D",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rasterizer_ordered_texture_2d_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_2D,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rasterizer_ordered_texture_2d_array_descriptor_heap_getter_object_type::F_nsl_rasterizer_ordered_texture_2d_array_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RasterizerOrderedTexture2DArray",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rasterizer_ordered_texture_2d_array_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_2D_ARRAY,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}



	F_nsl_rasterizer_ordered_texture_3d_descriptor_heap_getter_object_type::F_nsl_rasterizer_ordered_texture_3d_descriptor_heap_getter_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_object_type(
			shader_compiler_p,
			"$RasterizerOrderedTexture3D",
			false,
			2,
			2
		)
	{
	}

	TK<A_nsl_object> F_nsl_rasterizer_ordered_texture_3d_descriptor_heap_getter_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	)
	{
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_descriptor_heap_getter_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_3D,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}
#endif



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
			return childs_to_string_opt;

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
				"can't parse shader params"
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

				auto type_selection_opt = param_child_info_tree_reader.read_data_type_selection(0);
				if(!type_selection_opt)
					return eastl::nullopt;

				data_params_.push_back(
					F_nsl_data_param {
						.argument = F_nsl_data_argument{
							.name = param_child_info_tree.name,
							.type_selection = type_selection_opt.value(),
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
				"can't parse shader body"
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

		// @default_slot_space annotation
		{
			auto it = context.current_object_config.find("default_slot_space");
			if(it != context.current_object_config.end()) {

				auto value_opt = it->second.read_u32(0);

				if(!value_opt)
					return eastl::nullopt;

				default_slot_space = value_opt.value();
			}
		}

		//
		if(
			(thread_group_size_.x == 0)
			|| (thread_group_size_.y == 0)
			|| (thread_group_size_.z == 0)
		)
		{
			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				trees[index].begin_location,
				"invalid thread group size: ["
				+ G_to_string(thread_group_size_.x)
				+ ", "
				+ G_to_string(thread_group_size_.y)
				+ ", "
				+ G_to_string(thread_group_size_.z)
				+ "]"
			);
			return eastl::nullopt;
		}

		return std::move(childs);
	}
	eastl::optional<G_string> F_nsl_compute_shader_object::apply(
		const F_nsl_ast_tree& tree
	) {
		return apply_shader_with_customizations(
			tree,
			G_string("NSL_PRE_SHADER_KEYWORDS_NUM_THREADS(")
			+ G_to_string(thread_group_size_.x)
			+ ","
			+ G_to_string(thread_group_size_.y)
			+ ","
			+ G_to_string(thread_group_size_.z)
			+ ")\n"
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



#ifdef NRHI_DRIVER_SUPPORT_AMPLIFICATION_SHADER
	F_nsl_amplification_shader_object::F_nsl_amplification_shader_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_shader_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			ED_shader_type::AMPLIFICATION,
			name
		)
	{
	}
	F_nsl_amplification_shader_object::~F_nsl_amplification_shader_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_amplification_shader_object::recursive_build_ast_tree(
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

		// @default_slot_space annotation
		{
			auto it = context.current_object_config.find("default_slot_space");
			if(it != context.current_object_config.end()) {

				auto value_opt = it->second.read_u32(0);

				if(!value_opt)
					return eastl::nullopt;

				default_slot_space = value_opt.value();
			}
		}

		//


		//
		if(
			(thread_group_size_.x == 0)
			|| (thread_group_size_.y == 0)
			|| (thread_group_size_.z == 0)
		)
		{
			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				trees[index].begin_location,
				"invalid thread group size: ["
				+ G_to_string(thread_group_size_.x)
				+ ", "
				+ G_to_string(thread_group_size_.y)
				+ ", "
				+ G_to_string(thread_group_size_.z)
				+ "]"
			);
			return eastl::nullopt;
		}

		return std::move(childs);
	}
	eastl::optional<G_string> F_nsl_amplification_shader_object::apply(
		const F_nsl_ast_tree& tree
	) {
		return apply_shader_with_customizations(
			tree,
			G_string("NSL_PRE_SHADER_KEYWORDS_NUM_THREADS(")
			+ G_to_string(thread_group_size_.x)
			+ ","
			+ G_to_string(thread_group_size_.y)
			+ ","
			+ G_to_string(thread_group_size_.z)
			+ ")\n"
		);
	}



	F_nsl_amplification_shader_object_type::F_nsl_amplification_shader_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_shader_object_type(
			shader_compiler_p,
			"amplification_shader"
		)
	{
	}
	F_nsl_amplification_shader_object_type::~F_nsl_amplification_shader_object_type() {
	}

	TK<A_nsl_object> F_nsl_amplification_shader_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_amplification_shader_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}
#endif



#ifdef NRHI_DRIVER_SUPPORT_MESH_SHADER
	F_nsl_mesh_shader_object::F_nsl_mesh_shader_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_shader_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			ED_shader_type::MESH,
			name
		)
	{
	}
	F_nsl_mesh_shader_object::~F_nsl_mesh_shader_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_mesh_shader_object::recursive_build_ast_tree(
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

		// @output_topology annotation
		{
			auto it = context.current_object_config.find("output_topology");
			if(it != context.current_object_config.end()) {

				auto value_opt = it->second.read_output_topology(0);

				if(!value_opt)
					return eastl::nullopt;

				output_topology_ = value_opt.value();
			}
		}

		// @default_slot_space annotation
		{
			auto it = context.current_object_config.find("default_slot_space");
			if(it != context.current_object_config.end()) {

				auto value_opt = it->second.read_u32(0);

				if(!value_opt)
					return eastl::nullopt;

				default_slot_space = value_opt.value();
			}
		}

		//
		for(auto& data_param : data_params_)
		{
			auto& config_map = data_param.argument.config_map;

			if(config_map.find("indices") != config_map.end())
				data_param.flags = flag_combine(data_param.flags, E_nsl_data_param_flag::INDICES);

			if(config_map.find("vertices") != config_map.end())
				data_param.flags = flag_combine(data_param.flags, E_nsl_data_param_flag::VERTICES);

			if(config_map.find("primitives") != config_map.end())
				data_param.flags = flag_combine(data_param.flags, E_nsl_data_param_flag::PRIMITIVES);

			if(config_map.find("payload") != config_map.end())
				data_param.flags = flag_combine(data_param.flags, E_nsl_data_param_flag::PAYLOAD);
		}

		//


		//
		if(
			(thread_group_size_.x == 0)
			|| (thread_group_size_.y == 0)
			|| (thread_group_size_.z == 0)
		)
		{
			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				trees[index].begin_location,
				"invalid thread group size: ["
				+ G_to_string(thread_group_size_.x)
				+ ", "
				+ G_to_string(thread_group_size_.y)
				+ ", "
				+ G_to_string(thread_group_size_.z)
				+ "]"
			);
			return eastl::nullopt;
		}

		return std::move(childs);
	}
	eastl::optional<G_string> F_nsl_mesh_shader_object::apply(
		const F_nsl_ast_tree& tree
	) {
		G_string output_topology_str;
		if(output_topology_ == E_nsl_output_topology::TRIANGLE)
			output_topology_str = "triangle";
		if(output_topology_ == E_nsl_output_topology::LINE)
			output_topology_str = "line";

		return apply_shader_with_customizations(
			tree,
			G_string("NSL_PRE_SHADER_KEYWORDS_NUM_THREADS(")
			+ G_to_string(thread_group_size_.x)
			+ ","
			+ G_to_string(thread_group_size_.y)
			+ ","
			+ G_to_string(thread_group_size_.z)
			+ ")\n"
			+ "NSL_PRE_SHADER_KEYWORDS_OUTPUT_TOPOLOGY(\""
			+ output_topology_str
			+ "\")\n"
		);
	}



	F_nsl_mesh_shader_object_type::F_nsl_mesh_shader_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_shader_object_type(
			shader_compiler_p,
			"mesh_shader"
		)
	{
	}
	F_nsl_mesh_shader_object_type::~F_nsl_mesh_shader_object_type() {
	}

	TK<A_nsl_object> F_nsl_mesh_shader_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_mesh_shader_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}
#endif



#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
	F_nsl_node_shader_object::F_nsl_node_shader_object(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		TKPA_valid<A_nsl_object_type> type_p,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const G_string& name
	) :
		A_nsl_shader_object(
			shader_compiler_p,
			type_p,
			translation_unit_p,
			ED_shader_type::NODE,
			name
		)
	{
	}
	F_nsl_node_shader_object::~F_nsl_node_shader_object() {
	}

	eastl::optional<TG_vector<F_nsl_ast_tree>> F_nsl_node_shader_object::recursive_build_ast_tree(
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
			if(it != context.current_object_config.end())
			{
				thread_group_size_ = F_vector3_u32::one();
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

		// @max_dispatch_grid annotation
		{
			auto it = context.current_object_config.find("max_dispatch_grid");
			if(it != context.current_object_config.end())
			{
				max_dispatch_grid_ = F_vector3_u32::one();
				{
					auto value_opt = it->second.read_u32(0);

					if(!value_opt)
						return eastl::nullopt;

					max_dispatch_grid_.x = value_opt.value();
				}
				{
					auto value_opt = it->second.read_u32(1);

					if(!value_opt)
						return eastl::nullopt;

					max_dispatch_grid_.y = value_opt.value();
				}
				{
					auto value_opt = it->second.read_u32(2);

					if(!value_opt)
						return eastl::nullopt;

					max_dispatch_grid_.z = value_opt.value();
				}
			}
		}

		// @default_slot_space annotation
		{
			auto it = context.current_object_config.find("default_slot_space");
			if(it != context.current_object_config.end()) {

				auto value_opt = it->second.read_u32(0);

				if(!value_opt)
					return eastl::nullopt;

				default_slot_space = value_opt.value();
			}
		}

		//
		if(
			(thread_group_size_.x == 0)
			|| (thread_group_size_.y == 0)
			|| (thread_group_size_.z == 0)
		)
		{
			NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(
				&(unit_p->error_group_p()->stack()),
				trees[index].begin_location,
				"invalid thread group size: ["
				+ G_to_string(thread_group_size_.x)
				+ ", "
				+ G_to_string(thread_group_size_.y)
				+ ", "
				+ G_to_string(thread_group_size_.z)
				+ "]"
			);
			return eastl::nullopt;
		}

		return std::move(childs);
	}
	eastl::optional<G_string> F_nsl_node_shader_object::apply(
		const F_nsl_ast_tree& tree
	) {
		G_string launch_str;
		switch (launch_)
		{
		case E_nsl_node_launch::BROADCASTING:
			launch_str = "\"broadcasting\"";
			break;
		case E_nsl_node_launch::COALESCING:
			launch_str = "\"coalescing\"";
			break;
		case E_nsl_node_launch::THREAD:
			launch_str = "\"thread\"";
			break;
		}

		G_string max_dispatch_grid_str;
		if(max_dispatch_grid_ != F_vector3_u32::zero())
		{
			max_dispatch_grid_str = "[NodeMaxDispatchGrid("
			+ G_to_string(max_dispatch_grid_.x)
			+ ","
			+ G_to_string(max_dispatch_grid_.y)
			+ ","
			+ G_to_string(max_dispatch_grid_.z)
			+ ")]\n";
		}

		return apply_shader_with_customizations(
			tree,
			G_string("[Shader(\"node\")]\n")
			+ "[NodeLaunch(" + launch_str + ")]\n"

			+ max_dispatch_grid_str

			+ "NSL_PRE_SHADER_KEYWORDS_NUM_THREADS("
			+ G_to_string(thread_group_size_.x)
			+ ","
			+ G_to_string(thread_group_size_.y)
			+ ","
			+ G_to_string(thread_group_size_.z)
			+ ")\n"
		);
	}



	F_nsl_node_shader_object_type::F_nsl_node_shader_object_type(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
	) :
		A_nsl_shader_object_type(
			shader_compiler_p,
			"node_shader"
		)
	{
	}
	F_nsl_node_shader_object_type::~F_nsl_node_shader_object_type() {
	}

	TK<A_nsl_object> F_nsl_node_shader_object_type::create_object(
		F_nsl_ast_tree& tree,
		F_nsl_context& context,
		TKPA_valid<F_nsl_translation_unit> translation_unit_p
	) {
		NCPP_ASSERT(tree.type == E_nsl_ast_tree_type::OBJECT_IMPLEMENTATION) << "invalid ast tree type";

		auto object_p = register_object(
			TU<F_nsl_node_shader_object>()(
				shader_compiler_p(),
				NCPP_KTHIS(),
				translation_unit_p,
				tree.object_implementation.name
			)
		);

		tree.object_implementation.attached_object_p = object_p;

		return object_p;
	}
#endif



	F_nsl_object_manager::F_nsl_object_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
		register_type(
			TU<F_nsl_import_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_submodule_object_type>()(shader_compiler_p_)
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
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		register_type(
			TU<F_nsl_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_sampler_state_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_constant_buffer_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_buffer_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_structured_buffer_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_texture_1d_descriptor_heap_getter_object_type>()(shader_compiler_p_)
			);
		register_type(
			TU<F_nsl_texture_1d_array_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_texture_2d_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_texture_2d_array_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_texture_3d_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_texture_cube_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_texture_cube_array_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_rw_buffer_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_rw_structured_buffer_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_rw_texture_1d_descriptor_heap_getter_object_type>()(shader_compiler_p_)
			);
		register_type(
			TU<F_nsl_rw_texture_1d_array_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_rw_texture_2d_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_rw_texture_2d_array_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_rw_texture_3d_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_rasterizer_ordered_buffer_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_rasterizer_ordered_structured_buffer_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_rasterizer_ordered_texture_1d_descriptor_heap_getter_object_type>()(shader_compiler_p_)
			);
		register_type(
			TU<F_nsl_rasterizer_ordered_texture_1d_array_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_rasterizer_ordered_texture_2d_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_rasterizer_ordered_texture_2d_array_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_rasterizer_ordered_texture_3d_descriptor_heap_getter_object_type>()(shader_compiler_p_)
		);
#endif
		register_type(
			TU<F_nsl_vertex_shader_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_pixel_shader_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_compute_shader_object_type>()(shader_compiler_p_)
		);
#ifdef NRHI_DRIVER_SUPPORT_AMPLIFICATION_SHADER
		register_type(
			TU<F_nsl_amplification_shader_object_type>()(shader_compiler_p_)
		);
#endif
#ifdef NRHI_DRIVER_SUPPORT_MESH_SHADER
		register_type(
			TU<F_nsl_mesh_shader_object_type>()(shader_compiler_p_)
		);
#endif
#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
		register_type(
			TU<F_nsl_node_shader_object_type>()(shader_compiler_p_)
		);
#endif
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
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		register_type(
			TU<F_nsl_root_signature_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_global_root_signature_object_type>()(shader_compiler_p_)
		);
		register_type(
			TU<F_nsl_local_root_signature_object_type>()(shader_compiler_p_)
		);
#endif
#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
		register_type(
			TU<F_nsl_work_graph_object_type>()(shader_compiler_p_)
		);
#endif
#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
		register_type(
			TU<F_nsl_state_object_config_object_type>()(shader_compiler_p_)
		);
#endif
		register_type(
			TU<F_nsl_default_pipeline_state_object_type>()(shader_compiler_p_)
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
				flag_is_has_any(
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

		b8 auto_bind_slot = !(shader_compiler_p()->name_manager_p()->is_name_registered("NSL_DISABLE_AUTO_SLOTS"));

		u32 shader_count = name_to_shader_object_p_map.size();

		for(auto& sampler_state : name_to_sampler_state_info_map) {

			sampler_state.second.actual_slots.resize(shader_count);
			for(auto& actual_slot : sampler_state.second.actual_slots)
				actual_slot = -1;

			sampler_state.second.actual_slot_spaces.resize(shader_count);
			for(auto& actual_slot_space : sampler_state.second.actual_slot_spaces)
				actual_slot_space = -1;
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
				for(const auto& shader_filter : sampler_state.second.shader_filters) {

					shader_name_to_sampler_state_iterators_map[shader_filter.first].push_back(it);
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
			if(auto_bind_slot)
			{
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
			}

			// bind actual slots
			for(u32 i = 0; i < sampler_state_iterator_size; ++i) {

				auto& sampler_state_it = sampler_state_iterators[i];
				auto& sampler_state = *sampler_state_it;

				if(auto_bind_slot)
					sampler_state.second.actual_slots[shader_index] = i;
				else
					sampler_state.second.actual_slots[shader_index] = sampler_state.second.slot;

				auto& actual_slot_space = sampler_state.second.actual_slot_spaces[shader_index];
				if(sampler_state.second.slot_space == -1)
				{
					actual_slot_space = shader_object_p->default_slot_space;
				}
				else
				{
					actual_slot_space = sampler_state.second.slot_space;
				}
			}
		}

		return true;
	}
	b8 F_nsl_translation_unit_compiler::setup_srv_resource_actual_slots() {

		auto shader_manager_p = shader_compiler_p_->shader_manager_p();
		auto resource_manager_p = shader_compiler_p_->resource_manager_p();

		auto& name_to_shader_object_p_map = shader_manager_p->name_to_shader_object_p_map();
		auto& name_to_resource_info_map = resource_manager_p->name_to_resource_info_map();

		b8 auto_bind_slot = !(shader_compiler_p()->name_manager_p()->is_name_registered("NSL_DISABLE_AUTO_SLOTS"));

		u32 shader_count = name_to_shader_object_p_map.size();

		for(auto& resource : name_to_resource_info_map) {

			if(resource.second.type_class != E_nsl_resource_type_class::SHADER_RESOURCE)
				continue;

			resource.second.actual_slots.resize(shader_count);
			for(auto& actual_slot : resource.second.actual_slots)
				actual_slot = -1;

			resource.second.actual_slot_spaces.resize(shader_count);
			for(auto& actual_slot_space : resource.second.actual_slot_spaces)
				actual_slot_space = -1;
		}

		using F_resource_iterator = std::remove_reference_t<decltype(name_to_resource_info_map)>::iterator;
		TG_unordered_map<G_string, TG_vector<F_resource_iterator>> shader_name_to_resource_iterators_map;

		// bind resources to shaders
		for(auto& [shader_name, shader_object_p] : name_to_shader_object_p_map) {

			shader_name_to_resource_iterators_map[shader_name] = TG_vector<F_resource_iterator>();
		}
		for(auto it = name_to_resource_info_map.begin(); it != name_to_resource_info_map.end(); ++it) {

			auto& resource = *it;

			if(resource.second.type_class != E_nsl_resource_type_class::SHADER_RESOURCE)
				continue;

			b8 is_all_shader = (resource.second.shader_filters.find("*") != resource.second.shader_filters.end());

			if(is_all_shader) {

				for(auto& [shader_name, resource_iterators] : shader_name_to_resource_iterators_map) {

					resource_iterators.push_back(it);
				}
			}
			else {
				for(const auto& shader_filter : resource.second.shader_filters) {

					shader_name_to_resource_iterators_map[shader_filter.first].push_back(it);
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
			if(auto_bind_slot)
			{
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
			}

			// bind actual slots
			for(u32 i = 0; i < resource_iterator_size; ++i) {

				auto& resource_it = resource_iterators[i];
				auto& resource = *resource_it;

				if(auto_bind_slot)
					resource.second.actual_slots[shader_index] = i;
				else
					resource.second.actual_slots[shader_index] = resource.second.slot;

				auto& actual_slot_space = resource.second.actual_slot_spaces[shader_index];
				if(resource.second.slot_space == -1)
				{
					actual_slot_space = shader_object_p->default_slot_space;
				}
				else
				{
					actual_slot_space = resource.second.slot_space;
				}
			}
		}

		return true;
	}
	b8 F_nsl_translation_unit_compiler::setup_uav_resource_actual_slots() {

		auto shader_manager_p = shader_compiler_p_->shader_manager_p();
		auto resource_manager_p = shader_compiler_p_->resource_manager_p();

		auto& name_to_shader_object_p_map = shader_manager_p->name_to_shader_object_p_map();
		auto& name_to_resource_info_map = resource_manager_p->name_to_resource_info_map();

		b8 auto_bind_slot = !(shader_compiler_p()->name_manager_p()->is_name_registered("NSL_DISABLE_AUTO_SLOTS"));

		u32 shader_count = name_to_shader_object_p_map.size();

		for(auto& resource : name_to_resource_info_map) {

			if(resource.second.type_class != E_nsl_resource_type_class::UNORDERED_ACCESS)
				continue;

			resource.second.actual_slots.resize(shader_count);
			for(auto& actual_slot : resource.second.actual_slots)
				actual_slot = -1;

			resource.second.actual_slot_spaces.resize(shader_count);
			for(auto& actual_slot_space : resource.second.actual_slot_spaces)
				actual_slot_space = -1;
		}

		using F_resource_iterator = std::remove_reference_t<decltype(name_to_resource_info_map)>::iterator;
		TG_unordered_map<G_string, TG_vector<F_resource_iterator>> shader_name_to_resource_iterators_map;

		// bind resources to shaders
		for(auto& [shader_name, shader_object_p] : name_to_shader_object_p_map) {

			shader_name_to_resource_iterators_map[shader_name] = TG_vector<F_resource_iterator>();
		}
		for(auto it = name_to_resource_info_map.begin(); it != name_to_resource_info_map.end(); ++it) {

			auto& resource = *it;

			if(resource.second.type_class != E_nsl_resource_type_class::UNORDERED_ACCESS)
				continue;

			b8 is_all_shader = (resource.second.shader_filters.find("*") != resource.second.shader_filters.end());

			if(is_all_shader) {

				for(auto& [shader_name, resource_iterators] : shader_name_to_resource_iterators_map) {

					resource_iterators.push_back(it);
				}
			}
			else {
				for(const auto& shader_filter : resource.second.shader_filters) {

					shader_name_to_resource_iterators_map[shader_filter.first].push_back(it);
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
			if(auto_bind_slot)
			{
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
			}

			// bind actual slots
			for(u32 i = 0; i < resource_iterator_size; ++i) {

				auto& resource_it = resource_iterators[i];
				auto& resource = *resource_it;

				if(auto_bind_slot)
					resource.second.actual_slots[shader_index] = i;
				else
					resource.second.actual_slots[shader_index] = resource.second.slot;

				auto& actual_slot_space = resource.second.actual_slot_spaces[shader_index];
				if(resource.second.slot_space == -1)
				{
					actual_slot_space = shader_object_p->default_slot_space;
				}
				else
				{
					actual_slot_space = resource.second.slot_space;
				}
			}
		}

		return true;
	}
	b8 F_nsl_translation_unit_compiler::setup_cbv_resource_actual_slots() {

		auto shader_manager_p = shader_compiler_p_->shader_manager_p();
		auto resource_manager_p = shader_compiler_p_->resource_manager_p();

		auto& name_to_shader_object_p_map = shader_manager_p->name_to_shader_object_p_map();
		auto& name_to_resource_info_map = resource_manager_p->name_to_resource_info_map();

		b8 auto_bind_slot = !(shader_compiler_p()->name_manager_p()->is_name_registered("NSL_DISABLE_AUTO_SLOTS"));

		u32 shader_count = name_to_shader_object_p_map.size();

		for(auto& resource : name_to_resource_info_map) {

			if(resource.second.type_class != E_nsl_resource_type_class::CONSTANT_BUFFER)
				continue;

			resource.second.actual_slots.resize(shader_count);
			for(auto& actual_slot : resource.second.actual_slots)
				actual_slot = -1;

			resource.second.actual_slot_spaces.resize(shader_count);
			for(auto& actual_slot_space : resource.second.actual_slot_spaces)
				actual_slot_space = -1;
		}

		using F_resource_iterator = std::remove_reference_t<decltype(name_to_resource_info_map)>::iterator;
		TG_unordered_map<G_string, TG_vector<F_resource_iterator>> shader_name_to_resource_iterators_map;

		// bind resources to shaders
		for(auto& [shader_name, shader_object_p] : name_to_shader_object_p_map) {

			shader_name_to_resource_iterators_map[shader_name] = TG_vector<F_resource_iterator>();
		}
		for(auto it = name_to_resource_info_map.begin(); it != name_to_resource_info_map.end(); ++it) {

			auto& resource = *it;

			if(resource.second.type_class != E_nsl_resource_type_class::CONSTANT_BUFFER)
				continue;

			b8 is_all_shader = (resource.second.shader_filters.find("*") != resource.second.shader_filters.end());

			if(is_all_shader) {

				for(auto& [shader_name, resource_iterators] : shader_name_to_resource_iterators_map) {

					resource_iterators.push_back(it);
				}
			}
			else {
				for(const auto& shader_filter : resource.second.shader_filters) {

					shader_name_to_resource_iterators_map[shader_filter.first].push_back(it);
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
			if(auto_bind_slot)
			{
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
			}

			// bind actual slots
			for(u32 i = 0; i < resource_iterator_size; ++i) {

				auto& resource_it = resource_iterators[i];
				auto& resource = *resource_it;

				if(auto_bind_slot)
					resource.second.actual_slots[shader_index] = i;
				else
					resource.second.actual_slots[shader_index] = resource.second.slot;

				auto& actual_slot_space = resource.second.actual_slot_spaces[shader_index];
				if(resource.second.slot_space == -1)
				{
					actual_slot_space = shader_object_p->default_slot_space;
				}
				else
				{
					actual_slot_space = resource.second.slot_space;
				}
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
				if(resource.second.sort_uniforms)
				{
					auto sort_func = [&](const F_uniform_iterator& a, const F_uniform_iterator& b) -> b8 {

						return (
							data_type_manager_p->size(a->second.type_selection.name)
							> data_type_manager_p->size(b->second.type_selection.name)
						);
					};
					eastl::sort(uniform_iterator_vector.begin(), uniform_iterator_vector.end(), sort_func);
				}
				else
				{
					auto sort_func = [&](const F_uniform_iterator& a, const F_uniform_iterator& b) -> b8 {

						return (a->second.creation_index < b->second.creation_index);
					};
					eastl::sort(uniform_iterator_vector.begin(), uniform_iterator_vector.end(), sort_func);
				}

				// calculate buffer size and uniform offset
				{
					constexpr u32 min_pack_alignment = 16;

					u32 offset = 0;

					for(u32 i = 0; i < uniform_count; ++i) {

						F_uniform_iterator& uniform_iterator = uniform_iterator_vector[i];

						u32 member_alignment = data_type_manager_p->alignment(uniform_iterator->second.type_selection.name);
						u32 member_element_count = uniform_iterator->second.count;

						u32 member_single_element_size = data_type_manager_p->size(uniform_iterator->second.type_selection.name);
						u32 aligned_member_single_element_size = align_size(member_single_element_size, member_alignment);

						u32 member_size = (
							(member_element_count - 1) * align_size(aligned_member_single_element_size, min_pack_alignment)
							+ member_single_element_size
						);

						offset = align_size(offset, member_alignment);

						uniform_iterator->second.offset = offset;

						offset += member_size;
					}

					resource.second.constant_size = align_size(offset, NRHI_CONSTANT_BUFFER_MIN_ALIGNMENT);
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

		// compile main unit
		{
			auto unit_compiled_result_opt = compile_unit(
				NCPP_FOH_VALID(main_unit_p_)
			);

			if(!unit_compiled_result_opt)
				return false;

			compiled_result_.src_content += unit_compiled_result_opt.value();
		}

		return true;
	}

	eastl::optional<G_string> F_nsl_translation_unit_compiler::compile_unit(TKPA_valid<F_nsl_translation_unit> unit_p)
	{
		if(unit_p->is_compiled())
			return G_string();

		unit_p->is_compiled_ = true;

		auto compiled_result_opt_from_ast_tree = ast_trees_to_string(
			unit_p->ast_trees()
		);

		if(!compiled_result_opt_from_ast_tree)
			return eastl::nullopt;

		return compiled_result_opt_from_ast_tree;
	}

	b8 F_nsl_translation_unit_compiler::compile_minimal() {

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

		if(unit_p->is_prepared())
			return true;

		unit_p->is_prepared_ = true;

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
		shader_compiler_p()->shader_module_manager_p()->register_translation_unit(
			abs_path,
			NCPP_FOH_VALID(main_unit_p_)
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

			u32 member_alignment = alignment(argument.type_selection.name);
			u32 member_element_count = argument.count;

			u32 member_single_element_size = size(argument.type_selection.name);
			u32 aligned_member_single_element_size = align_size(member_single_element_size, member_alignment);

			u32 member_size = (
				(member_element_count - 1) * align_size(aligned_member_single_element_size, min_pack_alignment)
				+ member_single_element_size
			);

			offset = align_size(offset, member_alignment);

			result.alignment = align_size(result.alignment, member_alignment);

			argument_member.offset = offset;

			offset += member_size;
		}

		result.size = align_size(offset, result.alignment);

		register_size(
			name,
			result.size
		);
		register_alignment(
			name,
			result.alignment
		);
		register_type_class(
			name,
			E_nsl_type_class::STRUCTURE
		);
		register_element_count(
			name,
			1
		);
		register_element_format(
			name,
			E_nsl_element_format::NONE
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
	G_string A_nsl_output_language::register_slot_space_macro(const G_string& name) {

		return "NSL_REGISTER_SLOT_SPACE_" + name;
	}

	b8 A_nsl_output_language::is_support(E_nsl_feature feature) {

		return true;
	}

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	eastl::optional<G_string> A_nsl_output_language::descriptor_heap_getter_to_string(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const F_nsl_ast_tree& tree,
		const G_string& index_str
	)
	{
		return eastl::nullopt;
	}
#endif



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

		result += "#define NSL_PRE_SHADER_KEYWORDS_NUM_THREADS(X, Y, Z) [numthreads(X, Y, Z)]\n";
		result += "#define NSL_PRE_SHADER_KEYWORDS_OUTPUT_TOPOLOGY(X) [outputtopology(X)]\n";

		result += G_string("#define pure_struct struct\n");

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

		for(u32 i = 0; i < NRHI_NSL_MAX_SV_TARGET_COUNT; ++i)
		{
			result += G_string("#define SV_TARGET_" + G_to_string(i) + " SV_Target" + G_to_string(i) + "\n");
		}

		result += G_string("#define SV_INSTANCE_ID SV_InstanceID\n");
		result += G_string("#define SV_PRIMITIVE_ID SV_PrimitiveID\n");
		result += G_string("#define SV_VERTEX_ID SV_VertexID\n");
		result += G_string("#define SV_DEPTH SV_Depth\n");

		result += G_string("#define SV_DISPATCH_THREAD_ID SV_DispatchThreadID\n");
		result += G_string("#define SV_GROUP_THREAD_ID SV_GroupThreadID\n");
		result += G_string("#define SV_GROUP_INDEX SV_GroupIndex\n");
		result += G_string("#define SV_GROUP_ID SV_GroupID\n");
		result += G_string("#define SV_VIEW_ID SV_ViewID\n");
		result += G_string("#define SV_CULL_PRIMITIVE SV_CullPrimitive\n");

		result += G_string("#define NSL_GLUE_INTERNAL(A, B) A##B\n");
		result += G_string("#define NSL_GLUE(A, B) NSL_GLUE_INTERNAL(A, B)\n");

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
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RasterizerOrderedBuffer");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RasterizerOrderedByteAddressBuffer");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RasterizerOrderedStructuredBuffer");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RasterizerOrderedTexture1D");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RasterizerOrderedTexture1DArray");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RasterizerOrderedTexture2D");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RasterizerOrderedTexture2DArray");
		name_manager_p->template T_register_name<FE_nsl_name_types::RESOURCE_TYPE>("RasterizerOrderedTexture3D");

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

		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_InstanceID");
		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_PrimitiveID");
		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_VertexID");
		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_Depth");

		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_DispatchThreadID");
		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_GroupThreadID");
		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_GroupIndex");
		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_GroupID");
		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_ViewID");
		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_CullPrimitive");

		name_manager_p->register_name("SV_POSITION", "SV_Position");
		name_manager_p->register_name("SV_TARGET", "SV_Target");

		for(u32 i = 0; i < NRHI_NSL_MAX_SV_TARGET_COUNT; ++i)
		{
			name_manager_p->register_name("SV_TARGET_" + G_to_string(i), "SV_Target" + G_to_string(i));
		}

		name_manager_p->register_name("SV_INSTANCE_ID", "SV_InstanceID");
		name_manager_p->register_name("SV_PRIMITIVE_ID", "SV_PrimitiveID");
		name_manager_p->register_name("SV_VERTEX_ID", "SV_VertexID");
		name_manager_p->register_name("SV_DEPTH", "SV_Depth");

		name_manager_p->register_name("SV_DISPATCH_THREAD_ID", "SV_DispatchThreadID");
		name_manager_p->register_name("SV_GROUP_THREAD_ID", "SV_GroupThreadID");
		name_manager_p->register_name("SV_GROUP_INDEX", "SV_GroupIndex");
		name_manager_p->register_name("SV_GROUP_ID", "SV_GroupID");
		name_manager_p->register_name("SV_VIEW_ID", "SV_ViewID");
		name_manager_p->register_name("SV_CULL_PRIMITIVE", "SV_CullPrimitive");

		data_type_manager_p->register_semantic(
			"SV_Position",
			F_nsl_semantic_info("float4")
		);
		data_type_manager_p->register_semantic(
			"SV_Target",
			F_nsl_semantic_info("float4")
		);

		data_type_manager_p->register_semantic(
			"SV_InstanceID",
			F_nsl_semantic_info("uint")
		);
		data_type_manager_p->register_semantic(
			"SV_PrimitiveID",
			F_nsl_semantic_info("uint")
		);
		data_type_manager_p->register_semantic(
			"SV_VertexID",
			F_nsl_semantic_info("uint")
		);
		data_type_manager_p->register_semantic(
			"SV_Depth",
			F_nsl_semantic_info("float")
		);

		data_type_manager_p->register_semantic(
			"SV_DispatchThreadID",
			F_nsl_semantic_info("uint3")
		);
		data_type_manager_p->register_semantic(
			"SV_GroupThreadID",
			F_nsl_semantic_info("uint3")
		);
		data_type_manager_p->register_semantic(
			"SV_GroupIndex",
			F_nsl_semantic_info("uint")
		);
		data_type_manager_p->register_semantic(
			"SV_GroupID",
			F_nsl_semantic_info("uint3")
		);
		data_type_manager_p->register_semantic(
			"SV_ViewID",
			F_nsl_semantic_info("uint3")
		);
		data_type_manager_p->register_semantic(
			"SV_CullPrimitive",
			F_nsl_semantic_info("bool")
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
		G_string sampler_state_register_slot_space_macro = register_slot_space_macro(sampler_state.first);

		G_string type;

		if(sampler_state.second.is_comparison_state)
		{
			type = "SamplerComparisonState";
		}
		else
		{
			type = "SamplerState";
		}

		result += (
			G_string("\n#ifdef ")
			+ sampler_state_register_slot_macro
			+ "\n"

			+ "#ifdef NSL_HLSL_SUPPORT_REGISTER_SLOT_SPACE\n"
			+ "#define NSL_REGISTER_SPACE_"
			+ sampler_state.first
			+ " , NSL_GLUE(space,"
			+ sampler_state_register_slot_space_macro
			+ ")\n"
			+ "#else\n"
			+ "#define NSL_REGISTER_SPACE_"
			+ sampler_state.first
			+ "\n"
			+ "#endif\n"

			+ "#define NSL_REGISTER_"
			+ sampler_state.first
			+ " register(NSL_GLUE(s,"
			+ sampler_state_register_slot_macro
			+ ")"
			+ " NSL_REGISTER_SPACE_"
			+ sampler_state.first
			+ ")\n"

			+ type
			+ " "
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

		for(const auto& argument_member : structure.second.argument_members)
		{
			b8 is_semantic = data_type_manager_p->is_name_has_semantic_info(argument_member.argument.type_selection.name);

			G_string semantic_option;

			if(is_semantic) {

				const auto& semantic_info = data_type_manager_p->semantic_info(argument_member.argument.type_selection.name);

				semantic_option = ": " + semantic_info.target_binding;
			}

			auto argument_type_str_opt = data_type_selection_to_string(
				translation_unit_p,
				argument_member.argument.type_selection
			);
			if(!argument_type_str_opt)
				return eastl::nullopt;

			argument_member_declarations += (
				argument_type_str_opt.value()
				+ " "
				+ argument_member.argument.name
			);

			if(argument_member.argument.is_array)
			{
				argument_member_declarations += "[";
				argument_member_declarations += G_to_string(argument_member.argument.count);
				argument_member_declarations += "]";
			}

			argument_member_declarations += semantic_option;

			argument_member_declarations += ";\n";
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

			b8 is_semantic = data_type_manager_p->is_name_has_semantic_info(data_param.argument.type_selection.name);

			G_string semantic_option;

			if(is_semantic) {

				const auto& semantic_info = data_type_manager_p->semantic_info(data_param.argument.type_selection.name);

				semantic_option = ": " + semantic_info.target_binding;
			}

			auto argument_type_str_opt = data_type_selection_to_string(
				translation_unit_p,
				data_param.argument.type_selection
			);
			if(!argument_type_str_opt)
				return eastl::nullopt;

			if(!is_first_data_param) {

				data_param_declarations += ",\n";
			}
			is_first_data_param = false;

			if(data_param.is_out && data_param.is_in)
				data_param_declarations += "inout ";
			if(data_param.is_out && !data_param.is_in)
				data_param_declarations += "out ";

			if(flag_is_has(data_param.flags, E_nsl_data_param_flag::INDICES))
				data_param_declarations += "indices ";

			if(flag_is_has(data_param.flags, E_nsl_data_param_flag::VERTICES))
				data_param_declarations += "vertices ";

			if(flag_is_has(data_param.flags, E_nsl_data_param_flag::PRIMITIVES))
				data_param_declarations += "primitives ";

			if(flag_is_has(data_param.flags, E_nsl_data_param_flag::PAYLOAD))
				data_param_declarations += "payload ";

			//
			{
				auto it = data_param.argument.config_map.find("max_records");
				if(it != data_param.argument.config_map.end())
				{
					auto& reader = it->second;

					auto value_opt = reader.read_i32(0);
					if(!value_opt)
						return eastl::nullopt;

					data_param_declarations += "[MaxRecords(" + G_to_string((value_opt.value())) + ")] ";
				}
			}

			//
			{
				auto it = data_param.argument.config_map.find("max_records_shared_with");
				if(it != data_param.argument.config_map.end())
				{
					auto& reader = it->second;

					auto value_opt = reader.read_string(0);
					if(!value_opt)
						return eastl::nullopt;

					data_param_declarations += "[MaxRecordsSharedWith(" + value_opt.value() + ")] ";
				}
			}

			//
			{
				auto it = data_param.argument.config_map.find("node_array_size");
				if(it != data_param.argument.config_map.end())
				{
					auto& reader = it->second;

					auto value_opt = reader.read_i32(0);
					if(!value_opt)
						return eastl::nullopt;

					data_param_declarations += "[NodeArraySize(" + G_to_string((value_opt.value())) + ")] ";
				}
			}

			//
			{
				auto it = data_param.argument.config_map.find("node_array_size");
				if(it != data_param.argument.config_map.end())
				{
					auto& reader = it->second;

					auto value_opt = reader.read_i32(0);
					if(!value_opt)
						return eastl::nullopt;

					data_param_declarations += "[NodeArraySize(" + G_to_string((value_opt.value())) + ")] ";
				}
			}

			//
			{
				auto it = data_param.argument.config_map.find("unbounded_sparse_nodes");
				if(it != data_param.argument.config_map.end())
				{
					data_param_declarations += "[UnboundedSparseNodes] ";
				}
			}

			//
			{
				auto it = data_param.argument.config_map.find("allow_sparse_nodes");
				if(it != data_param.argument.config_map.end())
				{
					data_param_declarations += "[AllowSparseNodes] ";
				}
			}

			//
			{
				auto it = data_param.argument.config_map.find("globally_coherent");
				if(it != data_param.argument.config_map.end())
				{
					data_param_declarations += "globallycoherent ";
				}
			}

			//
			{
				auto it = data_param.argument.config_map.find("node_array_size");
				if(it != data_param.argument.config_map.end())
				{
					auto& reader = it->second;

					auto value_opt = reader.read_i32(0);
					if(!value_opt)
						return eastl::nullopt;

					data_param_declarations += "[NodeArraySize(" + G_to_string((value_opt.value())) + ")] ";
				}
			}

			//
			{
				auto it = data_param.argument.config_map.find("node_id");
				if(it != data_param.argument.config_map.end())
				{
					auto& reader = it->second;

					F_node_id node_id;

					{
						auto value_opt = reader.read_string(0);
						if(!value_opt)
							return eastl::nullopt;

						node_id.name = value_opt.value();
					}

					if(reader.info_trees().size() > 1)
					{
						auto value_opt = reader.read_i32(0);
						if(!value_opt)
							return eastl::nullopt;

						node_id.index = value_opt.value();
					}

					data_param_declarations += "[NodeID(" + node_id.name + ", " + G_to_string(node_id.index) + ")] ";
				}
			}

			data_param_declarations += (
				argument_type_str_opt.value()
				+ " "
				+ data_param.argument.name
			);

			if(data_param.argument.is_array)
			{
				data_param_declarations += "[";
				data_param_declarations += G_to_string(data_param.argument.count);
				data_param_declarations += "]";
			}

			data_param_declarations += semantic_option;
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
		case E_nsl_resource_type_class::SHADER_RESOURCE:
			register_type = 't';
			break;
		case E_nsl_resource_type_class::UNORDERED_ACCESS:
			register_type = 'u';
			break;
		case E_nsl_resource_type_class::CONSTANT_BUFFER:
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

			if(type_arg_count)
			{
				if(
					(resource.second.type_args[0] == "StructuredBuffer")
					|| (resource.second.type_args[0] == "RWStructuredBuffer")
				)
				{

				}
			}

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
		G_string resource_register_slot_space_macro = register_slot_space_macro(resource.first);

		G_string uniform_declarations;
		auto uniform_manager_p = shader_compiler_p()->uniform_manager_p();
		auto uniforms = resource.second.uniforms;
		{
			auto compare = [&](const G_string& a, const G_string& b)
			{
				const auto& uniform_info_a = uniform_manager_p->uniform_info(a);
				const auto& uniform_info_b = uniform_manager_p->uniform_info(b);

				return (uniform_info_a.creation_index < uniform_info_b.creation_index);
			};
			eastl::sort(uniforms.begin(), uniforms.end(), compare);
		}
		for(const auto& uniform : uniforms) {

			const auto& uniform_info = uniform_manager_p->uniform_info(uniform);

			uniform_declarations += uniform_info.type_selection.name + " " + uniform;

			if(uniform_info.is_array)
			{
				uniform_declarations += "[";
				uniform_declarations += G_to_string(uniform_info.count);
				uniform_declarations += "]";
			}

			uniform_declarations += ";\n";
		}

		G_string pre_def_content;

		if(flag_is_has(resource.second.flags, E_nsl_resource_flag::GLOBALLY_COHERENT))
			pre_def_content += "globallycoherent ";

		result += (
			G_string("\n#ifdef ")
			+ resource_register_slot_macro
			+ "\n"

			+ "#ifdef NSL_HLSL_SUPPORT_REGISTER_SLOT_SPACE\n"
			+ "#define NSL_REGISTER_SPACE_"
			+ resource.first
			+ " , NSL_GLUE(space,"
			+ resource_register_slot_space_macro
			+ ")\n"
			+ "#else\n"
			+ "#define NSL_REGISTER_SPACE_"
			+ resource.first
			+ "\n"
			+ "#endif\n"

			+ "#define NSL_REGISTER_"
			+ resource.first
			+ " register(NSL_GLUE("
		  	+ G_string(1, register_type)
			+ ","
			+ resource_register_slot_macro
			+ ")"
			+ " NSL_REGISTER_SPACE_"
			+ resource.first
			+ ")\n"
		);

		if(resource.second.type != "ConstantBuffer")
			result += (
				pre_def_content

				// resource type
				+ parsed_type
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
				pre_def_content

				// resource type
				+ "cbuffer "

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

	eastl::optional<G_string> A_nsl_output_hlsl::data_type_selection_to_string(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const F_nsl_data_type_selection& data_type_selection
	)
	{
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();
		if(data_type_manager_p->is_name_has_semantic_info(data_type_selection.name)) {

			const auto& semantic_info = data_type_manager_p->semantic_info(data_type_selection.name);

			return semantic_info.target_type;
		}

		G_string result = data_type_selection.name;

		u32 child_count = data_type_selection.childs.size();

		if(child_count)
		{
			result += "<";
		}

		for(u32 i = 0; i < child_count; ++i)
		{
			auto child_value_opt = data_type_selection_to_string(
				translation_unit_p,
				data_type_selection.childs[i]
			);
			if(!child_value_opt)
				return eastl::nullopt;

			result += child_value_opt.value();

			if(i < (child_count - 1))
			{
				result += ", ";
			}
		}

		if(child_count)
		{
			result += ">";
		}

		return eastl::move(result);
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

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "0");
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

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "0");
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

		name_manager_p->deregister_name("NSL_HLSL_MAJOR");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "5");

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "1");

		name_manager_p->register_name("NSL_HLSL_SUPPORT_REGISTER_SLOT_SPACE");
	}

	eastl::optional<G_string> F_nsl_output_hlsl_5_1::src_header()
	{
		auto result = F_nsl_output_hlsl_5::src_header().value();

		result += "#define NSL_HLSL_SUPPORT_REGISTER_SLOT_SPACE\n";

		return eastl::move(result);
	}

	eastl::optional<G_string> F_nsl_output_hlsl_5_1::resource_to_string(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const F_nsl_resource& resource
	) {
		G_string sub_name_keyword;

		const auto& resource_info = resource.second;

		if(resource_info.is_array)
			for(u32 i = 0; i < resource_info.dimension_count; ++i) {

				u32 dimension_size = resource_info.dimension_sizes[i];

				sub_name_keyword += "[";
				if(dimension_size != NCPP_U32_MAX)
				{
					sub_name_keyword += G_to_string(dimension_size);
				}
				sub_name_keyword += "]";
			}

		return resource_to_string_with_customization(
			translation_unit_p,
			resource,
			sub_name_keyword
		);
	}



	F_nsl_output_hlsl_6_0::F_nsl_output_hlsl_6_0(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		F_nsl_output_hlsl_5_1(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl_6_0::~F_nsl_output_hlsl_6_0() {
	}

	void F_nsl_output_hlsl_6_0::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->deregister_name("NSL_HLSL_MAJOR");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "6");

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "0");

		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("uint64_t");
		name_manager_p->register_name("u64", "uint64_t");
		data_type_manager_p->register_size("uint64_t", 8);
		data_type_manager_p->register_alignment("uint64_t", 8);
		data_type_manager_p->register_primitive_data_type("uint64_t", E_nsl_primitive_data_type::U64);
		data_type_manager_p->register_type_class("uint64_t", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_element_format("uint64_t", E_nsl_element_format::UINT_64);
		data_type_manager_p->register_element_count("uint64_t", 1);
	}

	eastl::optional<G_string> F_nsl_output_hlsl_6_0::src_header()
	{
		auto result = F_nsl_output_hlsl_5_1::src_header().value();

		result += "#define u64 uint64_t\n";

		return eastl::move(result);
	}



	F_nsl_output_hlsl_6_1::F_nsl_output_hlsl_6_1(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		F_nsl_output_hlsl_6_0(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl_6_1::~F_nsl_output_hlsl_6_1() {
	}

	void F_nsl_output_hlsl_6_1::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->deregister_name("NSL_HLSL_MAJOR");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "6");

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "1");
	}



	F_nsl_output_hlsl_6_2::F_nsl_output_hlsl_6_2(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		F_nsl_output_hlsl_6_1(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl_6_2::~F_nsl_output_hlsl_6_2() {
	}

	void F_nsl_output_hlsl_6_2::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->deregister_name("NSL_HLSL_MAJOR");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "6");

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "2");

		name_manager_p->template T_register_name<FE_nsl_name_types::DATA_TYPE>("uint16_t");
		name_manager_p->register_name("u16", "uint16_t");
		data_type_manager_p->register_size("uint16_t", 2);
		data_type_manager_p->register_alignment("uint16_t", 2);
		data_type_manager_p->register_primitive_data_type("uint16_t", E_nsl_primitive_data_type::U16);
		data_type_manager_p->register_type_class("uint16_t", E_nsl_type_class::PRIMITIVE);
		data_type_manager_p->register_element_format("uint16_t", E_nsl_element_format::UINT_16);
		data_type_manager_p->register_element_count("uint16_t", 1);
	}

	eastl::optional<G_string> F_nsl_output_hlsl_6_2::src_header()
	{
		auto result = F_nsl_output_hlsl_6_1::src_header().value();

		result += "#define u16 uint16_t\n";

		return eastl::move(result);
	}



	F_nsl_output_hlsl_6_3::F_nsl_output_hlsl_6_3(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		F_nsl_output_hlsl_6_2(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl_6_3::~F_nsl_output_hlsl_6_3() {
	}

	void F_nsl_output_hlsl_6_3::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->deregister_name("NSL_HLSL_MAJOR");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "6");

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "3");
	}



	F_nsl_output_hlsl_6_4::F_nsl_output_hlsl_6_4(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		F_nsl_output_hlsl_6_3(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl_6_4::~F_nsl_output_hlsl_6_4() {
	}

	void F_nsl_output_hlsl_6_4::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->deregister_name("NSL_HLSL_MAJOR");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "6");

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "4");
	}



	F_nsl_output_hlsl_6_5::F_nsl_output_hlsl_6_5(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		F_nsl_output_hlsl_6_4(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl_6_5::~F_nsl_output_hlsl_6_5() {
	}

	void F_nsl_output_hlsl_6_5::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->deregister_name("NSL_HLSL_MAJOR");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "6");

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "5");
	}



	F_nsl_output_hlsl_6_6::F_nsl_output_hlsl_6_6(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		F_nsl_output_hlsl_6_5(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl_6_6::~F_nsl_output_hlsl_6_6() {
	}

	void F_nsl_output_hlsl_6_6::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->deregister_name("NSL_HLSL_MAJOR");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "6");

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "6");
	}

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	eastl::optional<G_string> F_nsl_output_hlsl_6_6::descriptor_heap_getter_to_string(
		TKPA_valid<F_nsl_translation_unit> translation_unit_p,
		const F_nsl_ast_tree& tree,
		const G_string& index_str
	)
	{
		auto descriptor_heap_getter_object_p = tree.object_implementation.attached_object_p.T_cast<F_nsl_descriptor_heap_getter_object>();

		ED_descriptor_heap_type descriptor_heap_type = descriptor_heap_getter_object_p->descriptor_heap_type();
		E_nsl_resource_type resource_type = descriptor_heap_getter_object_p->resource_type();

		G_string getter;

		NRHI_ENUM_SWITCH(
			descriptor_heap_type,
			NRHI_ENUM_CASE(
				ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
				getter = "ResourceDescriptorHeap[" + index_str + "]";
			)
			NRHI_ENUM_CASE(
				ED_descriptor_heap_type::SAMPLER,
				getter = "SamplerDescriptorHeap[" + index_str + "]";
			)
		);

		switch (resource_type)
		{
		case E_nsl_resource_type::NONE:
			return getter;
			break;
		case E_nsl_resource_type::CONSTANT_BUFFER:
		{
			return  (
				G_string("(")
				+ "(ConstantBuffer<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::BUFFER:
		{
			return  (
				G_string("(")
				+ "(Buffer<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::BYTE_ADDRESS_BUFFER:
		{
			return  (
				G_string("(")
				+ "(ByteAddressBuffer)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::STRUCTURED_BUFFER:
		{
			return  (
				G_string("(")
				+ "(StructuredBuffer<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::TEXTURE_1D:
		{
			return  (
				G_string("(")
				+ "(Texture1D<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::TEXTURE_1D_ARRAY:
		{
			return  (
				G_string("(")
				+ "(Texture1DArray<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::TEXTURE_2D:
		{
			return  (
				G_string("(")
				+ "(Texture2D<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::TEXTURE_2D_ARRAY:
		{
			return  (
				G_string("(")
				+ "(Texture2DArray<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::TEXTURE_3D:
		{
			return  (
				G_string("(")
				+ "(Texture3D<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::TEXTURE_CUBE:
		{
			return  (
				G_string("(")
				+ "(TextureCube<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::TEXTURE_CUBE_ARRAY:
		{
			return  (
				G_string("(")
				+ "(TextureCubeArray<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RW_BUFFER:
		{
			return  (
				G_string("(")
				+ "(RWBuffer<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RW_BYTE_ADDRESS_BUFFER:
		{
			return  (
				G_string("(")
				+ "(RWByteAddressBuffer)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RW_STRUCTURED_BUFFER:
		{
			return  (
				G_string("(")
				+ "(RWStructuredBuffer<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RW_TEXTURE_1D:
		{
			return  (
				G_string("(")
				+ "(RWTexture1D<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RW_TEXTURE_1D_ARRAY:
		{
			return  (
				G_string("(")
				+ "(RWTexture1DArray<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RW_TEXTURE_2D:
		{
			return  (
				G_string("(")
				+ "(RWTexture2D<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RW_TEXTURE_2D_ARRAY:
		{
			return  (
				G_string("(")
				+ "(RWTexture2DArray<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RW_TEXTURE_3D:
		{
			return  (
				G_string("(")
				+ "(RWTexture3D<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RASTERIZER_ORDERED_BUFFER:
		{
			return  (
				G_string("(")
				+ "(RasterizerOrderedBuffer<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RASTERIZER_ORDERED_BYTE_ADDRESS_BUFFER:
		{
			return  (
				G_string("(")
				+ "(RasterizerOrderedByteAddressBuffer)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RASTERIZER_ORDERED_STRUCTURED_BUFFER:
		{
			return  (
				G_string("(")
				+ "(RasterizerOrderedStructuredBuffer<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_1D:
		{
			return  (
				G_string("(")
				+ "(RasterizerOrderedTexture1D<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_1D_ARRAY:
		{
			return  (
				G_string("(")
				+ "(RasterizerOrderedTexture1DArray<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_2D:
		{
			return  (
				G_string("(")
				+ "(RasterizerOrderedTexture2D<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_2D_ARRAY:
		{
			return  (
				G_string("(")
				+ "(RasterizerOrderedTexture2DArray<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		case E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_3D:
		{
			return  (
				G_string("(")
				+ "(RasterizerOrderedTexture3D<"
				+ tree.object_implementation.bodies[0].content
				+ ">)"
				+ getter
				+ ")"
			);
			break;
		}
		}

		return eastl::nullopt;
	}
#endif



	F_nsl_output_hlsl_6_7::F_nsl_output_hlsl_6_7(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		F_nsl_output_hlsl_6_6(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl_6_7::~F_nsl_output_hlsl_6_7() {
	}

	void F_nsl_output_hlsl_6_7::register_data_types_internal() {

		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->deregister_name("NSL_HLSL_MAJOR");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "6");

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "7");
	}



	F_nsl_output_hlsl_6_8::F_nsl_output_hlsl_6_8(
		TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
		E_nsl_output_language output_language_as_enum
	) :
		F_nsl_output_hlsl_6_7(shader_compiler_p, output_language_as_enum)
	{
		register_data_types_internal();
	}
	F_nsl_output_hlsl_6_8::~F_nsl_output_hlsl_6_8() {
	}

	void F_nsl_output_hlsl_6_8::register_data_types_internal()
	{
		auto name_manager_p = shader_compiler_p()->name_manager_p();
		auto data_type_manager_p = shader_compiler_p()->data_type_manager_p();

		name_manager_p->deregister_name("NSL_HLSL_MAJOR");
		name_manager_p->register_name("NSL_HLSL_MAJOR", "6");

		name_manager_p->deregister_name("NSL_HLSL_MINOR");
		name_manager_p->register_name("NSL_HLSL_MINOR", "8");

		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_DISPATCH_GRID_3D");
		data_type_manager_p->register_semantic(
			"SV_DISPATCH_GRID_3D",
			F_nsl_semantic_info{
				.target_type = "uint3",
				.target_binding = "SV_DispatchGrid"
			}
		);

		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_DISPATCH_GRID_2D");
		data_type_manager_p->register_semantic(
			"SV_DISPATCH_GRID_2D",
			F_nsl_semantic_info{
				.target_type = "uint2",
				.target_binding = "SV_DispatchGrid"
			}
		);

		name_manager_p->template T_register_name<FE_nsl_name_types::SEMANTIC>("SV_DISPATCH_GRID_1D");
		data_type_manager_p->register_semantic(
			"SV_DISPATCH_GRID_1D",
			F_nsl_semantic_info{
				.target_type = "uint",
				.target_binding = "SV_DispatchGrid"
			}
		);

		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("DispatchNodeInputRecord");
		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("RWDispatchNodeInputRecord");
		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("GroupNodeInputRecords");
		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("RWGroupNodeInputRecords");
		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("ThreadNodeInputRecord");
		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("RWThreadNodeInputRecord");
		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("EmptyNodeInput");

		data_type_manager_p->register_virtual("DispatchNodeInputRecord");
		data_type_manager_p->register_virtual("RWDispatchNodeInputRecord");
		data_type_manager_p->register_virtual("GroupNodeInputRecords");
		data_type_manager_p->register_virtual("RWGroupNodeInputRecords");
		data_type_manager_p->register_virtual("ThreadNodeInputRecord");
		data_type_manager_p->register_virtual("RWThreadNodeInputRecord");
		data_type_manager_p->register_virtual("EmptyNodeInput");

		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("NodeOutput");
		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("NodeOutputArray");
		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("EmptyNodeOutput");
		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("EmptyNodeOutputArray");

		data_type_manager_p->register_virtual("NodeOutput");
		data_type_manager_p->register_virtual("NodeOutputArray");
		data_type_manager_p->register_virtual("EmptyNodeOutput");
		data_type_manager_p->register_virtual("EmptyNodeOutputArray");

		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("ThreadNodeOutputRecords");
		name_manager_p->T_register_name<FE_nsl_name_types::DATA_TYPE>("GroupNodeOutputRecords");

		data_type_manager_p->register_virtual("ThreadNodeOutputRecords");
		data_type_manager_p->register_virtual("GroupNodeOutputRecords");
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
		name_to_resource_type_class_map_["ConstantBuffer"] = E_nsl_resource_type_class::CONSTANT_BUFFER;
		name_to_resource_type_class_map_["Buffer"] = E_nsl_resource_type_class::SHADER_RESOURCE;
		name_to_resource_type_class_map_["ByteAddressBuffer"] = E_nsl_resource_type_class::SHADER_RESOURCE;
		name_to_resource_type_class_map_["StructuredBuffer"] = E_nsl_resource_type_class::SHADER_RESOURCE;
		name_to_resource_type_class_map_["Texture1D"] = E_nsl_resource_type_class::SHADER_RESOURCE;
		name_to_resource_type_class_map_["Texture1DArray"] = E_nsl_resource_type_class::SHADER_RESOURCE;
		name_to_resource_type_class_map_["Texture2D"] = E_nsl_resource_type_class::SHADER_RESOURCE;
		name_to_resource_type_class_map_["Texture2DArray"] = E_nsl_resource_type_class::SHADER_RESOURCE;
		name_to_resource_type_class_map_["Texture3D"] = E_nsl_resource_type_class::SHADER_RESOURCE;
		name_to_resource_type_class_map_["TextureCube"] = E_nsl_resource_type_class::SHADER_RESOURCE;
		name_to_resource_type_class_map_["TextureCubeArray"] = E_nsl_resource_type_class::SHADER_RESOURCE;
		name_to_resource_type_class_map_["RWBuffer"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RWByteAddressBuffer"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RWStructuredBuffer"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RWTexture1D"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RWTexture1DArray"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RWTexture2D"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RWTexture2DArray"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RWTexture3D"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RasterizerOrderedBuffer"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RasterizerOrderedByteAddressBuffer"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RasterizerOrderedStructuredBuffer"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RasterizerOrderedTexture1D"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RasterizerOrderedTexture1DArray"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RasterizerOrderedTexture2D"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RasterizerOrderedTexture2DArray"] = E_nsl_resource_type_class::UNORDERED_ACCESS;
		name_to_resource_type_class_map_["RasterizerOrderedTexture3D"] = E_nsl_resource_type_class::UNORDERED_ACCESS;

		// setup name_to_resource_type_map_
		name_to_resource_type_map_["ConstantBuffer"] = E_nsl_resource_type::CONSTANT_BUFFER;
		name_to_resource_type_map_["Buffer"] = E_nsl_resource_type::BUFFER;
		name_to_resource_type_map_["ByteAddressBuffer"] = E_nsl_resource_type::BYTE_ADDRESS_BUFFER;
		name_to_resource_type_map_["StructuredBuffer"] = E_nsl_resource_type::STRUCTURED_BUFFER;
		name_to_resource_type_map_["Texture1D"] = E_nsl_resource_type::TEXTURE_1D;
		name_to_resource_type_map_["Texture1DArray"] = E_nsl_resource_type::TEXTURE_1D_ARRAY;
		name_to_resource_type_map_["Texture2D"] = E_nsl_resource_type::TEXTURE_2D;
		name_to_resource_type_map_["Texture2DArray"] = E_nsl_resource_type::TEXTURE_2D_ARRAY;
		name_to_resource_type_map_["Texture3D"] = E_nsl_resource_type::TEXTURE_3D;
		name_to_resource_type_map_["TextureCube"] = E_nsl_resource_type::TEXTURE_CUBE;
		name_to_resource_type_map_["TextureCubeArray"] = E_nsl_resource_type::TEXTURE_CUBE_ARRAY;
		name_to_resource_type_map_["RWBuffer"] = E_nsl_resource_type::RW_BUFFER;
		name_to_resource_type_map_["RWByteAddressBuffer"] = E_nsl_resource_type::RW_BYTE_ADDRESS_BUFFER;
		name_to_resource_type_map_["RWStructuredBuffer"] = E_nsl_resource_type::RW_STRUCTURED_BUFFER;
		name_to_resource_type_map_["RWTexture1D"] = E_nsl_resource_type::RW_TEXTURE_1D;
		name_to_resource_type_map_["RWTexture1DArray"] = E_nsl_resource_type::RW_TEXTURE_1D_ARRAY;
		name_to_resource_type_map_["RWTexture2D"] = E_nsl_resource_type::RW_TEXTURE_2D;
		name_to_resource_type_map_["RWTexture2DArray"] = E_nsl_resource_type::RW_TEXTURE_2D_ARRAY;
		name_to_resource_type_map_["RWTexture3D"] = E_nsl_resource_type::RW_TEXTURE_3D;
		name_to_resource_type_map_["RasterizerOrderedBuffer"] = E_nsl_resource_type::RASTERIZER_ORDERED_BUFFER;
		name_to_resource_type_map_["RasterizerOrderedByteAddressBuffer"] = E_nsl_resource_type::RASTERIZER_ORDERED_BYTE_ADDRESS_BUFFER;
		name_to_resource_type_map_["RasterizerOrderedStructuredBuffer"] = E_nsl_resource_type::RASTERIZER_ORDERED_STRUCTURED_BUFFER;
		name_to_resource_type_map_["RasterizerOrderedTexture1D"] = E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_1D;
		name_to_resource_type_map_["RasterizerOrderedTexture1DArray"] = E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_1D_ARRAY;
		name_to_resource_type_map_["RasterizerOrderedTexture2D"] = E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_2D;
		name_to_resource_type_map_["RasterizerOrderedTexture2DArray"] = E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_2D_ARRAY;
		name_to_resource_type_map_["RasterizerOrderedTexture3D"] = E_nsl_resource_type::RASTERIZER_ORDERED_TEXTURE_3D;
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

		result.creation_index = next_uniform_creation_index_;
		++next_uniform_creation_index_;

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

			if(shader_object_p->type() == ED_shader_type::COMPUTE)
		    {
				result.type = ED_pipeline_state_type::COMPUTE;
				break;
			}
			if(
				(shader_object_p->type() == ED_shader_type::VERTEX)
#ifdef NRHI_DRIVER_SUPPORT_AMPLIFICATION_SHADER
				|| (shader_object_p->type() == ED_shader_type::AMPLIFICATION)
#endif
#ifdef NRHI_DRIVER_SUPPORT_MESH_SHADER
				|| (shader_object_p->type() == ED_shader_type::MESH)
#endif
			)
			{
				result.type = ED_pipeline_state_type::GRAPHICS;
				break;
			}
		}

		return std::move(result);
	}



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	F_nsl_root_signature_manager::F_nsl_root_signature_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_root_signature_manager::~F_nsl_root_signature_manager() {
	}

	F_nsl_root_signature_info F_nsl_root_signature_manager::process_root_signature_info(const G_string& name, const F_nsl_root_signature_info& root_signature_info) {

		F_nsl_root_signature_info result = root_signature_info;

		return std::move(result);
	}
#endif



#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
	F_nsl_work_graph_manager::F_nsl_work_graph_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_work_graph_manager::~F_nsl_work_graph_manager() {
	}

	F_nsl_work_graph_info F_nsl_work_graph_manager::process_work_graph_info(const G_string& name, const F_nsl_work_graph_info& work_graph_info) {

		F_nsl_work_graph_info result = work_graph_info;

		return std::move(result);
	}
#endif



#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
	F_nsl_state_object::F_nsl_state_object(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_state_object::~F_nsl_state_object() {
	}
#endif



	F_nsl_submodule_manager::F_nsl_submodule_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) :
		shader_compiler_p_(shader_compiler_p)
	{
	}
	F_nsl_submodule_manager::~F_nsl_submodule_manager() {
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
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		auto root_signature_manager_p = shader_compiler_p_->root_signature_manager_p();
#endif
#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
		auto work_graph_manager_p = shader_compiler_p_->work_graph_manager_p();
#endif

		auto& name_to_primitive_data_type_map = data_type_manager_p->name_to_primitive_data_type_map();
		auto& name_to_semantic_info_map = data_type_manager_p->name_to_semantic_info_map();
		auto& name_to_structure_info_map = data_type_manager_p->name_to_structure_info_map();
		auto& name_to_shader_object_p_map = shader_manager_p->name_to_shader_object_p_map();
		auto& name_to_pipeline_state_info_map = pipeline_state_manager_p->name_to_pipeline_state_info_map();
		auto& name_to_sampler_state_info_map = sampler_state_manager_p->name_to_sampler_state_info_map();
		auto& name_to_resource_info_map = resource_manager_p->name_to_resource_info_map();
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		auto& name_to_root_signature_info_map = root_signature_manager_p->name_to_root_signature_info_map();
#endif
#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
		auto& name_to_work_graph_info_map = work_graph_manager_p->name_to_work_graph_info_map();
#endif

		TG_unordered_map<G_string, u32> name_to_type_index_map;

		// semantic data types
		{
			for(auto& it : name_to_semantic_info_map)
			{
				const auto& semantic_info = it.second;

				auto primitive_data_type = name_to_primitive_data_type_map[semantic_info.target_type];

				name_to_type_index_map[it.first] = reflection.types.size();

				reflection.types.push_back(
					F_nsl_type_reflection {
						.name = it.first,
						.type_class = E_nsl_type_class::PRIMITIVE,
						.primitive_data_type = primitive_data_type,
						.semantic = {
							.name = it.first,
							.binding = semantic_info.target_binding
						},

						.size = data_type_manager_p->size(semantic_info.target_type),
						.alignment = data_type_manager_p->alignment(semantic_info.target_type)
					}
				);
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

					structure.placed_data_arguments.push_back(
						F_nsl_placed_data_argument_reflection {
							.argument = F_nsl_data_argument_reflection {
								.name = argument.name,
								.type_selection = argument.type_selection,
								.count = argument.count,
								.is_array = argument.is_array
							},
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

				F_nsl_shader_reflection shader_reflection = {
					.name = shader_object_p->name(),
					.type = shader_object_p->type(),

					.thread_group_size = shader_object_p->thread_group_size()
				};

				const auto& data_params = shader_object_p->data_params();
				u32 data_param_count = data_params.size();
				for(u32 j = 0; j < data_param_count; ++j) {

					auto& data_param = data_params[j];
					auto& argument = data_param.argument;

					shader_reflection.data_params.push_back(
						F_nsl_data_param_reflection {
							.argument = F_nsl_data_argument_reflection {
								.name = argument.name,
								.type_selection = argument.type_selection,
								.count = argument.count,
								.is_array = argument.is_array
							},
							.flags = data_param.flags
						}
					);
				}

				reflection.shaders[i] = shader_reflection;

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
					.root_signature_selection = pipeline_state_info.root_signature_selection,
					.shader_indices = shader_indices

				};

				++it;
			}
		}

		// root signatures
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		{
			u32 root_signature_count = name_to_root_signature_info_map.size();

			reflection.root_signatures.resize(root_signature_count);

			auto it = name_to_root_signature_info_map.begin();

			for(u32 i = 0; i < root_signature_count; ++i) {

				auto& root_signature_info = it->second;

				reflection.root_signatures[i] = F_nsl_root_signature_reflection {
					.name = it->first,
					.desc = root_signature_info.desc
				};

				++it;
			}
		}
		reflection.global_root_signature_selection = root_signature_manager_p->global_root_signature_selection();
		reflection.local_root_signature_selection = root_signature_manager_p->local_root_signature_selection();
#endif

		// work graphs
#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
		{
			u32 work_graph_count = name_to_work_graph_info_map.size();

			reflection.work_graphs.resize(work_graph_count);

			auto it = name_to_work_graph_info_map.begin();

			for(u32 i = 0; i < work_graph_count; ++i) {

				auto& work_graph_info = it->second;

				reflection.work_graphs[i] = F_nsl_work_graph_reflection {
					.name = it->first,
					.desc = work_graph_info.desc
				};

				++it;
			}
		}
#endif

		// state object config
#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
		reflection.state_object_config = shader_compiler_p()->state_object_p()->config();
#endif

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
					.is_static = sampler_state_info.is_static,
					.is_comparison_state = sampler_state_info.is_comparison_state,
					.actual_slots = sampler_state_info.actual_slots,
					.actual_slot_spaces = sampler_state_info.actual_slot_spaces

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
				TG_vector<F_nsl_placed_data_argument_reflection> placed_data_arguments;
				placed_data_arguments.reserve(data_arg_count);
				for(u32 j = 0; j < data_arg_count; ++j)
				{
					const G_string& uniform_name = resource_info.uniforms[j];

					auto& uniform_info = uniform_manager_p->uniform_info(uniform_name);

					placed_data_arguments.push_back(
						F_nsl_placed_data_argument_reflection{
							.argument = F_nsl_data_argument_reflection {
								.name = uniform_name,
								.type_selection = uniform_info.type_selection,
								.count = uniform_info.count,
								.is_array = uniform_info.is_array
							},
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
					.actual_slot_spaces = resource_info.actual_slot_spaces,
					.placed_data_arguments = std::move(placed_data_arguments),
					.sort_uniforms = resource_info.sort_uniforms,
					.constant_size = resource_info.constant_size,
					.flags = resource_info.flags

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
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		root_signature_manager_p_(customizer.root_signature_manager_creator(NCPP_KTHIS())),
#endif
#ifdef NRHI_DRIVER_SUPPORT_WORK_GRAPHS
		work_graph_manager_p_(customizer.work_graph_manager_creator(NCPP_KTHIS())),
#endif
#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
		state_object_p_(customizer.state_object_creator(NCPP_KTHIS())),
#endif
		submodule_manager_p_(customizer.submodule_manager_creator(NCPP_KTHIS())),
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
		case E_nsl_output_language::HLSL_6_0:
			return TU<F_nsl_output_hlsl_6_0>()(
				NCPP_KTHIS()
			);
		case E_nsl_output_language::HLSL_6_1:
			return TU<F_nsl_output_hlsl_6_1>()(
				NCPP_KTHIS()
			);
		case E_nsl_output_language::HLSL_6_2:
			return TU<F_nsl_output_hlsl_6_2>()(
				NCPP_KTHIS()
			);
		case E_nsl_output_language::HLSL_6_3:
			return TU<F_nsl_output_hlsl_6_3>()(
				NCPP_KTHIS()
			);
		case E_nsl_output_language::HLSL_6_4:
			return TU<F_nsl_output_hlsl_6_4>()(
				NCPP_KTHIS()
			);
		case E_nsl_output_language::HLSL_6_5:
			return TU<F_nsl_output_hlsl_6_5>()(
				NCPP_KTHIS()
			);
		case E_nsl_output_language::HLSL_6_6:
			return TU<F_nsl_output_hlsl_6_6>()(
				NCPP_KTHIS()
			);
		case E_nsl_output_language::HLSL_6_7:
			return TU<F_nsl_output_hlsl_6_7>()(
				NCPP_KTHIS()
			);
		case E_nsl_output_language::HLSL_6_8:
			return TU<F_nsl_output_hlsl_6_8>()(
				NCPP_KTHIS()
			);
		default:
			return null;
		}
	}

	eastl::optional<F_nsl_compiled_result> F_nsl_shader_compiler::compile(
		const G_string& raw_src_content,
		const G_string& class_name,
		E_nsl_output_language output_language_enum,
		const G_string& abs_path,
		const TG_vector<eastl::pair<G_string, G_string>>& macros
	) {
		//
		output_language_p_ = create_output_language(output_language_enum);

		// bind macros
		{
			for(auto& macro : macros)
				name_manager_p_->register_name(macro.first, macro.second);
		}

		//
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

		//
		auto compile_result_opt = translation_unit_compiler_p_->compile();
		if(!compile_result_opt)
		{
			is_compiled_ = true;
			is_compile_success_ = false;
			return eastl::nullopt;
		}

		compile_result_opt.value().class_name = class_name;

		is_compiled_ = true;
		is_compile_success_ = static_cast<b8>(compile_result_opt);

		// bind macros
		{
			G_string macros_str;
			for(auto& macro : macros)
				macros_str += "#define " + macro.first + " " + macro.second + "\n";

			compile_result_opt.value().src_content = macros_str + compile_result_opt.value().src_content;
		}

		return compile_result_opt;
	}

}