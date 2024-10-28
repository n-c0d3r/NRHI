#pragma once

/** @file nrhi/nsl_factory.hpp
*
*   Implement nsl factory desc.
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

#include <nrhi/nsl_shader_compiler.hpp>
#include <nrhi/state_object_builder.abstract_data.hpp>

#pragma endregion



namespace nrhi {

	class A_device;
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	class A_root_signature;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	class A_pipeline_state;



	class NRHI_API H_nsl_factory
	{
	public:
#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
		static TG_vector<TU<A_pipeline_state>> create_pipeline_states(
			TKPA_valid<A_device> device_p,
			const F_nsl_compiled_result& compiled_result
		);
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		static TG_vector<TU<A_pipeline_state>> create_pipeline_states_with_root_signature(
			TKPA_valid<A_device> device_p,
			const F_nsl_compiled_result& compiled_result,
			const TG_unordered_map<G_string, TK<A_root_signature>>& root_signature_map = {}
		);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
	public:
		static TG_vector<TU<A_root_signature>> create_root_signatures(
			TKPA_valid<A_device> device_p,
			const F_nsl_compiled_result& compiled_result
		);
		static TG_unordered_map<G_string, TK<A_root_signature>> make_root_signature_map(
			const TG_span<TK_valid<A_root_signature>>& root_signature_p_span,
			const F_nsl_compiled_result& compiled_result
		);
		struct F_owned_root_signature_map
		{
			TG_unordered_map<G_string, TK<A_root_signature>> map;
			TG_vector<TU<A_root_signature>> root_signature_p_vector;
		};
		static F_owned_root_signature_map make_owned_root_signature_map(
			TKPA_valid<A_device> device_p,
			const F_nsl_compiled_result& compiled_result
		)
		{
			F_owned_root_signature_map result;

			result.root_signature_p_vector = create_root_signatures(
				device_p,
				compiled_result
			);
			result.map = make_root_signature_map(
				{
					(TK_valid<A_root_signature>*)(result.root_signature_p_vector.data()),
					result.root_signature_p_vector.size()
				},
				compiled_result
			);

			return eastl::move(result);
		}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
	public:
		struct F_owned_state_object_builder
		{
			F_state_object_builder builder;
			F_owned_root_signature_map owned_root_signature_map;
		};
		static F_state_object_builder make_state_object_builder(
			const F_nsl_compiled_result& compiled_result,
			const TG_unordered_map<G_string, TK<A_root_signature>>& root_signature_map,
			ED_state_object_type state_object_type = ED_state_object_type::EXECUTABLE
		);
		static F_owned_state_object_builder make_owned_state_object_builder(
			TKPA_valid<A_device> device_p,
			const F_nsl_compiled_result& compiled_result,
			ED_state_object_type state_object_type = ED_state_object_type::EXECUTABLE
		)
		{
			F_owned_root_signature_map owned_root_signature_map = make_owned_root_signature_map(device_p, compiled_result);
			F_state_object_builder state_object_builder = make_state_object_builder(
				compiled_result,
				owned_root_signature_map.map,
				state_object_type
			);
			return {
				.builder = eastl::move(state_object_builder),
				.owned_root_signature_map = eastl::move(owned_root_signature_map)
			};
		}
#endif
	};
}
