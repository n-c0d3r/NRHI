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
#include <nrhi/sampler_state_desc.hpp>
#include <nrhi/cull_mode.hpp>
#include <nrhi/fill_mode.hpp>
#include <nrhi/format.hpp>
#include <nrhi/comparison_func.hpp>
#include <nrhi/primitive_topology.hpp>
#include <nrhi/pipeline_state_desc.hpp>
#include <nrhi/device.hpp>
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#include <nrhi/root_signature_desc.hpp>
#endif

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

		NONE,
		HLSL_4,
		HLSL_5,
		HLSL_5_1,
		HLSL_6_0,
		HLSL_6_1,
		HLSL_6_2,
		HLSL_6_3,
		HLSL_6_4,
		HLSL_6_5,
		HLSL_6_6,
		HLSL_6_7,

	};

	enum class E_nsl_feature {

		RESOURCE_ARRAY

	};

	using F_resource_dimension_sizes = TG_array<u32, 16>;

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

		NONE,

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

	enum class E_nsl_output_topology {
		TRIANGLE,
		LINE
	};

	class NRHI_API F_nsl_info_tree_reader {

	private:
		TK<F_nsl_shader_compiler> shader_compiler_p_;

		TG_vector<F_nsl_info_tree> info_trees_;
		u32 location_offset_to_save_ = 0;
		F_nsl_error_stack* error_stack_p_ = 0;

		static b8 is_maps_setup_;
		static TG_map<G_string, b8> b8_str_to_value_map_;
		static TG_map<G_string, E_nsl_element_format> element_format_str_to_value_map_;
		static TG_map<G_string, ED_input_classification> input_classification_str_to_value_map_;
		static TG_map<G_string, ED_filter> filter_str_to_value_map_;
		static TG_map<G_string, ED_texcoord_address_mode> texcoord_address_mode_str_to_value_map_;
		static TG_map<G_string, ED_cull_mode> cull_mode_str_to_value_map_;
		static TG_map<G_string, ED_fill_mode> fill_mode_str_to_value_map_;
		static TG_map<G_string, ED_format> format_str_to_value_map_;
		static TG_map<G_string, ED_comparison_func> comparison_func_str_to_value_map_;
		static TG_map<G_string, ED_primitive_topology> primitive_topology_str_to_value_map_;
		static TG_map<G_string, ED_shader_visibility> shader_visibility_str_to_value_map_;

	public:
		NCPP_FORCE_INLINE TKPA<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE const auto& info_trees() const noexcept { return info_trees_; }
		NCPP_FORCE_INLINE u32 location_offset_to_save() const noexcept { return location_offset_to_save_; }
		NCPP_FORCE_INLINE F_nsl_error_stack* error_stack_p() noexcept { return error_stack_p_; }



	public:
		F_nsl_info_tree_reader() = default;
		F_nsl_info_tree_reader(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			const TG_vector<F_nsl_info_tree>& info_trees,
			u32 location_offset_to_save = 0,
			F_nsl_error_stack* error_stack_p = 0
		);
		~F_nsl_info_tree_reader();

		F_nsl_info_tree_reader(const F_nsl_info_tree_reader&);
		F_nsl_info_tree_reader& operator = (const F_nsl_info_tree_reader&);
		F_nsl_info_tree_reader(F_nsl_info_tree_reader&&);
		F_nsl_info_tree_reader& operator = (F_nsl_info_tree_reader&&);

	private:
		G_string parse_value_str(const G_string& value_str) const;

	public:
		b8 guarantee_not_empty(b8 is_required = true) const;
		b8 guarantee_index(u32 index, b8 is_required = true) const;

	public:
		eastl::optional<b8> read_b8(u32 index, b8 is_required = true) const;
		eastl::optional<u8> read_u8(u32 index, b8 is_required = true) const;
		eastl::optional<u16> read_u16(u32 index, b8 is_required = true) const;
		eastl::optional<u32> read_u32(u32 index, b8 is_required = true) const;
		eastl::optional<u64> read_u64(u32 index, b8 is_required = true) const;
		eastl::optional<i8> read_i8(u32 index, b8 is_required = true) const;
		eastl::optional<i16> read_i16(u32 index, b8 is_required = true) const;
		eastl::optional<i32> read_i32(u32 index, b8 is_required = true) const;
		eastl::optional<i64> read_i64(u32 index, b8 is_required = true) const;
		eastl::optional<f32> read_f32(u32 index, b8 is_required = true) const;
		eastl::optional<f64> read_f64(u32 index, b8 is_required = true) const;
		eastl::optional<G_string> read_string(u32 index, b8 is_required = true) const;
		b8 guarantee_flag(const G_string& name, b8 is_required = true) const;
		eastl::optional<F_nsl_info_tree_reader> read_sub(const G_string& name, b8 is_required = true) const;
		eastl::optional<E_nsl_element_format> read_element_format(u32 index, b8 is_required = true) const;
		eastl::optional<ED_input_classification> read_input_classification(u32 index, b8 is_required = true) const;
		eastl::optional<ED_filter> read_filter(u32 index, b8 is_required = true) const;
		eastl::optional<ED_texcoord_address_mode> read_texcoord_address_mode(u32 index, b8 is_required = true) const;
		eastl::optional<ED_cull_mode> read_cull_mode(u32 index, b8 is_required = true) const;
		eastl::optional<ED_fill_mode> read_fill_mode(u32 index, b8 is_required = true) const;
		eastl::optional<ED_format> read_format(u32 index, b8 is_required = true) const;
		eastl::optional<ED_comparison_func> read_comparison_func(u32 index, b8 is_required = true) const;
		eastl::optional<ED_primitive_topology> read_primitive_topology(u32 index, b8 is_required = true) const;
		eastl::optional<E_nsl_output_topology> read_output_topology(u32 index, b8 is_required = true) const;
		eastl::optional<ED_shader_visibility> read_shader_visibility(u32 index, b8 is_required = true) const;
		b8 read_configurable_elements(
			const eastl::function<
				b8(
					const F_nsl_info_tree& element_info_tree,
					const F_nsl_info_tree_reader& element_info_tree_reader,
					TG_unordered_map<G_string, F_nsl_info_tree_reader>& config_map
				)
			>& callback
		) const;

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
		b8 is_array = false;
		F_nsl_data_argument_config_map config_map;
	};

	enum class E_nsl_data_param_flag
	{
		NONE = 0x0,
		INDICES = 0x1,
		VERTICES = 0x2,
		PRIMITIVES = 0x4,
		PAYLOAD = 0x8
	};
	struct F_nsl_data_param {

		F_nsl_data_argument argument;
		b8 is_in = false;
		b8 is_out = false;

		E_nsl_data_param_flag flags = E_nsl_data_param_flag::NONE;
	};

	struct F_nsl_data_argument_member {

		F_nsl_data_argument argument;
		u32 offset = 0;
	};

	using F_nsl_structure_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_structure_info {

		TG_vector<F_nsl_data_argument_member> argument_members;

		u32 alignment = 1;
		u32 size = 0;

		F_nsl_structure_config_map config_map;

		u32 begin_location = 0;
		u32 end_location = 0;
		TK<F_nsl_translation_unit> translation_unit_p;
	};
	using F_nsl_structure = eastl::pair<G_string, F_nsl_structure_info>;

	using F_nsl_enumeration_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_enumeration_info {

		G_string value_type = "u32";
		TG_vector<TG_pack<G_string, u64>> values;

		F_nsl_enumeration_config_map config_map;

		u32 begin_location = 0;
		u32 end_location = 0;
		TK<F_nsl_translation_unit> translation_unit_p;

	};
	using F_nsl_enumeration = eastl::pair<G_string, F_nsl_enumeration_info>;

	struct E_nsl_shader_filter
	{
		u32 slot_space = 0;
	};

	enum class E_nsl_resource_type {
		NONE,
		CONSTANT_BUFFER,
		BUFFER,
		BYTE_ADDRESS_BUFFER,
		STRUCTURED_BUFFER,
		TEXTURE_1D,
		TEXTURE_1D_ARRAY,
		TEXTURE_2D,
		TEXTURE_2D_ARRAY,
		TEXTURE_3D,
		TEXTURE_CUBE,
		TEXTURE_CUBE_ARRAY,
		RW_BUFFER,
		RW_BYTE_ADDRESS_BUFFER,
		RW_STRUCTURED_BUFFER,
		RW_TEXTURE_1D,
		RW_TEXTURE_1D_ARRAY,
		RW_TEXTURE_2D,
		RW_TEXTURE_2D_ARRAY,
		RW_TEXTURE_3D
	};
	enum class E_nsl_resource_type_class {
		NONE,
		SHADER_RESOURCE,
		UNORDERED_ACCESS,
		CONSTANT_BUFFER
	};
	enum class E_nsl_resource_flag {
		NONE,
		GLOBALLY_COHERENT
	};
	using F_nsl_resource_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_resource_info {

		G_string type;
		E_nsl_resource_type type_as_enum = E_nsl_resource_type::NONE;
		TG_vector<G_string> type_args;

		E_nsl_resource_type_class type_class = E_nsl_resource_type_class::NONE;

		u32 slot = NCPP_U32_MAX;
		u32 slot_space = NCPP_U32_MAX;
		TG_vector<u32> actual_slots;
		TG_vector<u32> actual_slot_spaces;

		TG_vector<G_string> uniforms;
		u32 sort_uniforms = false;
		u32 constant_size = NCPP_U32_MAX;

		F_resource_dimension_sizes dimension_sizes = { 1 };
		u32 dimension_count = 1;
		u32 is_array = false;

		E_nsl_resource_flag flags = E_nsl_resource_flag::NONE;

		TG_unordered_map<G_string, E_nsl_shader_filter> shader_filters = { { "*", {} } };

		F_nsl_resource_config_map config_map;

		u32 begin_location = 0;
		u32 end_location = 0;
		TK<F_nsl_translation_unit> translation_unit_p;

	};
	using F_nsl_resource = eastl::pair<G_string, F_nsl_resource_info>;

	using F_nsl_uniform_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_uniform_info {

		G_string type;
		G_string buffer;

		u32 count = 1;
		b8 is_array = false;

		u32 offset = NCPP_U32_MAX;

		F_nsl_uniform_config_map config_map;

		u32 begin_location = 0;
		u32 end_location = 0;
		TK<F_nsl_translation_unit> translation_unit_p;

		u32 creation_index = NCPP_U32_MAX;
	};
	using F_nsl_uniform = eastl::pair<G_string, F_nsl_uniform_info>;

	using F_nsl_sampler_state_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_sampler_state_info {

		F_sampler_state_desc desc;
		b8 is_static = false;
		b8 is_comparison_state = false;

		u32 slot = NCPP_U32_MAX;
		u32 slot_space = NCPP_U32_MAX;
		TG_vector<u32> actual_slots;
		TG_vector<u32> actual_slot_spaces;

		TG_unordered_map<G_string, E_nsl_shader_filter> shader_filters = { { "*", {} } };

		F_nsl_sampler_state_config_map config_map;

		u32 begin_location = 0;
		u32 end_location = 0;
		TK<F_nsl_translation_unit> translation_unit_p;

	};
	using F_nsl_sampler_state = eastl::pair<G_string, F_nsl_sampler_state_info>;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	using F_nsl_root_signature_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_root_signature_info
	{
		F_root_signature_desc desc;

		F_nsl_root_signature_config_map config_map;

		u32 begin_location = 0;
		u32 end_location = 0;
		TK<F_nsl_translation_unit> translation_unit_p;
	};
	using F_nsl_root_signature = eastl::pair<G_string, F_nsl_root_signature_info>;

	enum class E_nsl_root_signature_selection_type
	{
		EMBEDDED,
		EXTERNAL
	};
	struct F_nsl_root_signature_selection
	{
		G_string name;
		E_nsl_root_signature_selection_type type = E_nsl_root_signature_selection_type::EMBEDDED;

		NCPP_FORCE_INLINE b8 is_valid() const noexcept
		{
			return !(name.empty());
		}
		NCPP_FORCE_INLINE b8 is_null() const noexcept
		{
			return name.empty();
		}
		NCPP_FORCE_INLINE operator b8 () const noexcept
		{
			return is_valid();
		}
	};
