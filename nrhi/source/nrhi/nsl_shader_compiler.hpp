#pragma once

/** @file nrhi/nsl_shader_compiler.hpp
*
*   Implement nsl shader compiler.
*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#pragma region Includes

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/prerequisites.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/shader_compiler_base.hpp>
#include <nrhi/shader_class_base.hpp>

#pragma endregion



namespace nrhi {

	class A_shader;
	class A_shader_class;



	struct F_nsl_include_blob {

	};



	class NRHI_API H_nsl_utilities {

	public:
		struct F_error {
			G_string src_content;
			G_string description;
		};
		using F_errors = TG_vector<F_error>;

		struct NRHI_API F_str_state {

			b8 value = false;
			b8 value_1 = false; // for '
			b8 value_2 = false; // for "

			b8 next_value = false;
			b8 next_value_1 = false; // for '
			b8 next_value_2 = false; // for "

			b8 prev_value = false;
			b8 prev_value_1 = false; // for '
			b8 prev_value_2 = false; // for "

		public:
			void begin_check(char c);
			void end_check();

		};

		struct NRHI_API F_info_tree {

			G_string name;
			TG_vector<F_info_tree> childs;
			G_string child_src_content;

		};



	public:
		static b8 is_variable_name_character(char c);

	public:
		static eastl::optional<TG_vector<F_info_tree>> build_info_trees(
			const G_string& src_content,
			F_errors* errors_p = 0
		);

	public:
		struct F_function_macro_use {
			sz begin_location = 0;
			sz end_location = 0;
			G_string arg;
		};
		using F_function_macro_result_functor = eastl::function<
			G_string(const F_function_macro_use& use)
		>;
		static TG_vector<F_function_macro_use> find_function_macro_uses(
			const G_string& src_content,
			const G_string& macro_name
		);
		static G_string apply_function_macro_uses(
			const G_string& src_content,
			const G_string& macro_name,
			const F_function_macro_result_functor& macro_result_functor,
			const TG_vector<F_function_macro_use>& uses
		);

	public:
		using F_variable_macro_use = sz;
		static TG_vector<F_variable_macro_use> find_variable_macro_uses(
			const G_string& src_content,
			const G_string& macro_name
		);
		static G_string apply_variable_macro_uses(
			const G_string& src_content,
			const G_string& macro_name,
			const G_string& macro_result,
			const TG_vector<F_variable_macro_use>& uses
		);

	public:
		static G_string clear_space_head_tail(const G_string& src_content);
		static G_string clear_space_head(const G_string& src_content);
		static G_string clear_space_tail(const G_string& src_content);

	public:
		static G_string remove_comments(const G_string& src_content);

	};



	class NRHI_API H_nsl_tools {

	public:
		struct F_preprocessed_src {

			G_string content;

		};
		struct F_kernel_definition {

			H_nsl_utilities::F_function_macro_use use;
			H_nsl_utilities::F_info_tree info_tree;
			TG_vector<H_nsl_utilities::F_info_tree> macro_definition_trees;
			E_shader_type shader_type = E_shader_type::NONE;

		};
		static eastl::optional<TG_vector<F_kernel_definition>> find_kernel_definitions(
			const F_preprocessed_src& src,
			H_nsl_utilities::F_errors* errors_p = 0
		);

	};

#define NSL_PUSH_ERROR_INTERNAL(src_content, ...) if(errors_p) errors_p->push_back({src_content, __VA_ARGS__})

#define NSL_VERTEX_SHADER_DEFINITION_MACRO_NAME "NSL_VERTEX_SHADER"
#define NSL_PIXEL_SHADER_DEFINITION_MACRO_NAME "NSL_PIXEL_SHADER"
#define NSL_COMPUTE_SHADER_DEFINITION_MACRO_NAME "NSL_COMPUTE_SHADER"



	class NRHI_API F_nsl_shader_compiler {

	public:
		F_nsl_shader_compiler();
		~F_nsl_shader_compiler();

	public:
		NCPP_DISABLE_COPY(F_nsl_shader_compiler);

	public:
		virtual eastl::optional<G_string> apply_kernel_definition(
			const H_nsl_tools::F_preprocessed_src& src,
			const H_nsl_tools::F_kernel_definition& kernel_definition,
			H_nsl_utilities::F_errors* errors_p = 0
		);

	public:
		virtual eastl::optional<H_nsl_tools::F_preprocessed_src> include_src(
			const G_string& from_abs_path,
			const G_string& to_abs_path,
			H_nsl_utilities::F_errors* errors_p = 0
		);
		virtual eastl::optional<H_nsl_tools::F_preprocessed_src> preprocess_src(
			const G_string& src_content,
			const G_string& abs_path,
			H_nsl_utilities::F_errors* errors_p = 0
		);

	};

}
