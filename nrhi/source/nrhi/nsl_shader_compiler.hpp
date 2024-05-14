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

	class F_nsl_shader_compiler;
	class F_nsl_shader_module_loader;
	class F_nsl_translation_unit;
	class F_nsl_translation_unit_manager;
	class F_nsl_object;
	class F_nsl_object_type;
	class F_nsl_object_manager;



	struct F_nsl_error {
		G_string description;
		sz location = 0;
		sz raw_location = 0xFFFFFFFFFFFFFFFF;
	};
	using F_nsl_error_stack = TG_stack<F_nsl_error>;

	struct F_nsl_plain_text {

		G_string content;
		sz begin_location = 0;
		sz end_location = 0;

	};

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

	struct F_nsl_object_implementation_body {

		G_string content;

		sz begin_location = 0;
		sz end_location = 0;

	};

	struct F_nsl_object_implementation {

		G_string keyword;
		G_string name;

		sz begin_location = 0;
		sz end_location = 0;
		sz begin_keyword_location = 0;
		sz end_keyword_location = 0;
		sz begin_name_location = 0;
		sz end_name_location = 0;

		TG_vector<F_nsl_object_implementation_body> bodies;

	};

	struct NRHI_API F_nsl_str_state {

		b8 value = false;
		b8 value_1 = false; // for '
		b8 value_2 = false; // for "

		b8 next_value = false;
		b8 next_value_1 = false; // for '
		b8 next_value_2 = false; // for "

		b8 prev_value = false;
		b8 prev_value_1 = false; // for '
		b8 prev_value_2 = false; // for "

		b8 is_special_character = false;

	public:
		void begin_check(char c);
		void end_check();

	};

	enum class E_nsl_ast_tree_type {

		PLAIN_TEXT = 0x1,
		USE = 0x2,
		INFO_TREE = 0x3,
		OBJECT_IMPLEMENTATION = 0x4

	};
	struct F_nsl_ast_tree {

		E_nsl_ast_tree_type type = E_nsl_ast_tree_type::PLAIN_TEXT;
		F_nsl_use use;
		F_nsl_plain_text plain_text;
		F_nsl_info_tree info_tree;
		F_nsl_object_implementation object_implementation;

		sz begin_location = 0;
		sz end_location = 0;

		TG_vector<F_nsl_ast_tree> childs;

	};
	using F_nsl_ast_tree_recursive_build_functor = eastl::function<
		eastl::optional<TG_vector<F_nsl_ast_tree>>(
			const G_string& src_content,
			sz location_offset_to_save,
			const F_nsl_ast_tree& tree,
			sz index,
			F_nsl_error_stack* error_stack_p
		)
	>;
	using F_nsl_ast_tree_result_functor = eastl::function<
		G_string(const G_string& src_content, const F_nsl_ast_tree& tree, sz index)
	>;
	struct F_nsl_ast_tree_try_build_input {

		const G_string& src_content;
		F_nsl_error_stack* error_stack_p;

		const F_nsl_str_state& str_state;

		sz current_location = 0;

		sz begin_location = 0;
		sz end_location = 0;

		sz location_offset_to_save = 0;

	};
	struct F_nsl_ast_tree_try_build_result {

		sz begin_location = 0;
		sz end_location = 0;
		TG_vector<F_nsl_ast_tree> trees;

	};
	using F_nsl_ast_tree_try_build_functor = eastl::function<
		eastl::optional<F_nsl_ast_tree_try_build_result>(
			const F_nsl_ast_tree_try_build_input& input
		)
	>;



	class NRHI_API H_nsl_utilities {

	public:
		static b8 is_variable_name_character(char c);

	public:
		static eastl::optional<TG_vector<F_nsl_info_tree>> build_info_trees(
			const G_string& src_content,
			sz location_offset_to_save = 0,
			F_nsl_error_stack* error_stack_p = 0
		);

	public:
		static eastl::optional<F_nsl_ast_tree_try_build_result> try_build_use(
			const F_nsl_ast_tree_try_build_input& input
		);
		static eastl::optional<F_nsl_ast_tree_try_build_result> try_build_object_implementation(
			const G_string& keyword,
			b8 is_object_name_required,
			sz min_object_body_count,
			sz max_object_body_count,
			const F_nsl_ast_tree_try_build_input& input
		);
		static F_nsl_ast_tree_try_build_functor make_try_build_object_implementation_functor(
			const G_string& keyword,
			b8 is_object_name_required = true,
			sz min_object_body_count = 0,
			sz max_object_body_count = 2
		);
		static eastl::optional<TG_vector<F_nsl_ast_tree>> build_ast_trees(
			const G_string& src_content,
			const TG_vector<F_nsl_ast_tree_try_build_functor>& try_build_functors,
			const F_nsl_ast_tree_recursive_build_functor& resursive_build_functor,
			sz location_offset_to_save = 0,
			F_nsl_error_stack* error_stack_p = 0
		);
		static eastl::optional<TG_vector<F_nsl_ast_tree>> build_ast_trees(
			const G_string& src_content,
			const F_nsl_ast_tree_recursive_build_functor& resursive_build_functor,
			sz location_offset_to_save = 0,
			F_nsl_error_stack* error_stack_p = 0
		);
		static eastl::optional<G_string> apply_ast_trees(
			const G_string& src_content,
			const TG_vector<F_nsl_ast_tree>& trees,
			const F_nsl_ast_tree_result_functor& result_functor,
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
		TG_vector<sz> raw_locations;
		G_string abs_path;

	};

#define NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(error_stack_p, location, ...) if(error_stack_p) error_stack_p->push({__VA_ARGS__, location})
#define NSL_PUSH_ERROR_TO_SRC_INTERNAL(src, location, ...) src.error_stack.push({__VA_ARGS__, location})

#define NSL_VERTEX_SHADER_DEFINITION_MACRO_NAME "NSL_VERTEX_SHADER"
#define NSL_PIXEL_SHADER_DEFINITION_MACRO_NAME "NSL_PIXEL_SHADER"
#define NSL_COMPUTE_SHADER_DEFINITION_MACRO_NAME "NSL_COMPUTE_SHADER"



	class NRHI_API F_nsl_shader_module_loader {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }



	public:
		F_nsl_shader_module_loader(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_shader_module_loader();

	public:
		NCPP_OBJECT(F_nsl_shader_module_loader);

	public:
		virtual eastl::optional<G_string> load_src_content(
			TKPA_valid<F_nsl_translation_unit> from_unit_p,
			const G_string& path
		);
		virtual TU<F_nsl_translation_unit> load(
			TKPA_valid<F_nsl_translation_unit> from_unit_p,
			const G_string& path
		);

	};



	class NRHI_API F_nsl_object {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;
		G_string name_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }
		NCPP_FORCE_INLINE const G_string& name() const noexcept { return name_; }



	public:
		F_nsl_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_object();

	public:
		NCPP_OBJECT(F_nsl_object);

	};



	class NRHI_API F_nsl_object_type {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;
		G_string name_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }
		NCPP_FORCE_INLINE const G_string& name() const noexcept { return name_; }



	public:
		F_nsl_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			const G_string& name
		);
		virtual ~F_nsl_object_type();

	public:
		virtual TU<F_nsl_object> create_object(const G_string& object_name);

	};



	class NRHI_API F_nsl_object_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

		TG_unordered_map<G_string, TU<F_nsl_object_type>> type_p_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }



	public:
		F_nsl_object_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_object_manager();

	public:
		NCPP_OBJECT(F_nsl_object_manager);

	public:
		TK_valid<F_nsl_object_type> obtain_type(const G_string& type);

	};



	class NRHI_API F_nsl_error_group {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

		G_string abs_path_;

		F_nsl_error_stack stack_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const G_string& abs_path() const noexcept { return abs_path_; }

		NCPP_FORCE_INLINE F_nsl_error_stack& stack() noexcept { return stack_; }
		NCPP_FORCE_INLINE const F_nsl_error_stack& stack() const noexcept { return stack_; }



	public:
		F_nsl_error_group(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			const G_string& abs_path
		);
		virtual ~F_nsl_error_group();

	public:
		NCPP_OBJECT(F_nsl_error_group);

	};



	class NRHI_API F_nsl_error_storage {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

		TG_stack<TU<F_nsl_error_group>> group_p_stack_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const TG_stack<TU<F_nsl_error_group>>& group_p_stack() const noexcept { return group_p_stack_; }



	public:
		F_nsl_error_storage(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_error_storage();

	public:
		NCPP_OBJECT(F_nsl_error_storage);

	public:
		TK_valid<F_nsl_error_group> optain_group(const G_string& abs_path);

	};



	class NRHI_API F_nsl_translation_unit {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

		G_string raw_src_content_;
		G_string abs_path_;
		F_nsl_preprocessed_src preprocessed_src_;

		TK_valid<F_nsl_error_group> error_group_p_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const G_string& raw_src_content() const noexcept { return raw_src_content_; }
		NCPP_FORCE_INLINE const G_string& abs_path() const noexcept { return abs_path_; }
		NCPP_FORCE_INLINE const F_nsl_preprocessed_src& preprocessed_src() const noexcept { return preprocessed_src_; }

		NCPP_FORCE_INLINE TKPA_valid<F_nsl_error_group> error_group_p() const noexcept { return error_group_p_; }



	public:
		F_nsl_translation_unit(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			const G_string& raw_src_content,
			const G_string& abs_path,
			const F_nsl_preprocessed_src& preprocessed_src,
			TKPA_valid<F_nsl_error_group> error_group_p
		);
		virtual ~F_nsl_translation_unit();

	public:
		NCPP_OBJECT(F_nsl_translation_unit);

	};



	class NRHI_API F_nsl_translation_unit_manager {

	public:
		using F_unit_p_map = TG_unordered_map<G_string, TU<F_nsl_translation_unit>>;



	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }



	public:
		F_nsl_translation_unit_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_translation_unit_manager();

	public:
		NCPP_OBJECT(F_nsl_translation_unit_manager);

	protected:
		virtual TU<F_nsl_translation_unit> create_unit_instance(
			const G_string& raw_src_content,
			const G_string& abs_path,
			const F_nsl_preprocessed_src& preprocessed_src,
			TKPA_valid<F_nsl_error_group> error_group_p
		);

	public:
		virtual TU<F_nsl_translation_unit> create_unit(
			const G_string& raw_src_content,
			const G_string& abs_path
		);

	};



	class NRHI_API F_nsl_shader_compiler {

	private:
		TU<F_nsl_shader_module_loader> module_loader_p_;
		TU<F_nsl_translation_unit_manager> translation_unit_manager_p_;
		TU<F_nsl_error_storage> error_storage_p_;
		TU<F_nsl_object_manager> object_manager_p_;

	public:
		NCPP_FORCE_INLINE TK_valid<F_nsl_shader_module_loader> module_loader_p() const noexcept { return NCPP_FOH_VALID(module_loader_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_translation_unit_manager> translation_unit_manager_p() const noexcept { return NCPP_FOH_VALID(translation_unit_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_error_storage> error_storage_p() const noexcept { return NCPP_FOH_VALID(error_storage_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_object_manager> object_manager_p() const noexcept { return NCPP_FOH_VALID(object_manager_p_); }



	public:
		F_nsl_shader_compiler();
		F_nsl_shader_compiler(
			TU<F_nsl_shader_module_loader>&& module_loader_p,
			TU<F_nsl_translation_unit_manager>&& translation_unit_manager_p,
			TU<F_nsl_error_storage>&& error_storage_p,
			TU<F_nsl_object_manager>&& object_manager_p
		);
		virtual ~F_nsl_shader_compiler();

	public:
		NCPP_OBJECT(F_nsl_shader_compiler);

	};

}
