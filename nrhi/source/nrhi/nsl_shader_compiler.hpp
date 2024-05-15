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
	class F_nsl_shader_module_manager;
	class F_nsl_translation_unit;
	class F_nsl_translation_unit_manager;
	class A_nsl_object;
	class A_nsl_object_type;
	class F_nsl_object_manager;
	class A_nsl_section_storage;



	enum class E_nsl_output_language {

		HLSL = 0x1

	};

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

		TK<A_nsl_object> attached_object_p;

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
		INFO_TREE = 0x3,
		OBJECT_IMPLEMENTATION = 0x4

	};
	struct F_nsl_ast_tree {

		E_nsl_ast_tree_type type = E_nsl_ast_tree_type::PLAIN_TEXT;
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
			TG_vector<F_nsl_ast_tree>& trees,
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

	enum class E_nsl_name_type {

		DATA_TYPE,
		RESOURCE

	};

	struct F_nsl_context {

		TK<A_nsl_object> parent_object_p;

	};

	struct F_nsl_macro_definitions_section {};
	struct F_nsl_types_section {};
	struct F_nsl_resources_section {};
	struct F_nsl_shader_entry_points_section {};



	class NRHI_API H_nsl_utilities {

	public:
		static b8 is_variable_name_character(char c);
		static b8 is_variable_name(const G_string& str);
		static b8 is_object_name_character(char c);
		static b8 is_object_name(const G_string& str);

	public:
		static eastl::optional<TG_vector<F_nsl_info_tree>> build_info_trees(
			const G_string& src_content,
			sz location_offset_to_save = 0,
			F_nsl_error_stack* error_stack_p = 0
		);

	public:
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

	public:
		static b8 is_plain_text_blank(const F_nsl_plain_text& plain_text);

	};



	struct F_nsl_preprocessed_src {

		G_string content;
		TG_vector<sz> raw_locations;
		G_string abs_path;

	};

#define NSL_PUSH_ERROR_TO_ERROR_STACK_INTERNAL(error_stack_p, location, ...) if(error_stack_p) (error_stack_p)->push({__VA_ARGS__, location})

#define NSL_VERTEX_SHADER_DEFINITION_MACRO_NAME "NSL_VERTEX_SHADER"
#define NSL_PIXEL_SHADER_DEFINITION_MACRO_NAME "NSL_PIXEL_SHADER"
#define NSL_COMPUTE_SHADER_DEFINITION_MACRO_NAME "NSL_COMPUTE_SHADER"



	class NRHI_API F_nsl_shader_module_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TG_unordered_map<G_string, TK<F_nsl_translation_unit>> abs_path_to_translation_unit_p_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }



	public:
		F_nsl_shader_module_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_shader_module_manager();

	public:
		NCPP_OBJECT(F_nsl_shader_module_manager);

	public:
		struct F_load_src_content_result {
			G_string abs_path;
		};
		virtual eastl::optional<F_load_src_content_result> load_src_content(
			TKPA_valid<F_nsl_translation_unit> from_unit_p,
			const G_string& path,
			const F_nsl_ast_tree& tree,
			G_string& out_src_content
		);
		virtual TK<F_nsl_translation_unit> load(
			TKPA_valid<F_nsl_translation_unit> from_unit_p,
			const G_string& path,
			const F_nsl_ast_tree& tree
		);

	};



	class NRHI_API A_nsl_object {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;
		TK_valid<A_nsl_object_type> type_p_;
		TK_valid<F_nsl_translation_unit> translation_unit_p_;
		G_string name_;

	public:
		sz tree_index = 0xFFFFFFFFFFFFFFFF;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }
		NCPP_FORCE_INLINE TKPA_valid<A_nsl_object_type> type_p() const noexcept { return type_p_; }
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_translation_unit> translation_unit_p() const noexcept { return translation_unit_p_; }
		NCPP_FORCE_INLINE const G_string& name() const noexcept { return name_; }



	protected:
		A_nsl_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);

	public:
		virtual ~A_nsl_object();

	public:
		NCPP_OBJECT(A_nsl_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			const G_string& src_content,
			sz location_offset_to_save,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		);

	};



	class NRHI_API A_nsl_object_type {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;
		G_string name_;
		b8 is_object_name_required_ = true;
		sz min_object_body_count_ = 0;
		sz max_object_body_count_ = 2;

		TG_vector<TU<A_nsl_object>> object_p_vector_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }
		NCPP_FORCE_INLINE const G_string& name() const noexcept { return name_; }
		NCPP_FORCE_INLINE b8 is_object_name_required() const noexcept { return is_object_name_required_; }
		NCPP_FORCE_INLINE sz min_object_body_count() const noexcept { return min_object_body_count_; }
		NCPP_FORCE_INLINE sz max_object_body_count() const noexcept { return max_object_body_count_; }

		NCPP_FORCE_INLINE const TG_vector<TU<A_nsl_object>>& object_p_vector() const noexcept { return object_p_vector_; }



	protected:
		A_nsl_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			const G_string& name,
			b8 is_object_name_required = true,
			sz min_object_body_count = 0,
			sz max_object_body_count = 2
		);

	public:
		virtual ~A_nsl_object_type();

	public:
		NCPP_OBJECT(A_nsl_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) = 0;

	protected:
		TK<A_nsl_object> register_object(TU<A_nsl_object>&& object_p);

	};



	class NRHI_API F_nsl_import_object final : public A_nsl_object {

	public:
		F_nsl_import_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_import_object();

	public:
		NCPP_OBJECT(F_nsl_import_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			const G_string& src_content,
			sz location_offset_to_save,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_import_object_type final : public A_nsl_object_type {

	public:
		F_nsl_import_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_import_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_import_object_type);

	};



	class NRHI_API F_nsl_define_object final : public A_nsl_object {

	public:
		G_string target;



	public:
		F_nsl_define_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_define_object();

	public:
		NCPP_OBJECT(F_nsl_define_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			const G_string& src_content,
			sz location_offset_to_save,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_define_object_type final : public A_nsl_object_type {

	public:
		F_nsl_define_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_define_object_type();

	public:
		NCPP_OBJECT(F_nsl_define_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	};



	class NRHI_API F_nsl_if_object final : public A_nsl_object {

	public:
		G_string target;



	public:
		F_nsl_if_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_if_object();

	public:
		NCPP_OBJECT(F_nsl_if_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			const G_string& src_content,
			sz location_offset_to_save,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_if_object_type final : public A_nsl_object_type {

	public:
		F_nsl_if_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_if_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_if_object_type);

	};



	class NRHI_API F_nsl_elif_object final : public A_nsl_object {

	public:
		G_string target;



	public:
		F_nsl_elif_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_elif_object();

	public:
		NCPP_OBJECT(F_nsl_elif_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			const G_string& src_content,
			sz location_offset_to_save,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_elif_object_type final : public A_nsl_object_type {

	public:
		F_nsl_elif_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_elif_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_elif_object_type);

	};



	class NRHI_API F_nsl_else_object final : public A_nsl_object {

	public:
		G_string target;



	public:
		F_nsl_else_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_else_object();

	public:
		NCPP_OBJECT(F_nsl_else_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			const G_string& src_content,
			sz location_offset_to_save,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_else_object_type final : public A_nsl_object_type {

	public:
		F_nsl_else_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_else_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_else_object_type);

	};



	class NRHI_API F_nsl_alias_object final : public A_nsl_object {

	public:
		G_string target;



	public:
		F_nsl_alias_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_alias_object();

	public:
		NCPP_OBJECT(F_nsl_alias_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			const G_string& src_content,
			sz location_offset_to_save,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_alias_object_type final : public A_nsl_object_type {

	public:
		F_nsl_alias_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_alias_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_alias_object_type);

	};



	class NRHI_API F_nsl_object_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

		TG_unordered_map<G_string, TU<A_nsl_object_type>> type_p_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const TG_unordered_map<G_string, TU<A_nsl_object_type>>& type_p_map() const noexcept { return type_p_map_; }



	public:
		F_nsl_object_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_object_manager();

	public:
		NCPP_OBJECT(F_nsl_object_manager);

	public:
		TK_valid<A_nsl_object_type> register_type(TU<A_nsl_object_type>&& object_type_p);

		NCPP_FORCE_INLINE b8 is_has_type_p(const G_string& name) const noexcept {

			auto it = type_p_map_.find(name);

			return (it != type_p_map_.end());
		}
		NCPP_FORCE_INLINE TK_valid<A_nsl_object_type> type_p(const G_string& name) const noexcept {

			auto it = type_p_map_.find(name);

			NCPP_ASSERT(it != type_p_map_.end()) << "type " << T_cout_value(name) << " not found";

			return NCPP_FOH_VALID(it->second);
		}

		TG_vector<F_nsl_ast_tree_try_build_functor> ast_tree_try_build_functors();

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

		TG_vector<TK_valid<F_nsl_translation_unit>> dependency_p_vector_;

		TG_vector<F_nsl_ast_tree> ast_trees_;
		TU<A_nsl_section_storage> section_storage_p_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const G_string& raw_src_content() const noexcept { return raw_src_content_; }
		NCPP_FORCE_INLINE const G_string& abs_path() const noexcept { return abs_path_; }
		NCPP_FORCE_INLINE const F_nsl_preprocessed_src& preprocessed_src() const noexcept { return preprocessed_src_; }

		NCPP_FORCE_INLINE TKPA_valid<F_nsl_error_group> error_group_p() const noexcept { return error_group_p_; }

		NCPP_FORCE_INLINE const TG_vector<TK_valid<F_nsl_translation_unit>>& dependency_p_vector() const noexcept { return dependency_p_vector_; }

		NCPP_FORCE_INLINE TG_vector<F_nsl_ast_tree>& ast_trees() noexcept { return ast_trees_; }
		NCPP_FORCE_INLINE const TG_vector<F_nsl_ast_tree>& ast_trees() const noexcept { return ast_trees_; }

		NCPP_FORCE_INLINE TKPA_valid<A_nsl_section_storage> section_storage_p() noexcept {

			if(!section_storage_p_)
				section_storage_p_ = create_section_storage();
			return NCPP_FOH_VALID(section_storage_p_);
		}



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

	protected:
		virtual TU<A_nsl_section_storage> create_section_storage();

	public:
		void add_dependency(TK_valid<F_nsl_translation_unit> dependency_p);

	};



	class NRHI_API F_nsl_translation_unit_manager {

	public:
		using F_unit_p_map = TG_unordered_map<G_string, TU<F_nsl_translation_unit>>;



	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

		TG_vector<TU<F_nsl_translation_unit>> translation_unit_p_vector_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const TG_vector<TU<F_nsl_translation_unit>>& translation_unit_p_vector() const noexcept { return translation_unit_p_vector_; }



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
		virtual TK<F_nsl_translation_unit> create_unit(
			const G_string& raw_src_content,
			const G_string& abs_path
		);

	};



	class NRHI_API F_nsl_translation_unit_compiler {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TK<F_nsl_translation_unit> main_unit_p_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE TKPA<F_nsl_translation_unit> main_unit_p() const noexcept { return main_unit_p_; }



	public:
		F_nsl_translation_unit_compiler(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_translation_unit_compiler();

	public:
		NCPP_OBJECT(F_nsl_translation_unit_compiler);

	private:
		b8 sort_internal();
		b8 read_internal();
		b8 apply_internal();
		b8 apply_macro_definitions_internal();
		b8 apply_types_internal();
		b8 apply_resources_internal();
		b8 apply_shader_entry_points_internal();

	protected:
		b8 compile_minimal();

	protected:
		b8 prepare_unit(TK_valid<F_nsl_translation_unit> unit_p);

	public:
		b8 prepare_units(
			const G_string& raw_src_content,
			const G_string& abs_path
		);
		eastl::optional<G_string> compile();

	public:
		eastl::optional<TG_vector<F_nsl_ast_tree>> parse(
			TK_valid<F_nsl_translation_unit> unit_p,
			const G_string& src_content,
			F_nsl_context& context,
			sz location_offset_to_safe = 0
		);
		eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			const G_string& src_content,
			sz location_offset_to_save,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		);

	};



	class NRHI_API A_nsl_section_storage {

	private:
		TK_valid<F_nsl_translation_unit> translation_unit_p_;

	protected:
		TG_unordered_map<u64, G_string> section_map_;
		TG_vector<u64> section_ids_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_translation_unit> translation_unit_p() const noexcept { return translation_unit_p_; }

		NCPP_FORCE_INLINE const TG_unordered_map<u64, G_string>& section_map() const noexcept { return section_map_; }



	public:
		A_nsl_section_storage(TKPA_valid<F_nsl_translation_unit> translation_unit_p);
		virtual ~A_nsl_section_storage();

	public:
		NCPP_OBJECT(A_nsl_section_storage);

	public:
		NCPP_FORCE_INLINE b8 is_has_section(u64 id) const {

			return (section_map_.find(id) != section_map_.end());
		}
		NCPP_FORCE_INLINE G_string& section(u64 id) {

			auto it = section_map_.find(id);

			NCPP_ASSERT(it != section_map_.end()) << "section with id " << T_cout_value(id) << " not found";

			return it->second;
		}
		NCPP_FORCE_INLINE const G_string& section(u64 id) const {

			auto it = section_map_.find(id);

			NCPP_ASSERT(it != section_map_.end()) << "section with id " << T_cout_value(id) << " not found";

			return it->second;
		}

	public:
		template<typename F_section__>
		NCPP_FORCE_INLINE b8 T_is_has_section(u64 id) const {

			return is_has_section(T_type_hash_code<F_section__>);
		}
		template<typename F_section__>
		NCPP_FORCE_INLINE G_string& T_section(u64 id) {

			return section(T_type_hash_code<F_section__>);
		}
		template<typename F_section__>
		NCPP_FORCE_INLINE const G_string& T_section(u64 id) const {

			return section(T_type_hash_code<F_section__>);
		}

	public:
		G_string combine();

	};



	template<typename... F_sections__>
	class NRHI_API TF_nsl_section_storage : public A_nsl_section_storage {

	public:
		TF_nsl_section_storage(TKPA_valid<F_nsl_translation_unit> translation_unit_p) :
			A_nsl_section_storage(translation_unit_p)
		{
			section_map_ = {
				{
					T_type_hash_code<F_sections__>,
					G_string("")
				}...
			};
			section_ids_ = TG_vector<u64>({ T_type_hash_code<F_sections__>... });
		}
		virtual ~TF_nsl_section_storage() {
		}

	public:
		NCPP_OBJECT(TF_nsl_section_storage);

	};

	using F_nsl_section_storage = TF_nsl_section_storage<
		F_nsl_macro_definitions_section,
		F_nsl_types_section,
		F_nsl_resources_section,
		F_nsl_shader_entry_points_section
	>;



	class NRHI_API F_nsl_name_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TG_set<G_string> name_set_;
		TG_unordered_map<G_string, G_string> name_to_target_map_;
		TG_unordered_map<G_string, E_nsl_name_type> name_to_name_type_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const TG_set<G_string>& name_set() const noexcept { return name_set_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, G_string>& name_to_target_map() const noexcept { return name_to_target_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, E_nsl_name_type>& name_to_name_type_map() const noexcept { return name_to_name_type_map_; }



	public:
		F_nsl_name_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_name_manager();

	public:
		NCPP_OBJECT(F_nsl_name_manager);

	public:
		NCPP_FORCE_INLINE b8 is_name_has_target(const G_string& name) const {

			auto it = name_to_target_map_.find(name);

			return (it != name_to_target_map_.end());
		}
		NCPP_FORCE_INLINE b8 is_name_has_type(const G_string& name) const {

			auto it = name_to_name_type_map_.find(
				target(name)
			);

			return (it != name_to_name_type_map_.end());
		}
		NCPP_FORCE_INLINE G_string target(const G_string& name) const {

			G_string result = "";
			auto it = name_to_target_map_.find(name);

			NCPP_ASSERT(it != name_to_target_map_.end()) << T_cout_value(name) << " is not registered";

			while(it != name_to_target_map_.end()) {

				result = it->second;
				it = name_to_target_map_.find(result);
			}

			return result;
		}
		NCPP_FORCE_INLINE E_nsl_name_type name_type(const G_string& name) const {

			auto it = name_to_name_type_map_.find(
				target(name)
			);

			NCPP_ASSERT(it != name_to_name_type_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_name(const G_string& name, const G_string& target) {

			NCPP_ASSERT(name_to_target_map_.find(name) == name_to_target_map_.end()) << T_cout_value(name) << " already exists";
			NCPP_ASSERT(is_name_has_target(target)) << T_cout_value(target) << " is not registered";

			name_to_target_map_[name] = target;
		}
		NCPP_FORCE_INLINE void register_name(const G_string& name, E_nsl_name_type name_type) {

			NCPP_ASSERT(name_to_name_type_map_.find(name) == name_to_name_type_map_.end()) << T_cout_value(name) << " already exists";

			name_to_name_type_map_[name] = name_type;
			name_to_target_map_[name] = name;
		}

	};



	class NRHI_API F_nsl_data_type_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TG_unordered_map<G_string, sz> name_to_size_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const TG_unordered_map<G_string, sz>& name_to_size_map() const noexcept { return name_to_size_map_; }



	public:
		F_nsl_data_type_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_data_type_manager();

	public:
		NCPP_OBJECT(F_nsl_data_type_manager);

	public:
		NCPP_FORCE_INLINE b8 is_name_has_size(const G_string& name) const {

			auto it = name_to_size_map_.find(name);

			return (it != name_to_size_map_.end());
		}
		NCPP_FORCE_INLINE sz size(const G_string& name) const {

			auto it = name_to_size_map_.find(name);

			NCPP_ASSERT(it != name_to_size_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_size(const G_string& name, sz size) {

			NCPP_ASSERT(name_to_size_map_.find(name) == name_to_size_map_.end()) << T_cout_value(name) << " already exists";

			name_to_size_map_[name] = size;
		}

	};



	class NRHI_API A_nsl_output_language {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

		E_nsl_output_language as_enum_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE E_nsl_output_language as_enum() const noexcept { return as_enum_; }



	protected:
		A_nsl_output_language(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language as_enum
		);

	public:
		virtual ~A_nsl_output_language();

	public:
		NCPP_OBJECT(A_nsl_output_language);

	};



	class NRHI_API F_nsl_output_hlsl : public A_nsl_output_language {

	public:
		F_nsl_output_hlsl(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_output_hlsl();

	public:
		NCPP_OBJECT(F_nsl_output_hlsl);

	private:
		void register_data_types_internal();

	};



	class NRHI_API F_nsl_resource_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }



	public:
		F_nsl_resource_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_resource_manager();

	public:
		NCPP_OBJECT(F_nsl_resource_manager);

	};



	class NRHI_API F_nsl_shader_compiler {

	private:
		TU<F_nsl_shader_module_manager> module_manager_p_;
		TU<F_nsl_translation_unit_manager> translation_unit_manager_p_;
		TU<F_nsl_translation_unit_compiler> translation_unit_compiler_p_;
		TU<F_nsl_error_storage> error_storage_p_;
		TU<F_nsl_object_manager> object_manager_p_;
		TU<F_nsl_name_manager> name_manager_p_;
		TU<F_nsl_data_type_manager> data_type_manager_p_;
		TU<F_nsl_resource_manager> resource_manager_p_;

		TU<A_nsl_output_language> output_language_p_;

	public:
		NCPP_FORCE_INLINE TK_valid<F_nsl_shader_module_manager> module_manager_p() const noexcept { return NCPP_FOH_VALID(module_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_translation_unit_manager> translation_unit_manager_p() const noexcept { return NCPP_FOH_VALID(translation_unit_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_translation_unit_compiler> translation_unit_compiler_p() const noexcept { return NCPP_FOH_VALID(translation_unit_compiler_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_error_storage> error_storage_p() const noexcept { return NCPP_FOH_VALID(error_storage_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_object_manager> object_manager_p() const noexcept { return NCPP_FOH_VALID(object_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_name_manager> name_manager_p() const noexcept { return NCPP_FOH_VALID(name_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_data_type_manager> data_type_manager_p() const noexcept { return NCPP_FOH_VALID(data_type_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_resource_manager> resource_manager_p() const noexcept { return NCPP_FOH_VALID(resource_manager_p_); }

		NCPP_FORCE_INLINE TK<A_nsl_output_language> output_language_p() const noexcept { return output_language_p_; }



	public:
		F_nsl_shader_compiler();
		F_nsl_shader_compiler(
			TU<F_nsl_shader_module_manager>&& module_manager_p,
			TU<F_nsl_translation_unit_manager>&& translation_unit_manager_p,
			TU<F_nsl_translation_unit_compiler>&& translation_unit_compiler_p,
			TU<F_nsl_error_storage>&& error_storage_p,
			TU<F_nsl_object_manager>&& object_manager_p,
			TU<F_nsl_name_manager>&& name_manager_p,
			TU<F_nsl_data_type_manager>&& data_type_manager_p,
			TU<F_nsl_resource_manager>&& resource_manager_p
		);
		virtual ~F_nsl_shader_compiler();

	public:
		NCPP_OBJECT(F_nsl_shader_compiler);

	protected:
		virtual TU<A_nsl_output_language> create_output_language();

	public:
		eastl::optional<G_string> compile(
			const G_string& raw_src_content,
			E_nsl_output_language output_language,
			const G_string& abs_path = ""
		);

	};

}
