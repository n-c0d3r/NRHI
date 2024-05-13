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



	struct F_nsl_error {
		G_string description;
		sz location = 0;
		sz raw_location = 0xFFFFFFFFFFFFFFFF;
	};
	using F_nsl_error_stack = TG_stack<F_nsl_error>;

	struct F_nsl_use {

		G_string name;
		G_string arg;

		sz begin_location = 0;
		sz end_location = 0;
		sz begin_name_location = 0;
		sz end_name_location = 0;
		sz begin_arg_location = 0;
		sz end_arg_location = 0;

	};
	using F_nsl_use_result_functor = eastl::function<
		G_string(const F_nsl_use& use, sz index)
	>;

	struct NRHI_API F_nsl_info_tree {

		G_string name;
		TG_vector<F_nsl_info_tree> childs;
		G_string child_src_content;

		sz begin_location = 0;
		sz end_location = 0;
		sz begin_name_location = 0;
		sz end_name_location = 0;
		sz begin_childs_location = 0;
		sz end_childs_location = 0;

	};



	class NRHI_API H_nsl_utilities {

	public:

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



	public:
		static b8 is_variable_name_character(char c);

	public:
		static eastl::optional<TG_vector<F_nsl_info_tree>> build_info_trees(
			const G_string& src_content,
			sz location_offset_to_save = 0,
			F_nsl_error_stack* error_stack_p = 0
		);

	public:
		static eastl::optional<TG_vector<F_nsl_use>> find_uses(
			const G_string& src_content,
			F_nsl_error_stack* error_stack_p = 0
		);
		static eastl::optional<G_string> apply_uses(
			const G_string& src_content,
			const TG_vector<F_nsl_use>& uses,
			const F_nsl_use_result_functor& result_functor,
			F_nsl_error_stack* error_stack_p = 0
		);

	public:
		static G_string clear_space_head_tail(const G_string& src_content);
		static G_string clear_space_head(const G_string& src_content);
		static G_string clear_space_tail(const G_string& src_content);

	public:
		static eastl::optional<TG_pack<G_string, TG_vector<sz>>> remove_comments(
			const G_string& src_content,
			F_nsl_error_stack* error_stack_p = 0
		);

	};



	struct F_nsl_preprocessed_src {

		G_string content;
		TG_vector<sz> locations;
		G_string abs_path;
		mutable F_nsl_error_stack error_stack;

	};

#define NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(error_stack_p, location, ...) if(error_stack_p) error_stack_p->push({__VA_ARGS__, location})
#define NSL_PUSH_ERROR_TO_SRC_INTERNAL(src, location, ...) src.error_stack.push({__VA_ARGS__, location})

#define NSL_VERTEX_SHADER_DEFINITION_MACRO_NAME "NSL_VERTEX_SHADER"
#define NSL_PIXEL_SHADER_DEFINITION_MACRO_NAME "NSL_PIXEL_SHADER"
#define NSL_COMPUTE_SHADER_DEFINITION_MACRO_NAME "NSL_COMPUTE_SHADER"



	class NRHI_API F_nsl_shader_header_loader {

	public:
		F_nsl_shader_header_loader();
		~F_nsl_shader_header_loader();

	public:
		NCPP_DISABLE_COPY(F_nsl_shader_header_loader);

	public:
		virtual eastl::optional<G_string> load(const G_string& path);

	};



	class NRHI_API F_nsl_shader_compiler {

	private:
		TU<F_nsl_shader_header_loader> header_loader_p_;

	public:
		NCPP_FORCE_INLINE TK_valid<F_nsl_shader_header_loader> header_loader_p() const noexcept { return NCPP_FOH_VALID(header_loader_p_); }



	public:
		F_nsl_shader_compiler();
		F_nsl_shader_compiler(TU<F_nsl_shader_header_loader>&& header_loader_p);
		~F_nsl_shader_compiler();

	public:
		NCPP_DISABLE_COPY(F_nsl_shader_compiler);

	public:
		eastl::optional<F_nsl_preprocessed_src> include_src(
			const F_nsl_preprocessed_src& current_src,
			const G_string& path,
			F_nsl_error_stack* error_stack_p
		);

	public:
		virtual eastl::optional<F_nsl_preprocessed_src> preprocess_src(
			const G_string& src_content,
			const G_string& abs_path,
			F_nsl_error_stack* error_stack_p = 0
		);

	};

}
