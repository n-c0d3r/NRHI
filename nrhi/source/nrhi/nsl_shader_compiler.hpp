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
		struct NRHI_API F_str_state {

			b8 value = false;
			b8 value_1 = false; // for '
			b8 value_2 = false; // for "

			b8 prev_value = false;
			b8 prev_value_1 = false; // for '
			b8 prev_value_2 = false; // for "

		public:
			void begin_check(char c);
			void end_check();

		};

		struct NRHI_API F_space_segment {

		};
		struct NRHI_API F_variable_name_segment {

		};
		struct NRHI_API F_str_scope_segment {

		};
		struct NRHI_API F_brace_scope_segment {

		};
		struct NRHI_API F_parentheses_scope_segment {

		};
		struct NRHI_API F_segment_stream {

		};

		struct NRHI_API F_info_tree {

		};



	public:
		static b8 is_variable_name_character(char c);

	public:
		struct F_function_macro_use {
			sz begin_location = 0;
			sz end_location = 0;
			G_string arg;
		};
		using F_function_macro_result_functor = eastl::function<
			G_string(const G_string& arg)
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
		static eastl::optional<TG_vector<F_shader_kernel_desc>> search_kernel_descs(
			const G_string& src_content
		);

	};



	class NRHI_API F_nsl_shader_compiler {

	public:
		F_nsl_shader_compiler();
		~F_nsl_shader_compiler();

	public:
		NCPP_DISABLE_COPY(F_nsl_shader_compiler);

	protected:
		virtual eastl::optional<F_nsl_include_blob> load_include_blob(
			const G_string& path,
			const TG_span<F_shader_kernel_desc>& kernel_descs,
			u32 kernel_index
		);
		virtual G_string process_source(
			const G_string& src_content,
			const G_string& abs_path,
			const TG_span<F_shader_kernel_desc>& kernel_descs,
			u32 kernel_index
		);

	private:
		TU<A_shader_blob> compile_processed_source(
			const G_string& processed_src_content,
			const G_string& abs_path,
			const TG_span<F_shader_kernel_desc>& kernel_descs,
			u32 kernel_index
		);
		TU<A_shader_blob> compile_source(
			const G_string& src_content,
			const G_string& abs_path,
			const TG_span<F_shader_kernel_desc>& kernel_descs,
			u32 kernel_index
		);

	public:
		TU<A_shader_class> compile(
			const G_string& src_content,
			const G_string& abs_path
		);

	};

}