#endif

	using F_nsl_pipeline_state_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_pipeline_state_info {

		TG_vector<G_string> shaders;

		ED_pipeline_state_type type = ED_pipeline_state_type::NONE;

		F_general_pipeline_state_options options;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		F_nsl_root_signature_selection root_signature_selection;
#endif

		F_nsl_pipeline_state_config_map config_map;

		u32 begin_location = 0;
		u32 end_location = 0;
		TK<F_nsl_translation_unit> translation_unit_p;

	};
	using F_nsl_pipeline_state = eastl::pair<G_string, F_nsl_pipeline_state_info>;

	using F_nsl_input_assembler_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_input_attribute {

		G_string semantic;
		u32 buffer = 0;
		u32 offset = NCPP_U32_MAX;
		ED_input_classification classification = ED_input_classification::PER_VERTEX_DATA;

	};
	using F_nsl_input_attribute_config_map = TG_unordered_map<G_string, F_nsl_info_tree_reader>;
	struct F_nsl_input_assembler_info {

		TG_vector<F_nsl_input_attribute> attributes;

		F_input_assembler_desc desc;

		F_nsl_input_assembler_config_map config_map;

		u32 begin_location = 0;
		u32 end_location = 0;
		TK<F_nsl_translation_unit> translation_unit_p;

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

	struct FE_nsl_name_types
	{
		struct DATA_TYPE {};
		struct SEMANTIC {};
		struct STRUCTURE {};
		struct RESOURCE {};
		struct RESOURCE_TYPE {};
		struct UNIFORM {};
		struct SAMPLER_STATE {};
		struct PIPELINE_STATE {};
		struct SHADER {};
		struct SUBMODULE {};
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		struct ROOT_SIGNATURE {};
#endif
	};

	enum class E_nsl_primitive_data_type {

		NONE,

		B8,
		I32,
		U16,
		U32,
		U64,
		F16,
		F32,
		F64,

		B8X2,
		I32X2,
		U32X2,
		F16X2,
		F32X2,
		F64X2,

		B8X3,
		I32X3,
		U32X3,
		F16X3,
		F32X3,
		F64X3,

		B8X4,
		I32X4,
		U32X4,
		F16X4,
		F32X4,
		F64X4,

		B8X2X2,
		I32X2X2,
		U32X2X2,
		F16X2X2,
		F32X2X2,
		F64X2X2,

		B8X3X3,
		I32X3X3,
		U32X3X3,
		F16X3X3,
		F32X3X3,
		F64X3X3,

		B8X4X4,
		I32X4X4,
		U32X4X4,
		F16X4X4,
		F32X4X4,
		F64X4X4

	};

	enum class E_nsl_type_class {

		NONE,
		PRIMITIVE,
		STRUCTURE

	};

	using F_nsl_object_config = TG_unordered_map<G_string, F_nsl_info_tree_reader>;

	struct F_nsl_semantic_info {

		G_string target_type;
		G_string target_binding;
		E_nsl_element_format element_format = E_nsl_element_format::NONE;
		u32 element_count = 1;

	};
	using F_nsl_semantic = eastl::pair<G_string, F_nsl_semantic_info>;

	struct F_nsl_context {

		TK<A_nsl_object> parent_object_p;

		TG_stack<F_nsl_object_type_channel_mask> object_type_channel_mask_stack = { nsl_default_object_type_channel_mask };

		F_nsl_object_config temp_object_config;
		F_nsl_object_config current_object_config;
		TG_stack<F_nsl_object_config> object_config_stack;

		G_string default_constant_buffer;
		F_general_pipeline_state_options default_pipeline_state_options;

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

	struct F_nsl_shader_reflection {

		G_string name;

		ED_shader_type type = ED_shader_type::NONE;

		F_vector3_u thread_group_size = F_vector3_u::zero();
	};
	struct F_nsl_pipeline_state_reflection {

		G_string name;

		ED_pipeline_state_type type = ED_pipeline_state_type::NONE;

		F_general_pipeline_state_options options;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		F_nsl_root_signature_selection root_signature_selection;
#endif

		TG_vector<u32> shader_indices;

	};
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	struct F_nsl_root_signature_reflection {

		G_string name;

		F_root_signature_desc desc;

	};
#endif
	struct F_nsl_sampler_state_reflection {

		G_string name;

		F_sampler_state_desc desc;
		b8 is_static = false;
		b8 is_comparison_state = false;

		TG_vector<u32> actual_slots;
		TG_vector<u32> actual_slot_spaces;

	public:
		NCPP_FORCE_INLINE TG_vector<u32> shader_indices() const {

			u32 slot_count = actual_slots.size();

			TG_vector<u32> result;

			for(u32 i = 0; i < slot_count; ++i) {

				if(actual_slots[i] != -1)
					result.push_back(i);
			}

			return std::move(result);
		}

	};
	struct F_nsl_data_argument_reflection {

		G_string name;

		u32 type_index = NCPP_U32_MAX;

		u32 count = 1;
		b8 is_array = false;

		u32 offset = 0;

	};
	struct F_nsl_resource_reflection {

		G_string name;

		E_nsl_resource_type type = E_nsl_resource_type::NONE;
		E_nsl_resource_type_class type_class = E_nsl_resource_type_class::NONE;
		TG_vector<G_string> type_args;

		F_resource_dimension_sizes dimension_sizes = { 1 };
		u32 dimension_count = 1;
		u32 is_array = false;

		TG_vector<u32> actual_slots;
		TG_vector<u32> actual_slot_spaces;

		TG_vector<F_nsl_data_argument_reflection> data_arguments;
		u32 sort_uniforms = true;

		sz constant_size = 0;

		E_nsl_resource_flag flags = E_nsl_resource_flag::NONE;

	public:
		NCPP_FORCE_INLINE TG_vector<u32> shader_indices() const {

			u32 slot_count = actual_slots.size();

			TG_vector<u32> result;

			for(u32 i = 0; i < slot_count; ++i) {

				if(actual_slots[i] != -1)
					result.push_back(i);
			}

			return std::move(result);
		}

	};
	struct F_nsl_structure_reflection {

		TG_vector<F_nsl_data_argument_reflection> data_arguments;

	};
	struct F_nsl_type_reflection {

		G_string name;

		E_nsl_type_class type_class = E_nsl_type_class::NONE;

		E_nsl_primitive_data_type primitive_data_type = E_nsl_primitive_data_type::NONE;
		F_nsl_structure_reflection structure;

		sz size = 0;
		u32 alignment = 0;

	};
	struct NRHI_API F_nsl_reflection
	{
		TG_vector<F_nsl_shader_reflection> shaders;
		TG_vector<F_nsl_pipeline_state_reflection> pipeline_states;
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		TG_vector<F_nsl_root_signature_reflection> root_signatures;
#endif
		TG_vector<F_nsl_sampler_state_reflection> sampler_states;
		TG_vector<F_nsl_resource_reflection> resources;
		TG_vector<F_nsl_type_reflection> types;

	public:
		NCPP_FORCE_INLINE u32 search_shader_index(const G_string& name) const noexcept {

			u32 shader_count = shaders.size();
			for(u32 i = 0; i < shader_count; ++i) {

				if(shaders[i].name == name)
					return i;
			}

			return -1;
		}
		NCPP_FORCE_INLINE F_nsl_shader_reflection& search_shader(const G_string& name) noexcept {

			u32 shader_index = search_shader_index(name);

			NCPP_ASSERT(shader_index != -1)
				<< "not found shader "
				<< T_cout_value(name);

			return shaders[shader_index];
		}
		NCPP_FORCE_INLINE const F_nsl_shader_reflection& search_shader(const G_string& name) const noexcept {

			u32 shader_index = search_shader_index(name);

			NCPP_ASSERT(shader_index != -1)
				<< "not found shader "
				<< T_cout_value(name);

			return shaders[shader_index];
		}

	public:
		NCPP_FORCE_INLINE u32 search_pipeline_state_index(const G_string& name) const noexcept {

			u32 pipeline_state_count = pipeline_states.size();
			for(u32 i = 0; i < pipeline_state_count; ++i) {

				if(pipeline_states[i].name == name)
					return i;
			}

			return -1;
		}
		NCPP_FORCE_INLINE F_nsl_pipeline_state_reflection& search_pipeline_state(const G_string& name) noexcept {

			u32 pipeline_state_index = search_pipeline_state_index(name);

			NCPP_ASSERT(pipeline_state_index != -1)
				<< "not found pipeline state "
				<< T_cout_value(name);

			return pipeline_states[pipeline_state_index];
		}
		NCPP_FORCE_INLINE const F_nsl_pipeline_state_reflection& search_pipeline_state(const G_string& name) const noexcept {

			u32 pipeline_state_index = search_pipeline_state_index(name);

			NCPP_ASSERT(pipeline_state_index != -1)
				<< "not found pipeline state "
				<< T_cout_value(name);

			return pipeline_states[pipeline_state_index];
		}

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	public:
		NCPP_FORCE_INLINE u32 search_root_signature_index(const G_string& name) const noexcept {

			u32 root_signature_count = root_signatures.size();
			for(u32 i = 0; i < root_signature_count; ++i) {

				if(root_signatures[i].name == name)
					return i;
			}

			return -1;
		}
		NCPP_FORCE_INLINE F_nsl_root_signature_reflection& search_root_signature(const G_string& name) noexcept {

			u32 root_signature_index = search_root_signature_index(name);

			NCPP_ASSERT(root_signature_index != -1)
				<< "not found root signature "
				<< T_cout_value(name);

			return root_signatures[root_signature_index];
		}
		NCPP_FORCE_INLINE const F_nsl_root_signature_reflection& search_root_signature(const G_string& name) const noexcept {

			u32 root_signature_index = search_root_signature_index(name);

			NCPP_ASSERT(root_signature_index != -1)
				<< "not found root signature "
				<< T_cout_value(name);

			return root_signatures[root_signature_index];
		}
#endif

	public:
		NCPP_FORCE_INLINE u32 search_sampler_state_index(const G_string& name) const noexcept {

			u32 sampler_state_count = sampler_states.size();
			for(u32 i = 0; i < sampler_state_count; ++i) {

				if(sampler_states[i].name == name)
					return i;
			}

			return -1;
		}
		NCPP_FORCE_INLINE F_nsl_sampler_state_reflection& search_sampler_state(const G_string& name) noexcept {

			u32 sampler_state_index = search_sampler_state_index(name);

			NCPP_ASSERT(sampler_state_index != -1)
				<< "not found sampler state "
				<< T_cout_value(name);

			return sampler_states[sampler_state_index];
		}
		NCPP_FORCE_INLINE const F_nsl_sampler_state_reflection& search_sampler_state(const G_string& name) const noexcept {

			u32 sampler_state_index = search_sampler_state_index(name);

			NCPP_ASSERT(sampler_state_index != -1)
				<< "not found sampler state "
				<< T_cout_value(name);

			return sampler_states[sampler_state_index];
		}

	public:
		NCPP_FORCE_INLINE u32 search_resource_index(const G_string& name) const noexcept {

			u32 resource_count = resources.size();
			for(u32 i = 0; i < resource_count; ++i) {

				if(resources[i].name == name)
					return i;
			}

			return -1;
		}
		NCPP_FORCE_INLINE F_nsl_resource_reflection& search_resource(const G_string& name) noexcept {

			u32 resource_index = search_resource_index(name);

			NCPP_ASSERT(resource_index != -1)
				<< "not found resource "
				<< T_cout_value(name);

			return resources[resource_index];
		}
		NCPP_FORCE_INLINE const F_nsl_resource_reflection& search_resource(const G_string& name) const noexcept {

			u32 resource_index = search_resource_index(name);

			NCPP_ASSERT(resource_index != -1)
				<< "not found resource "
				<< T_cout_value(name);

			return resources[resource_index];
		}

	public:
		NCPP_FORCE_INLINE u32 search_type_index(const G_string& name) const noexcept {

			u32 type_count = types.size();
			for(u32 i = 0; i < type_count; ++i) {

				if(types[i].name == name)
					return i;
			}

			return -1;
		}
		NCPP_FORCE_INLINE F_nsl_type_reflection& search_type(const G_string& name) noexcept {

			u32 type_index = search_type_index(name);

			NCPP_ASSERT(type_index != -1)
				<< "not found type "
				<< T_cout_value(name);

			return types[type_index];
		}
		NCPP_FORCE_INLINE const F_nsl_type_reflection& search_type(const G_string& name) const noexcept {

			u32 type_index = search_type_index(name);

			NCPP_ASSERT(type_index != -1)
				<< "not found type "
				<< T_cout_value(name);

			return types[type_index];
		}

	public:
		void sort_sampler_states(
			const TG_vector<TG_unordered_set<G_string>>& sampler_state_name_sets
		);
		void sort_resources(
			const TG_vector<TG_unordered_set<G_string>>& resource_name_sets,
			E_nsl_resource_type_class type_class
		);
	};

	struct NRHI_API F_nsl_compiled_result {

		G_string class_name;

		G_string src_content;
		E_nsl_output_language output_language_enum = E_nsl_output_language::NONE;

		F_nsl_reflection reflection;

		u32 shader_count = 0;

		TG_vector<F_shader_binary> shader_binaries;

	public:
		G_string build(u32 shader_index) const;

		NCPP_FORCE_INLINE b8 is_finalized() const noexcept {

			return (shader_binaries.size() != 0);
		}
		void finalize();
		void finalize_and_release_src_content();

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
		TG_unordered_map<G_string, G_string> system_source_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, TK<F_nsl_translation_unit>>& abs_path_to_translation_unit_p() const noexcept { return abs_path_to_translation_unit_p_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, G_string>& system_source_map() const noexcept { return system_source_map_; }



	public:
		F_nsl_shader_module_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_shader_module_manager();

	public:
		NCPP_OBJECT(F_nsl_shader_module_manager);

	public:
		NCPP_FORCE_INLINE b8 is_has_system_source(const G_string& path) const noexcept {

			auto it = system_source_map_.find(path);

			return (it != system_source_map_.end());
		}
		void register_system_source(const G_string& path, const G_string src_content);

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
		virtual TK<F_nsl_translation_unit> make_virtual(
			const G_string& abs_path,
			const G_string& src_content,
			TKPA_valid<F_nsl_translation_unit> from_unit_p,
			const F_nsl_ast_tree& tree
		);

	public:
		void register_translation_unit(const G_string& abs_path, TKPA_valid<F_nsl_translation_unit> translation_unit_p);
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
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
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

	private:
		TK<F_nsl_translation_unit> imported_unit_p_;

	public:
		NCPP_FORCE_INLINE TKPA<F_nsl_translation_unit> imported_unit_p() const noexcept { return imported_unit_p_; }



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
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
		);
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



	class NRHI_API F_nsl_submodule_object final : public A_nsl_object {

	private:
		G_string content_;
		F_nsl_ast_tree ast_tree_;

	public:
		u32 next_id = 0;

	public:
		NCPP_FORCE_INLINE const auto& content() const noexcept { return content_; }
		NCPP_FORCE_INLINE const auto& ast_tree() const noexcept { return ast_tree_; }



	public:
		F_nsl_submodule_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_submodule_object();

	public:
		NCPP_OBJECT(F_nsl_submodule_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;
	};



	class NRHI_API F_nsl_submodule_object_type final : public A_nsl_object_type {

	public:
		F_nsl_submodule_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_submodule_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_submodule_object_type);

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
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
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
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
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
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
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
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
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
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
		);

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
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
		);

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
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
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



	class NRHI_API F_nsl_default_constant_buffer_object : public A_nsl_object {

	public:
		F_nsl_default_constant_buffer_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_default_constant_buffer_object();

	public:
		NCPP_OBJECT(F_nsl_default_constant_buffer_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_default_constant_buffer_object_type : public A_nsl_object_type {

	public:
		F_nsl_default_constant_buffer_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_default_constant_buffer_object_type();

	public:
		NCPP_OBJECT(F_nsl_default_constant_buffer_object_type);

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
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
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



	class NRHI_API F_nsl_pipeline_state_object : public A_nsl_object
	{
	private:
		F_general_pipeline_state_options options_;

	protected:
		b8 is_fake_ = false;

	public:
		NCPP_FORCE_INLINE const auto& options() const noexcept { return options_; }

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

	private:
		F_input_assembler_desc convert_to_input_assembler_desc(const F_nsl_input_assembler_info& input_assembler_info);

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



	class NRHI_API F_nsl_default_pipeline_state_object : public F_nsl_pipeline_state_object {

	public:
		F_nsl_default_pipeline_state_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_default_pipeline_state_object();

	public:
		NCPP_OBJECT(F_nsl_default_pipeline_state_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;

	};



	class NRHI_API F_nsl_default_pipeline_state_object_type : public A_nsl_object_type {

	public:
		F_nsl_default_pipeline_state_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_default_pipeline_state_object_type();

	public:
		NCPP_OBJECT(F_nsl_default_pipeline_state_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	};



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	class NRHI_API F_nsl_root_signature_object : public A_nsl_object
	{
	public:
		F_nsl_root_signature_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_root_signature_object();

	public:
		NCPP_OBJECT(F_nsl_root_signature_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;
	};



	class NRHI_API F_nsl_root_signature_object_type : public A_nsl_object_type {

	public:
		F_nsl_root_signature_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_root_signature_object_type();

	public:
		NCPP_OBJECT(F_nsl_root_signature_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};
#endif



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	class NRHI_API F_nsl_descriptor_heap_getter_object : public A_nsl_object
	{
	private:
		ED_descriptor_heap_type descriptor_heap_type_;
		E_nsl_resource_type resource_type_ = E_nsl_resource_type::NONE;

	public:
		NCPP_FORCE_INLINE ED_descriptor_heap_type descriptor_heap_type() const noexcept { return descriptor_heap_type_; }
		NCPP_FORCE_INLINE E_nsl_resource_type resource_type() const noexcept { return resource_type_; }



	public:
		F_nsl_descriptor_heap_getter_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			ED_descriptor_heap_type descriptor_heap_type = ED_descriptor_heap_type::CONSTANT_BUFFER_SHADER_RESOURCE_UNORDERED_ACCESS,
			E_nsl_resource_type resource_type = E_nsl_resource_type::NONE,
			const G_string& name = ""
		);
		virtual ~F_nsl_descriptor_heap_getter_object() = default;

	public:
		NCPP_OBJECT(F_nsl_descriptor_heap_getter_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
		) override;
	};



	class NRHI_API F_nsl_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			const G_string& name = "$",
			b8 is_object_name_required = false,
			u32 min_body_count = 1,
			u32 max_body_count = 1
		);
		virtual ~F_nsl_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_constant_buffer_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_constant_buffer_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_constant_buffer_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_constant_buffer_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_buffer_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_buffer_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_buffer_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_buffer_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_byte_address_buffer_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_byte_address_buffer_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_byte_address_buffer_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_byte_address_buffer_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_structured_buffer_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_structured_buffer_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_structured_buffer_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_structured_buffer_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_texture_1d_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_texture_1d_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_texture_1d_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_texture_1d_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_texture_1d_array_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_texture_1d_array_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_texture_1d_array_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_texture_1d_array_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_texture_2d_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_texture_2d_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_texture_2d_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_texture_2d_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_texture_2d_array_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_texture_2d_array_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_texture_2d_array_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_texture_2d_array_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_texture_3d_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_texture_3d_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_texture_3d_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_texture_3d_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_texture_cube_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_texture_cube_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_texture_cube_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_texture_cube_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_texture_cube_array_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_texture_cube_array_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_texture_cube_array_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_texture_cube_array_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_rw_buffer_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_rw_buffer_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_rw_buffer_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_rw_buffer_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_rw_byte_address_buffer_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_rw_byte_address_buffer_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_rw_byte_address_buffer_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_rw_byte_address_buffer_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_rw_structured_buffer_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_rw_structured_buffer_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_rw_structured_buffer_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_rw_structured_buffer_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_rw_texture_1d_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_rw_texture_1d_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_rw_texture_1d_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_rw_texture_1d_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_rw_texture_1d_array_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_rw_texture_1d_array_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_rw_texture_1d_array_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_rw_texture_1d_array_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_rw_texture_2d_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_rw_texture_2d_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_rw_texture_2d_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_rw_texture_2d_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_rw_texture_2d_array_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_rw_texture_2d_array_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_rw_texture_2d_array_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_rw_texture_2d_array_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};



	class NRHI_API F_nsl_rw_texture_3d_descriptor_heap_getter_object_type : public A_nsl_object_type
	{
	public:
		F_nsl_rw_texture_3d_descriptor_heap_getter_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_rw_texture_3d_descriptor_heap_getter_object_type() = default;

	public:
		NCPP_OBJECT(F_nsl_rw_texture_3d_descriptor_heap_getter_object_type);

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;
	};
#endif



	class NRHI_API A_nsl_shader_object : public A_nsl_object {

	private:
		ED_shader_type type_ = ED_shader_type::NONE;

	protected:
		TG_vector<F_nsl_data_param> data_params_;
		F_vector3_u thread_group_size_ = F_vector3_u::zero();
		E_nsl_output_topology output_topology_ = E_nsl_output_topology::TRIANGLE;

	public:
		u32 index = NCPP_U32_MAX;
		u32 default_slot_space = 0;

	public:
		NCPP_FORCE_INLINE const auto& data_params() const noexcept { return data_params_; }
		NCPP_FORCE_INLINE auto type() const noexcept { return type_; }
		NCPP_FORCE_INLINE const F_vector3_u& thread_group_size() const noexcept { return thread_group_size_; }
		NCPP_FORCE_INLINE E_nsl_output_topology output_topology() const noexcept { return output_topology_; }



	protected:
		A_nsl_shader_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			ED_shader_type type,
			const G_string& name = ""
		);

	public:
		virtual ~A_nsl_shader_object();

	public:
		NCPP_OBJECT(A_nsl_shader_object);

	protected:
		eastl::optional<G_string> apply_shader_with_customizations(
			const F_nsl_ast_tree& tree,
			const G_string& pre_shader_keyword = ""
		);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
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

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
		) override;

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



#ifdef NRHI_DRIVER_SUPPORT_AMPLIFICATION_SHADER
	class NRHI_API F_nsl_amplification_shader_object final : public A_nsl_shader_object {

	public:
		F_nsl_amplification_shader_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_amplification_shader_object();

	public:
		NCPP_OBJECT(F_nsl_amplification_shader_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
		) override;

	};



	class NRHI_API F_nsl_amplification_shader_object_type final : public A_nsl_shader_object_type {

	public:
		F_nsl_amplification_shader_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_amplification_shader_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_amplification_shader_object_type);

	};
#endif



#ifdef NRHI_DRIVER_SUPPORT_MESH_SHADER
	class NRHI_API F_nsl_mesh_shader_object final : public A_nsl_shader_object {

	public:
		F_nsl_mesh_shader_object(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			TKPA_valid<A_nsl_object_type> type_p,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name = ""
		);
		virtual ~F_nsl_mesh_shader_object();

	public:
		NCPP_OBJECT(F_nsl_mesh_shader_object);

	public:
		virtual eastl::optional<TG_vector<F_nsl_ast_tree>> recursive_build_ast_tree(
			F_nsl_context& context,
			TK_valid<F_nsl_translation_unit> unit_p,
			TG_vector<F_nsl_ast_tree>& trees,
			sz index,
			F_nsl_error_stack* error_stack_p
		) override;
		virtual eastl::optional<G_string> apply(
			const F_nsl_ast_tree& tree
		) override;

	};



	class NRHI_API F_nsl_mesh_shader_object_type final : public A_nsl_shader_object_type {

	public:
		F_nsl_mesh_shader_object_type(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p
		);
		virtual ~F_nsl_mesh_shader_object_type();

	public:
		virtual TK<A_nsl_object> create_object(
			F_nsl_ast_tree& tree,
			F_nsl_context& context,
			TKPA_valid<F_nsl_translation_unit> translation_unit_p
		) override;

	public:
		NCPP_OBJECT(F_nsl_mesh_shader_object_type);
	};
#endif



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

	public:
		friend class F_nsl_translation_unit_compiler;

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

		G_string raw_src_content_;
		G_string abs_path_;
		F_nsl_preprocessed_src preprocessed_src_;

		TK_valid<F_nsl_error_group> error_group_p_;

		TG_vector<TK_valid<F_nsl_translation_unit>> dependency_p_vector_;

		TG_vector<F_nsl_ast_tree> ast_trees_;
		TU<A_nsl_section_storage> section_storage_p_;

		b8 is_prepared_ = false;
		b8 is_compiled_ = false;

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

		NCPP_FORCE_INLINE b8 is_prepared() const noexcept { return is_prepared_; }
		NCPP_FORCE_INLINE b8 is_compiled() const noexcept { return is_compiled_; }



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

		TG_vector<TK<F_nsl_translation_unit>> sorted_unit_p_vector_;

		F_nsl_compiled_result compiled_result_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE TKPA<F_nsl_translation_unit> main_unit_p() const noexcept { return main_unit_p_; }
		NCPP_FORCE_INLINE const auto& sorted_unit_p_vector() const noexcept { return sorted_unit_p_vector_; }

		NCPP_FORCE_INLINE const auto& compiled_result() const noexcept { return compiled_result_; }



	public:
		F_nsl_translation_unit_compiler(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_translation_unit_compiler();

	public:
		NCPP_OBJECT(F_nsl_translation_unit_compiler);

	private:
		b8 sort_units_internal();
		b8 setup_shaders();
		b8 setup_sampler_state_actual_slots();
		b8 setup_srv_resource_actual_slots();
		b8 setup_uav_resource_actual_slots();
		b8 setup_cbv_resource_actual_slots();
		b8 bind_uniforms_to_constant_buffers_internal();
		b8 setup_constant_buffers_internal();
		b8 apply_internal();

	public:
		eastl::optional<G_string> compile_unit(TKPA_valid<F_nsl_translation_unit> unit_p);

	protected:
		b8 compile_minimal();

	public:
		b8 prepare_unit(TK_valid<F_nsl_translation_unit> unit_p, F_nsl_context& context);
		b8 prepare_units(
			const G_string& raw_src_content,
			const G_string& abs_path
		);
		eastl::optional<F_nsl_compiled_result> compile();

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

	public:
		eastl::optional<G_string> ast_trees_to_string(const TG_vector<F_nsl_ast_tree>& ast_trees);

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
//			NCPP_ASSERT(is_name_has_target(target) || (name == target)) << T_cout_value(target) << " is not registered";

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
		TG_unordered_map<G_string, E_nsl_primitive_data_type> name_to_primitive_data_type_map_;
		TG_unordered_map<G_string, E_nsl_type_class> name_to_type_class_map_;
		TG_unordered_map<G_string, E_nsl_element_format> name_to_element_format_map_;
		TG_unordered_map<G_string, u32> name_to_element_count_map_;
		TG_unordered_map<G_string, F_nsl_semantic_info> name_to_semantic_info_map_;
		TG_unordered_map<G_string, F_nsl_structure_info> name_to_structure_info_map_;
		TG_unordered_map<G_string, F_nsl_enumeration_info> name_to_enumeration_info_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE TG_unordered_map<G_string, sz>& name_to_size_map() noexcept { return name_to_size_map_; }
		NCPP_FORCE_INLINE TG_unordered_map<G_string, u32>& name_to_alignment_map() noexcept { return name_to_alignment_map_; }
		NCPP_FORCE_INLINE TG_unordered_map<G_string, E_nsl_primitive_data_type>& name_to_primitive_data_type_map() noexcept { return name_to_primitive_data_type_map_; }
		NCPP_FORCE_INLINE TG_unordered_map<G_string, E_nsl_type_class>& name_to_type_class_map() noexcept { return name_to_type_class_map_; }
		NCPP_FORCE_INLINE TG_unordered_map<G_string, E_nsl_element_format>& name_to_element_format_map() noexcept { return name_to_element_format_map_; }
		NCPP_FORCE_INLINE TG_unordered_map<G_string, u32>& name_to_element_count_map() noexcept { return name_to_element_count_map_; }
		NCPP_FORCE_INLINE TG_unordered_map<G_string, F_nsl_semantic_info>& name_to_semantic_info_map() noexcept { return name_to_semantic_info_map_; }
		NCPP_FORCE_INLINE TG_unordered_map<G_string, F_nsl_structure_info>& name_to_structure_info_map() noexcept { return name_to_structure_info_map_; }
		NCPP_FORCE_INLINE TG_unordered_map<G_string, F_nsl_enumeration_info>& name_to_enumeration_info_map() noexcept { return name_to_enumeration_info_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, sz>& name_to_size_map() const noexcept { return name_to_size_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, u32>& name_to_alignment_map() const noexcept { return name_to_alignment_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, E_nsl_primitive_data_type>& name_to_primitive_data_type_map() const noexcept { return name_to_primitive_data_type_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, E_nsl_type_class>& name_to_type_class_map() const noexcept { return name_to_type_class_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, E_nsl_element_format>& name_to_element_format_map() const noexcept { return name_to_element_format_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, u32>& name_to_element_count_map() const noexcept { return name_to_element_count_map_; }
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
		NCPP_FORCE_INLINE b8 is_name_has_primitive_data_type(const G_string& name) const {

			auto it = name_to_primitive_data_type_map_.find(name);

			return (it != name_to_primitive_data_type_map_.end());
		}
		NCPP_FORCE_INLINE E_nsl_primitive_data_type primitive_data_type(const G_string& name) const {

			auto it = name_to_primitive_data_type_map_.find(name);

			NCPP_ASSERT(it != name_to_primitive_data_type_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_primitive_data_type(const G_string& name, E_nsl_primitive_data_type primitive_data_type) {

			NCPP_ASSERT(name_to_primitive_data_type_map_.find(name) == name_to_primitive_data_type_map_.end()) << T_cout_value(name) << " already exists";

			name_to_primitive_data_type_map_[name] = primitive_data_type;
		}
		NCPP_FORCE_INLINE void deregister_primitive_data_type(const G_string& name) {

			NCPP_ASSERT(name_to_primitive_data_type_map_.find(name) != name_to_primitive_data_type_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_primitive_data_type_map_.find(name);
			name_to_primitive_data_type_map_.erase(it);
		}

	public:
		NCPP_FORCE_INLINE b8 is_name_has_type_class(const G_string& name) const {

			auto it = name_to_type_class_map_.find(name);

			return (it != name_to_type_class_map_.end());
		}
		NCPP_FORCE_INLINE E_nsl_type_class type_class(const G_string& name) const {

			auto it = name_to_type_class_map_.find(name);

			NCPP_ASSERT(it != name_to_type_class_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_type_class(const G_string& name, E_nsl_type_class type_class) {

			NCPP_ASSERT(name_to_type_class_map_.find(name) == name_to_type_class_map_.end()) << T_cout_value(name) << " already exists";

			name_to_type_class_map_[name] = type_class;
		}
		NCPP_FORCE_INLINE void deregister_type_class(const G_string& name) {

			NCPP_ASSERT(name_to_type_class_map_.find(name) != name_to_type_class_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_type_class_map_.find(name);
			name_to_type_class_map_.erase(it);
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
		NCPP_FORCE_INLINE b8 is_name_has_element_count(const G_string& name) const {

			auto it = name_to_element_count_map_.find(name);

			return (it != name_to_element_count_map_.end());
		}
		NCPP_FORCE_INLINE u32 element_count(const G_string& name) const {

			auto it = name_to_element_count_map_.find(name);

			NCPP_ASSERT(it != name_to_element_count_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_element_count(const G_string& name, u32 element_count) {

			NCPP_ASSERT(name_to_element_count_map_.find(name) == name_to_element_count_map_.end()) << T_cout_value(name) << " already exists";

			name_to_element_count_map_[name] = element_count;
		}
		NCPP_FORCE_INLINE void deregister_element_count(const G_string& name) {

			NCPP_ASSERT(name_to_element_count_map_.find(name) != name_to_element_count_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_element_count_map_.find(name);
			name_to_element_count_map_.erase(it);
		}

	public:
		NCPP_FORCE_INLINE b8 is_name_has_semantic_info(const G_string& name) const {

			auto it = name_to_semantic_info_map_.find(name);

			return (it != name_to_semantic_info_map_.end());
		}
		NCPP_FORCE_INLINE F_nsl_semantic_info& semantic_info(const G_string& name) {

			auto it = name_to_semantic_info_map_.find(name);

			NCPP_ASSERT(it != name_to_semantic_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
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
		NCPP_FORCE_INLINE F_nsl_structure_info& structure_info(const G_string& name) {

			auto it = name_to_structure_info_map_.find(name);

			NCPP_ASSERT(it != name_to_structure_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
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
		NCPP_FORCE_INLINE F_nsl_enumeration_info& enumeration_info(const G_string& name) {

			auto it = name_to_enumeration_info_map_.find(name);

			NCPP_ASSERT(it != name_to_enumeration_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
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

	public:
		G_string register_slot_macro(const G_string& name);
		G_string register_slot_space_macro(const G_string& name);

	public:
		virtual b8 is_support(E_nsl_feature feature);

	public:
		virtual eastl::optional<G_string> src_header() = 0;

	public:
		virtual eastl::optional<G_string> define_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name,
			const G_string& target
		) = 0;
		virtual eastl::optional<G_string> undef_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name
		) = 0;
		virtual eastl::optional<G_string> sampler_state_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const F_nsl_sampler_state& sampler_state
		) = 0;
		virtual eastl::optional<G_string> resource_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const F_nsl_resource& resource
		) = 0;
		virtual eastl::optional<G_string> structure_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const F_nsl_structure& structure
		) = 0;
		virtual eastl::optional<G_string> enumeration_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const F_nsl_enumeration& enumeration
		) = 0;
		virtual eastl::optional<G_string> shader_object_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			TKPA_valid<A_nsl_shader_object> shader_object_p,
			const G_string& body,
			const G_string& pre_shader_keyword
		) = 0;
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		virtual eastl::optional<G_string> descriptor_heap_getter_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const F_nsl_ast_tree& tree,
			const G_string& index_str
		);
#endif
	};



	class NRHI_API H_nsl_output_language
	{
	public:
		static E_nsl_output_language default_as_enum(TKPA_valid<A_device> device_p)
		{
#ifdef NRHI_DRIVER_DIRECTX_12
			if(driver_index() == NRHI_DRIVER_INDEX_DIRECTX_12)
			{
				auto highest_shader_model = HD_directx12_device::hlsl_highest_shader_model(device_p);

				if(highest_shader_model.first > 6)
					return E_nsl_output_language::HLSL_6_7;

				if(highest_shader_model.first == 6)
				{
					if(highest_shader_model.second == 0)
						return E_nsl_output_language::HLSL_6_0;
					if(highest_shader_model.second == 1)
						return E_nsl_output_language::HLSL_6_1;
					if(highest_shader_model.second == 2)
						return E_nsl_output_language::HLSL_6_2;
					if(highest_shader_model.second == 3)
						return E_nsl_output_language::HLSL_6_3;
					if(highest_shader_model.second == 4)
						return E_nsl_output_language::HLSL_6_4;
					if(highest_shader_model.second == 5)
						return E_nsl_output_language::HLSL_6_5;
					if(highest_shader_model.second == 6)
						return E_nsl_output_language::HLSL_6_6;
					if(highest_shader_model.second == 7)
						return E_nsl_output_language::HLSL_6_7;
				}

				return E_nsl_output_language::HLSL_5_1;
			}
#endif
#ifdef NRHI_DRIVER_DIRECTX_11
			if(driver_index() == NRHI_DRIVER_INDEX_DIRECTX_11)
				return E_nsl_output_language::HLSL_5;
#endif
#ifdef NRHI_DRIVER_VULKAN
#error "Vulkan is not supported"
			//			if(driver_index() == NRHI_DRIVER_INDEX_VULKAN)
			//				return E_nsl_output_language::HLSL_5;
#endif
#ifdef NRHI_DRIVER_METAL
#error "Metal is not supported"
			//			if(driver_index() == NRHI_DRIVER_INDEX_METAL)
			//				return E_nsl_output_language::HLSL_5;
#endif
#ifdef NRHI_DRIVER_OPENGL
#error "OpenGL is not supported"
			//			if(driver_index() == NRHI_DRIVER_INDEXOPENGL)
			//				return E_nsl_output_language::HLSL_5;
#endif
			return E_nsl_output_language::NONE;
		}
	};



	class NRHI_API A_nsl_output_hlsl : public A_nsl_output_language {

	protected:
		A_nsl_output_hlsl(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language output_language_as_enum
		);

	public:
		virtual ~A_nsl_output_hlsl();

	public:
		NCPP_OBJECT(A_nsl_output_hlsl);

	private:
		void register_data_types_internal();

	public:
		virtual eastl::optional<G_string> src_header() override;

	public:
		virtual eastl::optional<G_string> define_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name,
			const G_string& target
		) override;
		virtual eastl::optional<G_string> undef_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const G_string& name
		) override;
		virtual eastl::optional<G_string> sampler_state_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const F_nsl_sampler_state& sampler_state
		) override;
		virtual eastl::optional<G_string> resource_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const F_nsl_resource& resource
		) override;
		virtual eastl::optional<G_string> structure_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const F_nsl_structure& structure
		) override;
		virtual eastl::optional<G_string> enumeration_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const F_nsl_enumeration& enumeration
		) override;
		virtual eastl::optional<G_string> shader_object_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			TKPA_valid<A_nsl_shader_object> shader_object_p,
			const G_string& body,
			const G_string& pre_shader_keyword
		) override;

	protected:
		eastl::optional<G_string> resource_to_string_with_customization(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const F_nsl_resource& resource,
			const G_string& sub_name_keyword = ""
		);

	};



	class NRHI_API F_nsl_output_hlsl_4 : public A_nsl_output_hlsl {

	public:
		F_nsl_output_hlsl_4(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language output_language_as_enum = E_nsl_output_language::HLSL_4
		);
		virtual ~F_nsl_output_hlsl_4();

	public:
		NCPP_OBJECT(F_nsl_output_hlsl_4);

	private:
		void register_data_types_internal();

	};



	class NRHI_API F_nsl_output_hlsl_5 : public F_nsl_output_hlsl_4 {

	public:
		F_nsl_output_hlsl_5(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language output_language_as_enum = E_nsl_output_language::HLSL_5
		);
		virtual ~F_nsl_output_hlsl_5();

	public:
		NCPP_OBJECT(F_nsl_output_hlsl_5);

	private:
		void register_data_types_internal();

	};



	class NRHI_API F_nsl_output_hlsl_5_1 : public F_nsl_output_hlsl_5 {

	public:
		F_nsl_output_hlsl_5_1(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language output_language_as_enum = E_nsl_output_language::HLSL_5_1
		);
		virtual ~F_nsl_output_hlsl_5_1();

	public:
		NCPP_OBJECT(F_nsl_output_hlsl_5_1);

	private:
		void register_data_types_internal();

	public:
		virtual eastl::optional<G_string> src_header() override;

	public:
		virtual eastl::optional<G_string> resource_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const F_nsl_resource& resource
		) override;

	};



	class NRHI_API F_nsl_output_hlsl_6_0 : public F_nsl_output_hlsl_5_1 {

	public:
		F_nsl_output_hlsl_6_0(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language output_language_as_enum = E_nsl_output_language::HLSL_6_0
		);
		virtual ~F_nsl_output_hlsl_6_0();

	public:
		NCPP_OBJECT(F_nsl_output_hlsl_6_0);

	private:
		void register_data_types_internal();

	public:
		virtual eastl::optional<G_string> src_header() override;
	};



	class NRHI_API F_nsl_output_hlsl_6_1 : public F_nsl_output_hlsl_6_0 {

	public:
		F_nsl_output_hlsl_6_1(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language output_language_as_enum = E_nsl_output_language::HLSL_6_1
		);
		virtual ~F_nsl_output_hlsl_6_1();

	public:
		NCPP_OBJECT(F_nsl_output_hlsl_6_1);

	private:
		void register_data_types_internal();
	};



	class NRHI_API F_nsl_output_hlsl_6_2 : public F_nsl_output_hlsl_6_1 {

	public:
		F_nsl_output_hlsl_6_2(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language output_language_as_enum = E_nsl_output_language::HLSL_6_2
		);
		virtual ~F_nsl_output_hlsl_6_2();

	public:
		NCPP_OBJECT(F_nsl_output_hlsl_6_2);

	private:
		void register_data_types_internal();

	public:
		virtual eastl::optional<G_string> src_header() override;
	};



	class NRHI_API F_nsl_output_hlsl_6_3 : public F_nsl_output_hlsl_6_2 {

	public:
		F_nsl_output_hlsl_6_3(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language output_language_as_enum = E_nsl_output_language::HLSL_6_3
		);
		virtual ~F_nsl_output_hlsl_6_3();

	public:
		NCPP_OBJECT(F_nsl_output_hlsl_6_3);

	private:
		void register_data_types_internal();
	};



	class NRHI_API F_nsl_output_hlsl_6_4 : public F_nsl_output_hlsl_6_3 {

	public:
		F_nsl_output_hlsl_6_4(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language output_language_as_enum = E_nsl_output_language::HLSL_6_4
		);
		virtual ~F_nsl_output_hlsl_6_4();

	public:
		NCPP_OBJECT(F_nsl_output_hlsl_6_4);

	private:
		void register_data_types_internal();
	};



	class NRHI_API F_nsl_output_hlsl_6_5 : public F_nsl_output_hlsl_6_4 {

	public:
		F_nsl_output_hlsl_6_5(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language output_language_as_enum = E_nsl_output_language::HLSL_6_5
		);
		virtual ~F_nsl_output_hlsl_6_5();

	public:
		NCPP_OBJECT(F_nsl_output_hlsl_6_5);

	private:
		void register_data_types_internal();
	};



	class NRHI_API F_nsl_output_hlsl_6_6 : public F_nsl_output_hlsl_6_5 {

	public:
		F_nsl_output_hlsl_6_6(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language output_language_as_enum = E_nsl_output_language::HLSL_6_6
		);
		virtual ~F_nsl_output_hlsl_6_6();

	public:
		NCPP_OBJECT(F_nsl_output_hlsl_6_6);

	private:
		void register_data_types_internal();

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	public:
		virtual eastl::optional<G_string> descriptor_heap_getter_to_string(
			TKPA_valid<F_nsl_translation_unit> translation_unit_p,
			const F_nsl_ast_tree& tree,
			const G_string& index_str
		) override;
#endif
	};



	class NRHI_API F_nsl_output_hlsl_6_7 : public F_nsl_output_hlsl_6_6 {

	public:
		F_nsl_output_hlsl_6_7(
			TKPA_valid<F_nsl_shader_compiler> shader_compiler_p,
			E_nsl_output_language output_language_as_enum = E_nsl_output_language::HLSL_6_7
		);
		virtual ~F_nsl_output_hlsl_6_7();

	public:
		NCPP_OBJECT(F_nsl_output_hlsl_6_7);

	private:
		void register_data_types_internal();
	};



	class NRHI_API F_nsl_shader_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TG_unordered_map<G_string, TK<A_nsl_shader_object>> name_to_shader_object_p_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE TG_unordered_map<G_string, TK<A_nsl_shader_object>>& name_to_shader_object_p_map() noexcept { return name_to_shader_object_p_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, TK<A_nsl_shader_object>>& name_to_shader_object_p_map() const noexcept { return name_to_shader_object_p_map_; }



	public:
		F_nsl_shader_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_shader_manager();

	public:
		NCPP_OBJECT(F_nsl_shader_manager);

	public:
		NCPP_FORCE_INLINE b8 is_name_has_shader_object_p(const G_string& name) const {

			auto it = name_to_shader_object_p_map_.find(name);

			return (it != name_to_shader_object_p_map_.end());
		}
		NCPP_FORCE_INLINE TK_valid<A_nsl_shader_object> shader_object_p(const G_string& name) const {

			auto it = name_to_shader_object_p_map_.find(name);

			NCPP_ASSERT(it != name_to_shader_object_p_map_.end()) << "can't find " << T_cout_value(name);

			return NCPP_FOH_VALID(it->second);
		}
		NCPP_FORCE_INLINE void register_shader(const G_string& name, TKPA_valid<A_nsl_shader_object> shader_object_p) {

			NCPP_ASSERT(name_to_shader_object_p_map_.find(name) == name_to_shader_object_p_map_.end()) << T_cout_value(name) << " already exists";

			name_to_shader_object_p_map_[name] = shader_object_p.no_requirements();
		}
		NCPP_FORCE_INLINE void deregister_shader(const G_string& name) {

			NCPP_ASSERT(name_to_shader_object_p_map_.find(name) != name_to_shader_object_p_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_shader_object_p_map_.find(name);
			name_to_shader_object_p_map_.erase(it);
		}

	};



	class NRHI_API F_nsl_resource_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TG_unordered_map<G_string, F_nsl_resource_info> name_to_resource_info_map_;
		TG_unordered_map<G_string, E_nsl_resource_type_class> name_to_resource_type_class_map_;
		TG_unordered_map<G_string, E_nsl_resource_type> name_to_resource_type_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE TG_unordered_map<G_string, F_nsl_resource_info>& name_to_resource_info_map() noexcept { return name_to_resource_info_map_; }
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
		NCPP_FORCE_INLINE F_nsl_resource_info& resource_info(const G_string& name) {

			auto it = name_to_resource_info_map_.find(name);

			NCPP_ASSERT(it != name_to_resource_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
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

		u32 next_uniform_creation_index_ = 0;

	protected:
		TG_unordered_map<G_string, F_nsl_uniform_info> name_to_uniform_info_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE TG_unordered_map<G_string, F_nsl_uniform_info>& name_to_uniform_info_map() noexcept { return name_to_uniform_info_map_; }
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
		NCPP_FORCE_INLINE F_nsl_uniform_info& uniform_info(const G_string& name) {

			auto it = name_to_uniform_info_map_.find(name);

			NCPP_ASSERT(it != name_to_uniform_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
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

		NCPP_FORCE_INLINE TG_unordered_map<G_string, F_nsl_sampler_state_info>& name_to_sampler_state_info_map() noexcept { return name_to_sampler_state_info_map_; }
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
		NCPP_FORCE_INLINE F_nsl_sampler_state_info& sampler_state_info(const G_string& name) {

			auto it = name_to_sampler_state_info_map_.find(name);

			NCPP_ASSERT(it != name_to_sampler_state_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
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

		NCPP_FORCE_INLINE TG_unordered_map<G_string, F_nsl_pipeline_state_info>& name_to_pipeline_state_info_map() noexcept { return name_to_pipeline_state_info_map_; }
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
		NCPP_FORCE_INLINE F_nsl_pipeline_state_info& pipeline_state_info(const G_string& name) {

			auto it = name_to_pipeline_state_info_map_.find(name);

			NCPP_ASSERT(it != name_to_pipeline_state_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
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



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	class NRHI_API F_nsl_root_signature_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TG_unordered_map<G_string, F_nsl_root_signature_info> name_to_root_signature_info_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE TG_unordered_map<G_string, F_nsl_root_signature_info>& name_to_root_signature_info_map() noexcept { return name_to_root_signature_info_map_; }
		NCPP_FORCE_INLINE const TG_unordered_map<G_string, F_nsl_root_signature_info>& name_to_root_signature_info_map() const noexcept { return name_to_root_signature_info_map_; }



	public:
		F_nsl_root_signature_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_root_signature_manager();

	public:
		NCPP_OBJECT(F_nsl_root_signature_manager);

	public:
		NCPP_FORCE_INLINE b8 is_name_has_root_signature_info(const G_string& name) const {

			auto it = name_to_root_signature_info_map_.find(name);

			return (it != name_to_root_signature_info_map_.end());
		}
		NCPP_FORCE_INLINE F_nsl_root_signature_info& root_signature_info(const G_string& name) {

			auto it = name_to_root_signature_info_map_.find(name);

			NCPP_ASSERT(it != name_to_root_signature_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE const F_nsl_root_signature_info& root_signature_info(const G_string& name) const {

			auto it = name_to_root_signature_info_map_.find(name);

			NCPP_ASSERT(it != name_to_root_signature_info_map_.end()) << "can't find " << T_cout_value(name);

			return it->second;
		}
		NCPP_FORCE_INLINE void register_root_signature(const G_string& name, const F_nsl_root_signature_info& root_signature_info) {

			NCPP_ASSERT(name_to_root_signature_info_map_.find(name) == name_to_root_signature_info_map_.end()) << T_cout_value(name) << " already exists";

			name_to_root_signature_info_map_[name] = process_root_signature_info(name, root_signature_info);
		}
		NCPP_FORCE_INLINE void deregister_root_signature(const G_string& name) {

			NCPP_ASSERT(name_to_root_signature_info_map_.find(name) != name_to_root_signature_info_map_.end()) << T_cout_value(name) << " is not exists";

			auto it = name_to_root_signature_info_map_.find(name);
			name_to_root_signature_info_map_.erase(it);
		}

	private:
		F_nsl_root_signature_info process_root_signature_info(const G_string& name, const F_nsl_root_signature_info& root_signature_info);
	};
#endif



	class NRHI_API F_nsl_submodule_manager {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	protected:
		TG_unordered_map<G_string, TK<F_nsl_submodule_object>> name_to_submodule_object_p_map_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }

		NCPP_FORCE_INLINE auto& name_to_submodule_object_p_map() noexcept { return name_to_submodule_object_p_map_; }
		NCPP_FORCE_INLINE const auto& name_to_submodule_object_p_map() const noexcept { return name_to_submodule_object_p_map_; }



	public:
		F_nsl_submodule_manager(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_submodule_manager();

	public:
		NCPP_OBJECT(F_nsl_submodule_manager);

	public:
		NCPP_FORCE_INLINE b8 is_name_registered(const G_string& name) const {

			auto it = name_to_submodule_object_p_map_.find(name);

			return (it != name_to_submodule_object_p_map_.end());
		}
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_submodule_object> submodule_object_p(const G_string& name) const {

			auto it = name_to_submodule_object_p_map_.find(name);

			NCPP_ASSERT(it != name_to_submodule_object_p_map_.end()) << "can't find " << T_cout_value(name);

			return (TKPA_valid<F_nsl_submodule_object>)it->second;
		}
		NCPP_FORCE_INLINE void register_submodule(const G_string& name, TKPA_valid<F_nsl_submodule_object> submodule_object_p) {

			NCPP_ASSERT(name_to_submodule_object_p_map_.find(name) == name_to_submodule_object_p_map_.end()) << T_cout_value(name) << " already exists";

			name_to_submodule_object_p_map_[name] = submodule_object_p.no_requirements();
		}
	};



	class NRHI_API F_nsl_reflector {

	private:
		TK_valid<F_nsl_shader_compiler> shader_compiler_p_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<F_nsl_shader_compiler> shader_compiler_p() const noexcept { return shader_compiler_p_; }



	public:
		F_nsl_reflector(TKPA_valid<F_nsl_shader_compiler> shader_compiler_p);
		virtual ~F_nsl_reflector();

	public:
		NCPP_OBJECT(F_nsl_reflector);

	public:
		F_nsl_reflection reflect();

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
			F_nsl_shader_manager,
			shader_manager_creator
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
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_root_signature_manager,
			root_signature_manager_creator
		);
#endif
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_submodule_manager,
			submodule_manager_creator
		);
		NRHI_NSL_DEFINE_SUBSYSTEM_CREATOR_AS_CUSTOMIZATION_MEMBER(
			F_nsl_reflector,
			reflector_creator
		);

	};

	class NRHI_API F_nsl_shader_compiler {

	private:
		TU<F_nsl_shader_module_manager> shader_module_manager_p_;
		TU<F_nsl_translation_unit_manager> translation_unit_manager_p_;
		TU<F_nsl_translation_unit_compiler> translation_unit_compiler_p_;
		TU<F_nsl_error_storage> error_storage_p_;
		TU<F_nsl_object_manager> object_manager_p_;
		TU<F_nsl_name_manager> name_manager_p_;
		TU<F_nsl_data_type_manager> data_type_manager_p_;
		TU<F_nsl_shader_manager> shader_manager_p_;
		TU<F_nsl_resource_manager> resource_manager_p_;
		TU<F_nsl_uniform_manager> uniform_manager_p_;
		TU<F_nsl_sampler_state_manager> sampler_state_manager_p_;
		TU<F_nsl_pipeline_state_manager> pipeline_state_manager_p_;
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		TU<F_nsl_root_signature_manager> root_signature_manager_p_;
#endif
		TU<F_nsl_submodule_manager> submodule_manager_p_;
		TU<F_nsl_reflector> reflector_p_;

		TU<A_nsl_output_language> output_language_p_;

		b8 is_compiled_ = false;
		b8 is_compile_success_ = false;

	public:
		NCPP_FORCE_INLINE TK_valid<F_nsl_shader_module_manager> shader_module_manager_p() const noexcept { return NCPP_FOH_VALID(shader_module_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_translation_unit_manager> translation_unit_manager_p() const noexcept { return NCPP_FOH_VALID(translation_unit_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_translation_unit_compiler> translation_unit_compiler_p() const noexcept { return NCPP_FOH_VALID(translation_unit_compiler_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_error_storage> error_storage_p() const noexcept { return NCPP_FOH_VALID(error_storage_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_object_manager> object_manager_p() const noexcept { return NCPP_FOH_VALID(object_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_name_manager> name_manager_p() const noexcept { return NCPP_FOH_VALID(name_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_data_type_manager> data_type_manager_p() const noexcept { return NCPP_FOH_VALID(data_type_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_shader_manager> shader_manager_p() const noexcept { return NCPP_FOH_VALID(shader_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_resource_manager> resource_manager_p() const noexcept { return NCPP_FOH_VALID(resource_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_uniform_manager> uniform_manager_p() const noexcept { return NCPP_FOH_VALID(uniform_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_sampler_state_manager> sampler_state_manager_p() const noexcept { return NCPP_FOH_VALID(sampler_state_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_pipeline_state_manager> pipeline_state_manager_p() const noexcept { return NCPP_FOH_VALID(pipeline_state_manager_p_); }
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		NCPP_FORCE_INLINE TK_valid<F_nsl_root_signature_manager> root_signature_manager_p() const noexcept { return NCPP_FOH_VALID(root_signature_manager_p_); }
#endif
		NCPP_FORCE_INLINE TK_valid<F_nsl_submodule_manager> submodule_manager_p() const noexcept { return NCPP_FOH_VALID(submodule_manager_p_); }
		NCPP_FORCE_INLINE TK_valid<F_nsl_reflector> reflector_p() const noexcept { return NCPP_FOH_VALID(reflector_p_); }

		NCPP_FORCE_INLINE TK<A_nsl_output_language> output_language_p() const noexcept { return output_language_p_; }

		NCPP_FORCE_INLINE b8 is_compiled() const noexcept { return is_compiled_; }
		NCPP_FORCE_INLINE b8 is_compile_success() const noexcept { return is_compile_success_; }



	public:
		F_nsl_shader_compiler(
			const F_nsl_shader_compiler_customizer& customizer = F_nsl_shader_compiler_customizer {}
		);
		virtual ~F_nsl_shader_compiler();

	public:
		NCPP_OBJECT(F_nsl_shader_compiler);

	protected:
		virtual TU<A_nsl_output_language> create_output_language(E_nsl_output_language output_language_enum);

	public:
		eastl::optional<F_nsl_compiled_result> compile(
			const G_string& raw_src_content,
			const G_string& class_name,
			E_nsl_output_language output_language_enum,
			const G_string& abs_path = "",
			const TG_vector<eastl::pair<G_string, G_string>>& macros = {}
		);

	};

}
