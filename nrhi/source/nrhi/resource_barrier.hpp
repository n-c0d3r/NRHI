#pragma once

/** @file nrhi/resource_barrier.hpp
*
*   Implement resource barrier.
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

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
#include <nrhi/resource_state.hpp>
#include <nrhi/resource_barrier_type.hpp>
#include <nrhi/resource_barrier_flag.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#pragma endregion



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
namespace nrhi {

	class A_resource;



	static constexpr u32 resource_barrier_all_subresources = NCPP_U32_MAX;



	struct F_resource_transition_barrier {

		TK<A_resource> resource_p;
		u32 subresource_index = resource_barrier_all_subresources;
		ED_resource_state state_before;
		ED_resource_state state_after;

		b8 operator == (const F_resource_transition_barrier& other) const noexcept
		{
			return (
				(resource_p == other.resource_p)
				&& (subresource_index == other.subresource_index)
				&& (state_before == other.state_before)
				&& (state_after == other.state_after)
			);
		}
	};
	struct F_resource_aliasing_barrier {

		TK<A_resource> resource_before_p;
		TK<A_resource> resource_after_p;

		b8 operator == (const F_resource_aliasing_barrier& other) const noexcept
		{
			return (
				(resource_before_p == other.resource_before_p)
				&& (resource_after_p == other.resource_after_p)
			);
		}
	};
	struct F_resource_uav_barrier {

		TK<A_resource> resource_p;

		b8 operator == (const F_resource_uav_barrier& other) const noexcept
		{
			return (resource_p == other.resource_p);
		}
	};

	struct F_resource_barrier {

		static constexpr sz payload_size = (
			(
			   	sizeof(F_resource_uav_barrier)
				> (
					(sizeof(F_resource_transition_barrier) > sizeof(F_resource_aliasing_barrier))
					? sizeof(F_resource_transition_barrier)
					: sizeof(F_resource_aliasing_barrier)
				)
		   	)
		   	? sizeof(F_resource_uav_barrier)
		   	: (
			   	(sizeof(F_resource_transition_barrier) > sizeof(F_resource_aliasing_barrier))
			   	? sizeof(F_resource_transition_barrier)
			   	: sizeof(F_resource_aliasing_barrier)
		   	)
		);

		ED_resource_barrier_type type;
		ED_resource_barrier_flag flags = ED_resource_barrier_flag::NONE;

		union {

			F_resource_transition_barrier transition;
			F_resource_aliasing_barrier aliasing;
			F_resource_uav_barrier uav;

			u8 payload[payload_size] = {};

		};

		NCPP_FORCE_INLINE F_resource_barrier() noexcept
		{}
		NCPP_FORCE_INLINE ~F_resource_barrier() noexcept
		{}
		NCPP_FORCE_INLINE F_resource_barrier(const F_resource_barrier& x) noexcept :
			type(x.type),
			flags(x.flags)
		{
			memcpy(
				payload,
				x.payload,
				payload_size
			);
		}
		NCPP_FORCE_INLINE F_resource_barrier& operator = (const F_resource_barrier& x) noexcept
		{
			type = x.type;
			flags = x.flags;

			memcpy(
				payload,
				x.payload,
				payload_size
			);
			return *this;
		}

		b8 operator == (const F_resource_barrier& other) const noexcept
		{
			if(other.type != type)
				return false;
			if(other.flags != flags)
				return false;

			NRHI_ENUM_SWITCH(
				type,
				NRHI_ENUM_CASE(
					ED_resource_barrier_type::TRANSITION,
					return (transition == other.transition);
				)
				NRHI_ENUM_CASE(
					ED_resource_barrier_type::ALIASING,
					return (aliasing == other.aliasing);
				)
				NRHI_ENUM_CASE(
					ED_resource_barrier_type::UNORDERED_ACCESS,
					return (uav == other.uav);
				)
			);
		}
	};



	struct NRHI_API H_resource_barrier {

	public:
		static F_resource_barrier transition(
			const F_resource_transition_barrier& transition_barrier,
			ED_resource_barrier_flag flags = ED_resource_barrier_flag::NONE
		);
		static F_resource_barrier aliasing(
			const F_resource_aliasing_barrier& aliasing_barrier,
			ED_resource_barrier_flag flags = ED_resource_barrier_flag::NONE
		);
		static F_resource_barrier uav(
			const F_resource_uav_barrier& uav_barrier,
			ED_resource_barrier_flag flags = ED_resource_barrier_flag::NONE
		);
	};

}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
