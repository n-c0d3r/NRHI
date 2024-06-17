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
#include <nrhi/shader_blob_desc.hpp>
#include <nrhi/sampler_state_desc.hpp>
#include <nrhi/cull_mode.hpp>
#include <nrhi/fill_mode.hpp>
#include <nrhi/format.hpp>
#include <nrhi/depth_comparison_func.hpp>
#include <nrhi/primitive_topology.hpp>
#include <nrhi/pipeline_state_desc.hpp>

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

	using F_nsl_object_type_channel_mask = u32;
	using F_nsl_object_type_channel = u8;
	static constexpr F_nsl_object_type_channel_mask nsl_default_object_type_channel_mask = NCPP_U32_MAX;
	static constexpr F_nsl_object_type_channel_mask nsl_global_object_type_channel_mask = 0x1;
	static constexpr F_nsl_object_type_channel_mask nsl_function_body_object_type_channel_mask = 0x2;

	enum class E_nsl_element_format {

		FLOAT_64,
		UINT_64,
		SINT_64,
		TYPELESS_64,

		FLOAT_32,
		FLOAT_16,

		UNORM_16,
		UNORM_8,

		SNORM_16,
		SNORM_8,

		UINT_32,
		UINT_16,
		UINT_8,

		SINT_32,
		SINT_16,
		SINT_8,

		TYPELESS_32,
		TYPELESS_16,
		TYPELESS_8,

	};

	enum class E_nsl_semantic_input_class {

		PER_VERTEX,
		PER_INSTANCE

	};

	class NRHI_API F_nsl_info_tree_reader {

	private:
		TG_vector<F_nsl_info_tree> info_trees_;
		u32 location_offset_to_save_ = 0;
		F_nsl_error_stack* error_stack_p_ = 0;

		static b8 is_maps_setup_;
		static TG_map<G_string, b8> b8_str_to_value_map_;
		static TG_map<G_string, E_nsl_element_format> element_format_str_to_value_map_;
		static TG_map<G_string, E_nsl_semantic_input_class> semantic_input_class_str_to_value_map_;
		static TG_map<G_string, E_filter> filter_str_to_value_map_;
		static TG_map<G_string, E_texcoord_address_mode> texcoord_address_mode_str_to_value_map_;
		static TG_map<G_string, E_cull_mode> cull_mode_str_to_value_map_;
		static TG_map<G_string, E_fill_mode> fill_mode_str_to_value_map_;
		static TG_map<G_string, E_format> format_str_to_value_map_;
		static TG_map<G_string, E_depth_comparison_func> depth_comparison_func_str_to_value_map_;
		static TG_map<G_string, E_primitive_topology> primitive_topology_str_to_value_map_;

	public:
		NCPP_FORCE_INLINE const auto& info_trees() const noexcept { return info_trees_; }
		NCPP_FORCE_INLINE u32 location_offset_to_save() const noexcept { return location_offset_to_save_; }
		NCPP_FORCE_INLINE F_nsl_error_stack* error_stack_p() noexcept { return error_stack_p_; }



	public:
		F_nsl_info_tree_reader() = default;
		F_nsl_info_tree_reader(
			const TG_vector<F_nsl_info_tree>& info_trees,
			u32 location_offset_to_save = 0,
			F_nsl_error_stack* error_stack_p = 0
		);
		~F_nsl_info_tree_reader();

		F_nsl_info_tree_reader(const F_nsl_info_tree_reader&);
		F_nsl_info_tree_reader& operator = (const F_nsl_info_tree_reader&);
		F_nsl_info_tree_reader(F_nsl_info_tree_reader&&);
		F_nsl_info_tree_reader& operator = (F_nsl_info_tree_reader&&);

	public:
		b8 guarantee_not_empty() const;
		b8 guarantee_index(u32 index) const;

	public:
		eastl::optional<b8> read_b8(u32 index) const;
		eastl::optional<u8> read_u8(u32 index) const;
		eastl::optional<u16> read_u16(u32 index) const;
		eastl::optional<u32> read_u32(u32 index) const;
		eastl::optional<u64> read_u64(u32 index) const;
		eastl::optional<i8> read_i8(u32 index) const;
		eastl::optional<i16> read_i16(u32 index) const;
		eastl::optional<i32> read_i32(u32 index) const;
		eastl::optional<i64> read_i64(u32 index) const;
		eastl::optional<f32> read_f32(u32 index) const;
		eastl::optional<f64> read_f64(u32 index) const;
		eastl::optional<G_string> read_string(u32 index) const;
		b8 guarantee_flag(const G_string& name) const;
		eastl::optional<F_nsl_info_tree_reader> read_sub(const G_string& name) const;
		eastl::optional<E_nsl_element_format> read_element_format(u32 index) const;
		eastl::optional<E_nsl_semantic_input_class> read_semantic_input_class(u32 index) const;
		eastl::optional<E_filter> read_filter(u32 index) const;
		eastl::optional<E_texcoord_address_mode> read_texcoord_address_mode(u32 index) const;
		eastl::optional<E_cull_mode> read_cull_mode(u32 index) const;
		eastl::optional<E_fill_mode> read_fill_mode(u32 index) const;
		eastl::optional<E_format> read_format(u32 index) const;
		eastl::optional<E_depth_comparison_func> read_depth_comparison_func(u32 index) const;
		eastl::optional<E_primitive_topology> read_primitive_topology(u32 index) const;

	};

	struct F_nsl_data_type_desc {

		G_string name;
		G_string semantic;
		sz size = 0;

	};

	using F_nsl_data_argument_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_data_argument {

		G_string name;
		G_string type;
		u32 count = 1;
		F_nsl_data_argument_config_map config_map;

	};
	struct F_nsl_data_param {

		F_nsl_data_argument argument;
		b8 is_in = false;
		b8 is_out = false;

	};
	struct F_nsl_data_argument_member {

		F_nsl_data_argument argument;
		u32 offset;

	};

	using F_nsl_structure_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_structure_info {

		TG_vector<F_nsl_data_argument_member> argument_members;

		u32 alignment = 16;
		u32 size = 0;

		F_nsl_structure_config_map config_map;

	};
	using F_nsl_structure = eastl::pair<G_string, F_nsl_structure_info>;

	using F_nsl_enumeration_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_enumeration_info {

		G_string value_type = "u32";
		TG_vector<TG_pack<G_string, u64>> values;

		F_nsl_enumeration_config_map config_map;

	};
	using F_nsl_enumeration = eastl::pair<G_string, F_nsl_enumeration_info>;

	using F_nsl_resource_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_resource_info {

		G_string type;
		TG_vector<G_string> type_args;

		u32 slot = -1;

		TG_unordered_set<G_string> shader_filters = { "*" };

		F_nsl_resource_config_map config_map;

	};
	using F_nsl_resource = eastl::pair<G_string, F_nsl_resource_info>;

	using F_nsl_uniform_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_uniform_info {

		G_string type;
		G_string buffer;

		F_nsl_uniform_config_map config_map;

	};
	using F_nsl_uniform = eastl::pair<G_string, F_nsl_uniform_info>;

	using F_nsl_sampler_state_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_sampler_state_info {

		F_sampler_state_desc desc;

		u32 slot = -1;

		TG_unordered_set<G_string> shader_filters = { "*" };

		F_nsl_sampler_state_config_map config_map;

	};
	using F_nsl_sampler_state = eastl::pair<G_string, F_nsl_sampler_state_info>;

	using F_nsl_pipeline_state_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_pipeline_state_info {

		TG_vector<G_string> shaders;

		F_pipeline_state_desc desc;

		F_nsl_pipeline_state_config_map config_map;

	};
	using F_nsl_pipeline_state = eastl::pair<G_string, F_nsl_pipeline_state_info>;

	using F_nsl_vertex_layout_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_vertex_attribute {

		G_string semantic;
		u32 buffer = 0;
		u32 offset = -1;

	};
	struct F_nsl_vertex_layout_info {

		TG_vector<F_nsl_vertex_attribute> attributes;

		F_nsl_vertex_layout_config_map config_map;

	};
	using F_nsl_vertex_layout = eastl::pair<G_string, F_nsl_vertex_layout_info>;

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

	struct FE_nsl_name_types {

		struct DATA_TYPE {};
		struct SEMANTIC {};
		struct STRUCTURE {};
		struct RESOURCE {};
		struct UNIFORM {};
		struct SAMPLER_STATE {};
		struct PIPELINE_STATE {};
		struct SHADER {};

	};

	using F_nsl_object_config = TG_unordered_map<G_string, F_nsl_info_tree_reader>;

	struct F_nsl_semantic_info {

		G_string target_type;
		E_nsl_element_format element_format;
		E_nsl_semantic_input_class input_class = E_nsl_semantic_input_class::PER_VERTEX;

	};
	using F_nsl_semantic = eastl::pair<G_string, F_nsl_semantic_info>;

	struct F_nsl_context {

		TK<A_nsl_object> parent_object_p;

		TG_stack<F_nsl_object_type_channel_mask> object_type_channel_mask_stack = { nsl_default_object_type_channel_mask };

		F_nsl_object_config temp_object_config;
		F_nsl_object_config current_object_config;
		TG_stack<F_nsl_object_config> object_config_stack;

		G_string default_uniform_buffer;

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
			const F_nsl_ast_tree_recursive_build_functor& recursive_build_functor,
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



	class NRHI_API A_nsl_reflection_item {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;
		G_string name_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }
		NCPP_FORCE_INLINE const G_string& name() const noexcept { return name_; }



	protected:
		A_nsl_reflection_item(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			const G_string& name = ""
		);

	public:
		virtual ~A_nsl_reflection_item();

	public:
		NCPP_OBJECT(A_nsl_reflection_item);

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
		F_nsl_object_type_channel_mask channel_mask_ = nsl_default_object_type_channel_mask;

		TG_vector<TU<A_nsl_object>> object_p_vector_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }
		NCPP_FORCE_INLINE const G_string& name() const noexcept { return name_; }
		NCPP_FORCE_INLINE b8 is_object_name_required() const noexcept { return is_object_name_required_; }
		NCPP_FORCE_INLINE sz min_object_body_count() const noexcept { return min_object_body_count_; }
		NCPP_FORCE_INLINE sz max_object_body_count() const noexcept { return max_object_body_count_; }
		NCPP_FORCE_INLINE F_nsl_object_type_channel_mask channel_mask() const noexcept { return channel_mask_; }

		NCPP_FORCE_INLINE const TG_vector<TU<A_nsl_object>>& object_p_vector() const noexcept { return object_p_vector_; }



	protected:
		A_nsl_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			const G_string& name,
			b8 is_object_name_required = true,
			sz min_object_body_count = 0,
			sz max_object_body_count = 2,
			F_nsl_object_type_channel_mask channel_mask = nsl_default_object_type_channel_mask
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



	class NRHI_API F_nsl_undef_object final : public A_nsl_object {

	public:
		F_nsl_undef_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_undef_object();

	public:
		NCPP_OBJECT(F_nsl_undef_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_undef_object_type final : public A_nsl_object_type {

	public:
		F_nsl_undef_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_undef_object_type();

	public:
		NCPP_OBJECT(F_nsl_undef_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	};



	class NRHI_API F_nsl_require_object final : public A_nsl_object {

	private:
		b8 is_enabled_ = false;

	public:
		NCPP_FORCE_INLINE b8 is_enabled() const noexcept { return is_enabled_; }



	public:
		F_nsl_require_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_require_object();

	public:
		NCPP_OBJECT(F_nsl_require_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_require_object_type final : public A_nsl_object_type {

	public:
		F_nsl_require_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_require_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_require_object_type);

	};



	class NRHI_API F_nsl_otherwise_object final : public A_nsl_object {

	private:
		b8 is_enabled_ = false;

	public:
		NCPP_FORCE_INLINE b8 is_enabled() const noexcept { return is_enabled_; }



	public:
		F_nsl_otherwise_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_otherwise_object();

	public:
		NCPP_OBJECT(F_nsl_otherwise_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_otherwise_object_type final : public A_nsl_object_type {

	public:
		F_nsl_otherwise_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_otherwise_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_otherwise_object_type);

	};



	class NRHI_API F_nsl_annotation_object final : public A_nsl_object {

	public:
		F_nsl_annotation_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_annotation_object();

	public:
		NCPP_OBJECT(F_nsl_annotation_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_annotation_object_type final : public A_nsl_object_type {

	public:
		F_nsl_annotation_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_annotation_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_annotation_object_type);

	};



	class NRHI_API F_nsl_semantic_object final : public A_nsl_object {

	public:
		G_string target_type;



	public:
		F_nsl_semantic_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_semantic_object();

	public:
		NCPP_OBJECT(F_nsl_semantic_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_semantic_object_type final : public A_nsl_object_type {

	public:
		F_nsl_semantic_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_semantic_object_type();

	public:
		NCPP_OBJECT(F_nsl_semantic_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	};



	class NRHI_API F_nsl_structure_object : public A_nsl_object {

	public:
		F_nsl_structure_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_structure_object();

	public:
		NCPP_OBJECT(F_nsl_structure_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_structure_object_type : public A_nsl_object_type {

	public:
		F_nsl_structure_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_structure_object_type();

	public:
		NCPP_OBJECT(F_nsl_structure_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	};



	class NRHI_API F_nsl_enumeration_object : public A_nsl_object {

	public:
		F_nsl_enumeration_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_enumeration_object();

	public:
		NCPP_OBJECT(F_nsl_enumeration_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_enumeration_object_type : public A_nsl_object_type {

	public:
		F_nsl_enumeration_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_enumeration_object_type();

	public:
		NCPP_OBJECT(F_nsl_enumeration_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	};



	class NRHI_API F_nsl_resource_object : public A_nsl_object {

	public:
		F_nsl_resource_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_resource_object();

	public:
		NCPP_OBJECT(F_nsl_resource_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_resource_object_type : public A_nsl_object_type {

	public:
		F_nsl_resource_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_resource_object_type();

	public:
		NCPP_OBJECT(F_nsl_resource_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	};



	class NRHI_API F_nsl_uniform_object : public A_nsl_object {

	public:
		F_nsl_uniform_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_uniform_object();

	public:
		NCPP_OBJECT(F_nsl_uniform_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_uniform_object_type : public A_nsl_object_type {

	public:
		F_nsl_uniform_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_uniform_object_type();

	public:
		NCPP_OBJECT(F_nsl_uniform_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	};



	class NRHI_API F_nsl_default_uniform_buffer_object : public A_nsl_object {

	public:
		F_nsl_default_uniform_buffer_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_default_uniform_buffer_object();

	public:
		NCPP_OBJECT(F_nsl_default_uniform_buffer_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_default_uniform_buffer_object_type : public A_nsl_object_type {

	public:
		F_nsl_default_uniform_buffer_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_default_uniform_buffer_object_type();

	public:
		NCPP_OBJECT(F_nsl_default_uniform_buffer_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	};



	class NRHI_API F_nsl_sampler_state_object : public A_nsl_object {

	public:
		F_nsl_sampler_state_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_sampler_state_object();

	public:
		NCPP_OBJECT(F_nsl_sampler_state_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_sampler_state_object_type : public A_nsl_object_type {

	public:
		F_nsl_sampler_state_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_sampler_state_object_type();

	public:
		NCPP_OBJECT(F_nsl_sampler_state_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	};



	class NRHI_API F_nsl_pipeline_state_object : public A_nsl_object {

	public:
		F_nsl_pipeline_state_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_pipeline_state_object();

	public:
		NCPP_OBJECT(F_nsl_pipeline_state_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_pipeline_state_object_type : public A_nsl_object_type {

	public:
		F_nsl_pipeline_state_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_pipeline_state_object_type();

	public:
		NCPP_OBJECT(F_nsl_pipeline_state_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	};



	class NRHI_API F_nsl_vertex_layout_object : public A_nsl_object {

	public:
		F_nsl_vertex_layout_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_vertex_layout_object();

	public:
		NCPP_OBJECT(F_nsl_vertex_layout_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_vertex_layout_object_type : public A_nsl_object_type {

	public:
		F_nsl_vertex_layout_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_vertex_layout_object_type();

	public:
		NCPP_OBJECT(F_nsl_vertex_layout_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	};



	class NRHI_API A_nsl_shader_object : public A_nsl_object {

	protected:
		TG_vector<F_nsl_data_param> data_params_;

	public:
		NCPP_FORCE_INLINE const auto& data_params() const noexcept { return data_params_; }



	protected:
		A_nsl_shader_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);

	public:
		virtual ~A_nsl_shader_object();

	public:
		NCPP_OBJECT(A_nsl_shader_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API A_nsl_shader_object_type : public A_nsl_object_type {

	public:
		A_nsl_shader_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			const G_string& name
		);
		virtual ~A_nsl_shader_object_type();

	public:
		NCPP_OBJECT(A_nsl_shader_object_type);

	};



	class NRHI_API F_nsl_vertex_shader_object final : public A_nsl_shader_object {

	private:
		F_input_assembler_desc input_assempler_desc_;

	public:
		NCPP_FORCE_INLINE const F_input_assembler_desc& input_assempler_desc() const noexcept { return input_assempler_desc_; }



	public:
		F_nsl_vertex_shader_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_vertex_shader_object();

	public:
		NCPP_OBJECT(F_nsl_vertex_shader_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_vertex_shader_object_type final : public A_nsl_shader_object_type {

	public:
		F_nsl_vertex_shader_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_vertex_shader_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_vertex_shader_object_type);

	};



	class NRHI_API F_nsl_pixel_shader_object final : public A_nsl_shader_object {

	public:
		F_nsl_pixel_shader_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_pixel_shader_object();

	public:
		NCPP_OBJECT(F_nsl_pixel_shader_object);

	};



	class NRHI_API F_nsl_pixel_shader_object_type final : public A_nsl_shader_object_type {

	public:
		F_nsl_pixel_shader_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_pixel_shader_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_pixel_shader_object_type);

	};



	class NRHI_API F_nsl_compute_shader_object final : public A_nsl_shader_object {

	public:
		F_nsl_compute_shader_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_compute_shader_object();

	public:
		NCPP_OBJECT(F_nsl_compute_shader_object);

	};



	class NRHI_API F_nsl_compute_shader_object_type final : public A_nsl_shader_object_type {

	public:
		F_nsl_compute_shader_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_compute_shader_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_compute_shader_object_type);

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

		TG_vector<F_nsl_ast_tree_try_build_functor> ast_tree_try_build_functors(F_nsl_object_type_channel_mask mask = nsl_default_object_type_channel_mask);

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
			sz location_offset_to_safe = 0,
			F_nsl_object_type_channel_mask additional_object_type_channel_mask = 0
		);
		eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
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
		TG_unordered_map<G_string, u64> name_to_name_type_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const TG_set<G_string>& name_set() const noexcept { return name_set_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, G_string>& name_to_target_map() const noexcept { return name_to_target_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, u64>& name_to_name_type_map() const noexcept { return name_to_name_type_map_; }



	public:
		F_nsl_name_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_name_manager();

	public:
		NCPP_OBJECT(F_nsl_name_manager);

	public:
		NCPP_FORCE_INLINE b8 is_name_registered(const G_string& name) const {

			auto it = name_to_target_map_.find(name);

			return (it != name_to_target_map_.end());
		}
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

			G_string result = name;
			auto it = name_to_target_map_.find(name);

			NCPP_ASSERT(is_name_registered(name))
				<< T_cout_value(name)
				<< " is not registered";

			while(
				(it != name_to_target_map_.end())
			) {
				if(it->second == result)
					return result;

				result = it->second;

				it = name_to_target_map_.find(result);
			}

			return result;
		}
		NCPP_FORCE_INLINE u64 name_type(const G_string& name) const {

			auto it = name_to_name_type_map_.find(
				target(name)
			);

			if(it == name_to_name_type_map_.end())
				return 0;

			return it->second;
		}
		NCPP_FORCE_INLINE void register_name(const G_string& name, const G_string& target) {

			NCPP_ASSERT(name_to_target_map_.find(name) == name_to_target_map_.end()) << T_cout_value(name) << " already exists";
			NCPP_ASSERT(is_name_has_target(target) || (name == target)) << T_cout_value(target) << " is not registered";

			name_to_target_map_[name] = target;
		}
		NCPP_FORCE_INLINE void register_name(const G_string& name, u64 name_type) {

			NCPP_ASSERT(name_to_name_type_map_.find(name) == name_to_name_type_map_.end()) << T_cout_value(name) << " already exists";

			name_to_name_type_map_[name] = name_type;
			name_to_target_map_[name] = name;
		}
		NCPP_FORCE_INLINE void register_name(const G_string& name) {

			NCPP_ASSERT(name_to_name_type_map_.find(name) == name_to_name_type_map_.end()) << T_cout_value(name) << " already exists";

			name_to_name_type_map_[name] = 0;
			name_to_target_map_[name] = name;
		}
		template<typename F__>
		NCPP_FORCE_INLINE void T_register_name(const G_string& name) {

			NCPP_ASSERT(name_to_name_type_map_.find(name) == name_to_name_type_map_.end()) << T_cout_value(name) << " already exists";

			name_to_name_type_map_[name] = T_type_hash_code<F__>;
			name_to_target_map_[name] = name;
		}
		NCPP_FORCE_INLINE void deregister_name(const G_string& name) {

			NCPP_ASSERT(
				(name_to_target_map_.find(name) != name_to_target_map_.end())
				|| (name_to_name_type_map_.find(name) != name_to_name_type_map_.end())
			) << T_cout_value(name) << " is not exists";

			auto it1 = name_to_name_type_map_.find(name);
			auto it2 = name_to_target_map_.find(name);

			if(it1 != name_to_name_type_map_.end())
				name_to_name_type_map_.erase(it1);
			if(it2 != name_to_target_map_.end())
				name_to_target_map_.erase(it2);
		}

	public:
		eastl::optional<b8> check_formula(
			TK_valid<F_nsl_translation_unit> translation_unit_p,
			sz location,
			const G_string& formula
		);

	};



	class NRHI_API F_nsl_data_type_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TG_unordered_map<G_string, sz> name_to_size_map_;
		TG_unordered_map<G_string, u32> name_to_alignment_map_;
		TG_unordered_map<G_string, E_nsl_element_format> name_to_element_format_map_;
		TG_unordered_map<G_string, F_nsl_semantic_info> name_to_semantic_info_map_;
		TG_unordered_map<G_string, F_nsl_structure_info> name_to_structure_info_map_;
		TG_unordered_map<G_string, F_nsl_enumeration_info> name_to_enumeration_info_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const TG_unordered_map<G_string, sz>& name_to_size_map() const noexcept { return name_to_size_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, u32>& name_to_alignment_map() const noexcept { return name_to_alignment_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, E_nsl_element_format>& name_to_element_format_map() const noexcept { return name_to_element_format_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, F_nsl_semantic_info>& name_to_semantic_info_map() const noexcept { return name_to_semantic_info_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, F_nsl_structure_info>& name_to_structure_info_map() const noexcept { return name_to_structure_info_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, F_nsl_enumeration_info>& name_to_enumeration_info_map() const noexcept { return name_to_enumeration_info_map_; }



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
		NCPP_FORCE_INLINE void deregister_size(const G_string& name) {

			NCPP_ASSERT(name_to_size_map_.find(name) != name_to_size_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_size_map_.find(name);
			name_to_size_map_.erase(it);
		}

	public:
		NCPP_FORCE_INLINE b8 is_name_has_alignment(const G_string& name) const {

			auto it = name_to_alignment_map_.find(name);

			return (it != name_to_alignment_map_.end());
		}
		NCPP_FORCE_INLINE u32 alignment(const G_string& name) const {

			auto it = name_to_alignment_map_.find(name);

			NCPP_ASSERT(it != name_to_alignment_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_alignment(const G_string& name, u32 alignment) {

			NCPP_ASSERT(name_to_alignment_map_.find(name) == name_to_alignment_map_.end()) << T_cout_value(name) << " already exists";

			name_to_alignment_map_[name] = alignment;
		}
		NCPP_FORCE_INLINE void deregister_alignment(const G_string& name) {

			NCPP_ASSERT(name_to_alignment_map_.find(name) != name_to_alignment_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_alignment_map_.find(name);
			name_to_alignment_map_.erase(it);
		}

	public:
		NCPP_FORCE_INLINE b8 is_name_has_element_format(const G_string& name) const {

			auto it = name_to_element_format_map_.find(name);

			return (it != name_to_element_format_map_.end());
		}
		NCPP_FORCE_INLINE E_nsl_element_format element_format(const G_string& name) const {

			auto it = name_to_element_format_map_.find(name);

			NCPP_ASSERT(it != name_to_element_format_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_element_format(const G_string& name, E_nsl_element_format element_format) {

			NCPP_ASSERT(name_to_element_format_map_.find(name) == name_to_element_format_map_.end()) << T_cout_value(name) << " already exists";

			name_to_element_format_map_[name] = element_format;
		}
		NCPP_FORCE_INLINE void deregister_element_format(const G_string& name) {

			NCPP_ASSERT(name_to_element_format_map_.find(name) != name_to_element_format_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_element_format_map_.find(name);
			name_to_element_format_map_.erase(it);
		}

	public:
		NCPP_FORCE_INLINE b8 is_name_has_semantic_info(const G_string& name) const {

			auto it = name_to_semantic_info_map_.find(name);

			return (it != name_to_semantic_info_map_.end());
		}
		NCPP_FORCE_INLINE const F_nsl_semantic_info& semantic_info(const G_string& name) const {

			auto it = name_to_semantic_info_map_.find(name);

			NCPP_ASSERT(it != name_to_semantic_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_semantic(const G_string& name, const F_nsl_semantic_info& semantic_info) {

			NCPP_ASSERT(name_to_semantic_info_map_.find(name) == name_to_semantic_info_map_.end()) << T_cout_value(name) << " already exists";

			name_to_semantic_info_map_[name] = process_semantic_info(name, semantic_info);
		}
		NCPP_FORCE_INLINE void deregister_semantic(const G_string& name) {

			NCPP_ASSERT(name_to_semantic_info_map_.find(name) != name_to_semantic_info_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_semantic_info_map_.find(name);
			name_to_semantic_info_map_.erase(it);
		}

	public:
		NCPP_FORCE_INLINE b8 is_name_has_structure_info(const G_string& name) const {

			auto it = name_to_structure_info_map_.find(name);

			return (it != name_to_structure_info_map_.end());
		}
		NCPP_FORCE_INLINE const F_nsl_structure_info& structure_info(const G_string& name) const {

			auto it = name_to_structure_info_map_.find(name);

			NCPP_ASSERT(it != name_to_structure_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_structure(const G_string& name, const F_nsl_structure_info& structure_info) {

			NCPP_ASSERT(name_to_structure_info_map_.find(name) == name_to_structure_info_map_.end()) << T_cout_value(name) << " already exists";

			name_to_structure_info_map_[name] = process_structure_info(name, structure_info);
		}
		NCPP_FORCE_INLINE void deregister_structure(const G_string& name) {

			NCPP_ASSERT(name_to_structure_info_map_.find(name) != name_to_structure_info_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_structure_info_map_.find(name);
			name_to_structure_info_map_.erase(it);
		}

	public:
		NCPP_FORCE_INLINE b8 is_name_has_enumeration_info(const G_string& name) const {

			auto it = name_to_enumeration_info_map_.find(name);

			return (it != name_to_enumeration_info_map_.end());
		}
		NCPP_FORCE_INLINE const F_nsl_enumeration_info& enumeration_info(const G_string& name) const {

			auto it = name_to_enumeration_info_map_.find(name);

			NCPP_ASSERT(it != name_to_enumeration_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_enumeration(const G_string& name, const F_nsl_enumeration_info& enumeration_info) {

			NCPP_ASSERT(name_to_enumeration_info_map_.find(name) == name_to_enumeration_info_map_.end()) << T_cout_value(name) << " already exists";

			name_to_enumeration_info_map_[name] = process_enumeration_info(name, enumeration_info);
		}
		NCPP_FORCE_INLINE void deregister_enumeration(const G_string& name) {

			NCPP_ASSERT(name_to_enumeration_info_map_.find(name) != name_to_enumeration_info_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_enumeration_info_map_.find(name);
			name_to_enumeration_info_map_.erase(it);
		}

	private:
		F_nsl_semantic_info process_semantic_info(const G_string& name, const F_nsl_semantic_info& semantic_info);
		F_nsl_structure_info process_structure_info(const G_string& name, const F_nsl_structure_info& structure_info);
		F_nsl_enumeration_info process_enumeration_info(const G_string& name, const F_nsl_enumeration_info& enumeration_info);

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

	protected:
		TG_unordered_map<G_string, F_nsl_resource_info> name_to_resource_info_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const TG_unordered_map<G_string, F_nsl_resource_info>& name_to_resource_info_map() const noexcept { return name_to_resource_info_map_; }



	public:
		F_nsl_resource_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_resource_manager();

	public:
		NCPP_OBJECT(F_nsl_resource_manager);

	public:
		NCPP_FORCE_INLINE b8 is_name_has_resource_info(const G_string& name) const {

			auto it = name_to_resource_info_map_.find(name);

			return (it != name_to_resource_info_map_.end());
		}
		NCPP_FORCE_INLINE const F_nsl_resource_info& resource_info(const G_string& name) const {

			auto it = name_to_resource_info_map_.find(name);

			NCPP_ASSERT(it != name_to_resource_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_resource(const G_string& name, const F_nsl_resource_info& resource_info) {

			NCPP_ASSERT(name_to_resource_info_map_.find(name) == name_to_resource_info_map_.end()) << T_cout_value(name) << " already exists";

			name_to_resource_info_map_[name] = process_resource_info(name, resource_info);
		}
		NCPP_FORCE_INLINE void deregister_resource(const G_string& name) {

			NCPP_ASSERT(name_to_resource_info_map_.find(name) != name_to_resource_info_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_resource_info_map_.find(name);
			name_to_resource_info_map_.erase(it);
		}

	private:
		F_nsl_resource_info process_resource_info(const G_string& name, const F_nsl_resource_info& resource_info);

	};



	class NRHI_API F_nsl_uniform_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TG_unordered_map<G_string, F_nsl_uniform_info> name_to_uniform_info_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const TG_unordered_map<G_string, F_nsl_uniform_info>& name_to_uniform_info_map() const noexcept { return name_to_uniform_info_map_; }



	public:
		F_nsl_uniform_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_uniform_manager();

	public:
		NCPP_OBJECT(F_nsl_uniform_manager);

	public:
		NCPP_FORCE_INLINE b8 is_name_has_uniform_info(const G_string& name) const {

			auto it = name_to_uniform_info_map_.find(name);

			return (it != name_to_uniform_info_map_.end());
		}
		NCPP_FORCE_INLINE const F_nsl_uniform_info& uniform_info(const G_string& name) const {

			auto it = name_to_uniform_info_map_.find(name);

			NCPP_ASSERT(it != name_to_uniform_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_uniform(const G_string& name, const F_nsl_uniform_info& uniform_info) {

			NCPP_ASSERT(name_to_uniform_info_map_.find(name) == name_to_uniform_info_map_.end()) << T_cout_value(name) << " already exists";

			name_to_uniform_info_map_[name] = process_uniform_info(name, uniform_info);
		}
		NCPP_FORCE_INLINE void deregister_uniform(const G_string& name) {

			NCPP_ASSERT(name_to_uniform_info_map_.find(name) != name_to_uniform_info_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_uniform_info_map_.find(name);
			name_to_uniform_info_map_.erase(it);
		}

	private:
		F_nsl_uniform_info process_uniform_info(const G_string& name, const F_nsl_uniform_info& uniform_info);

	};



	class NRHI_API F_nsl_sampler_state_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TG_unordered_map<G_string, F_nsl_sampler_state_info> name_to_sampler_state_info_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const TG_unordered_map<G_string, F_nsl_sampler_state_info>& name_to_sampler_state_info_map() const noexcept { return name_to_sampler_state_info_map_; }



	public:
		F_nsl_sampler_state_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_sampler_state_manager();

	public:
		NCPP_OBJECT(F_nsl_sampler_state_manager);

	public:
		NCPP_FORCE_INLINE b8 is_name_has_sampler_state_info(const G_string& name) const {

			auto it = name_to_sampler_state_info_map_.find(name);

			return (it != name_to_sampler_state_info_map_.end());
		}
		NCPP_FORCE_INLINE const F_nsl_sampler_state_info& sampler_state_info(const G_string& name) const {

			auto it = name_to_sampler_state_info_map_.find(name);

			NCPP_ASSERT(it != name_to_sampler_state_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_sampler_state(const G_string& name, const F_nsl_sampler_state_info& sampler_state_info) {

			NCPP_ASSERT(name_to_sampler_state_info_map_.find(name) == name_to_sampler_state_info_map_.end()) << T_cout_value(name) << " already exists";

			name_to_sampler_state_info_map_[name] = process_sampler_state_info(name, sampler_state_info);
		}
		NCPP_FORCE_INLINE void deregister_sampler_state(const G_string& name) {

			NCPP_ASSERT(name_to_sampler_state_info_map_.find(name) != name_to_sampler_state_info_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_sampler_state_info_map_.find(name);
			name_to_sampler_state_info_map_.erase(it);
		}

	private:
		F_nsl_sampler_state_info process_sampler_state_info(const G_string& name, const F_nsl_sampler_state_info& sampler_state_info);

	};



	class NRHI_API F_nsl_pipeline_state_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TG_unordered_map<G_string, F_nsl_pipeline_state_info> name_to_pipeline_state_info_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const TG_unordered_map<G_string, F_nsl_pipeline_state_info>& name_to_pipeline_state_info_map() const noexcept { return name_to_pipeline_state_info_map_; }



	public:
		F_nsl_pipeline_state_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_pipeline_state_manager();

	public:
		NCPP_OBJECT(F_nsl_pipeline_state_manager);

	public:
		NCPP_FORCE_INLINE b8 is_name_has_pipeline_state_info(const G_string& name) const {

			auto it = name_to_pipeline_state_info_map_.find(name);

			return (it != name_to_pipeline_state_info_map_.end());
		}
		NCPP_FORCE_INLINE const F_nsl_pipeline_state_info& pipeline_state_info(const G_string& name) const {

			auto it = name_to_pipeline_state_info_map_.find(name);

			NCPP_ASSERT(it != name_to_pipeline_state_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_pipeline_state(const G_string& name, const F_nsl_pipeline_state_info& pipeline_state_info) {

			NCPP_ASSERT(name_to_pipeline_state_info_map_.find(name) == name_to_pipeline_state_info_map_.end()) << T_cout_value(name) << " already exists";

			name_to_pipeline_state_info_map_[name] = process_pipeline_state_info(name, pipeline_state_info);
		}
		NCPP_FORCE_INLINE void deregister_pipeline_state(const G_string& name) {

			NCPP_ASSERT(name_to_pipeline_state_info_map_.find(name) != name_to_pipeline_state_info_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_pipeline_state_info_map_.find(name);
			name_to_pipeline_state_info_map_.erase(it);
		}

	private:
		F_nsl_pipeline_state_info process_pipeline_state_info(const G_string& name, const F_nsl_pipeline_state_info& pipeline_state_info);

	};



	class NRHI_API F_nsl_vertex_layout_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TG_unordered_map<G_string, F_nsl_vertex_layout_info> name_to_vertex_layout_info_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const TG_unordered_map<G_string, F_nsl_vertex_layout_info>& name_to_vertex_layout_info_map() const noexcept { return name_to_vertex_layout_info_map_; }



	public:
		F_nsl_vertex_layout_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_vertex_layout_manager();

	public:
		NCPP_OBJECT(F_nsl_vertex_layout_manager);

	public:
		NCPP_FORCE_INLINE b8 is_name_has_vertex_layout_info(const G_string& name) const {

			auto it = name_to_vertex_layout_info_map_.find(name);

			return (it != name_to_vertex_layout_info_map_.end());
		}
		NCPP_FORCE_INLINE const F_nsl_vertex_layout_info& vertex_layout_info(const G_string& name) const {

			auto it = name_to_vertex_layout_info_map_.find(name);

			NCPP_ASSERT(it != name_to_vertex_layout_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_vertex_layout(const G_string& name, const F_nsl_vertex_layout_info& vertex_layout_info) {

			NCPP_ASSERT(name_to_vertex_layout_info_map_.find(name) == name_to_vertex_layout_info_map_.end()) << T_cout_value(name) << " already exists";

			name_to_vertex_layout_info_map_[name] = process_vertex_layout_info(name, vertex_layout_info);
		}
		NCPP_FORCE_INLINE void deregister_vertex_layout(const G_string& name) {

			NCPP_ASSERT(name_to_vertex_layout_info_map_.find(name) != name_to_vertex_layout_info_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_vertex_layout_info_map_.find(name);
			name_to_vertex_layout_info_map_.erase(it);
		}

	private:
		F_nsl_vertex_layout_info process_vertex_layout_info(const G_string& name, const F_nsl_vertex_layout_info& vertex_layout_info);

	};



	template<typename F__>
	using TF_nsl_shader_compiler_subsystem_creator = eastl::function<
		TU<F__>(TKPA_valid<F_nsl_shader_compiler>)
	>;

#define NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(Type, Name) \
			TF_nsl_shader_compiler_subsystem_creator<Type> Name = (\
				[](TKPA_valid<F_nsl_shader_compiler> shader_compiler_p) -> TU<Type> {\
			\
					return TU<Type>()(shader_compiler_p);\
				}\
			)

	struct F_nsl_shader_compiler_customizer {

		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_shader_module_manager,
			module_manager_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_shader_module_manager,
			shader_module_manager_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_translation_unit_manager,
			translation_unit_manager_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_translation_unit_compiler,
			translation_unit_compiler_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_error_storage,
			error_storage_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_object_manager,
			object_manager_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_name_manager,
			name_manager_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_data_type_manager,
			data_type_manager_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_resource_manager,
			resource_manager_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_uniform_manager,
			uniform_manager_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_sampler_state_manager,
			sampler_state_manager_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_pipeline_state_manager,
			pipeline_state_manager_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_vertex_layout_manager,
			vertex_layout_manager_creator
		);

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
		TU<F_nsl_uniform_manager> uniform_manager_p_;
		TU<F_nsl_sampler_state_manager> sampler_state_manager_p_;
		TU<F_nsl_pipeline_state_manager> pipeline_state_manager_p_;
		TU<F_nsl_vertex_layout_manager> vertex_layout_manager_p_;

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
		NCPP_FORCE_INLINE TK_valid<F_nsl_uniform_manager> uniform_manager_p() const noexcept { return NCPP_FOH_VALID(uniform_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_sampler_state_manager> sampler_state_manager_p() const noexcept { return NCPP_FOH_VALID(sampler_state_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_pipeline_state_manager> pipeline_state_manager_p() const noexcept { return NCPP_FOH_VALID(pipeline_state_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_vertex_layout_manager> vertex_layout_manager_p() const noexcept { return NCPP_FOH_VALID(vertex_layout_manager_p_); }

		NCPP_FORCE_INLINE TK<A_nsl_output_language> output_language_p() const noexcept { return output_language_p_; }



	public:
		F_nsl_shader_compiler();
		F_nsl_shader_compiler(
			const F_nsl_shader_compiler_customizer& customizer
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
