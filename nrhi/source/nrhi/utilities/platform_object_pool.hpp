#pragma once

/** @file nrhi/utilities/platform_object_pool.hpp
*
*   Implement platform object pool.
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

#pragma endregion



namespace nrhi {

	class A_device;



	namespace utilities {

		template<class F_pool__, typename F_platform_object__, typename F_input__>
		class TF_platform_object_pool {

		public:
			using F_pool = F_pool__;
			using F_platform_object = F_platform_object__;
			using F_input = F_input__;

		public:
			struct F_platform_object_wrapper {
				F_platform_object object;
				u64 counter = 0;
			};
			using F_platform_object_wrapper_map = TG_map<u64, F_platform_object_wrapper>;

#ifdef NRHI_THREAD_SAFE
		private:
			F_spin_lock spin_lock_;
#endif



		public:
			static inline void initialize() {

				F_pool__::default_id = hash_input(F_input__ {});

				F_pool__::map.clear();
			}
			static inline void release() {

				F_pool__::map.clear();
			}



		private:
			static NCPP_FORCE_INLINE u64 hash_input(const F_input& input) {

				return input.hash_members();
			}

		public:
			static inline F_platform_object acquire_object(TKPA_valid<A_device> device_p, const F_input& input) {

				u64 id = hash_input(input);

				if(F_pool__::default_id == id)
					return 0;

				F_platform_object result;

#ifdef NRHI_THREAD_SAFE
				spin_lock_.lock();
#endif

				auto it = F_pool::map.find(id);

				if(it != F_pool::map.end()) {
					auto& wrapper = it->second;
					result = wrapper.object;
					++(wrapper.counter);
				}
				else
				{
					result = F_pool::create_object(device_p, input);
					F_pool::map[id] = F_platform_object_wrapper{
						.object = result,
						.counter = 1
					};
				}

#ifdef NRHI_THREAD_SAFE
				spin_lock_.unlock();
#endif

				return result;
			}
			static inline void release_object(TKPA_valid<A_device> device_p, const F_input& input) {

				u64 id = hash_input(input);

				if(F_pool__::default_id == id)
					return;

#ifdef NRHI_THREAD_SAFE
				spin_lock_.lock();
#endif

				auto it = F_pool::map.find(id);

				NCPP_ASSERT(it != F_pool::map.end()) << "invalid input, object not acquired";

				if(0 == (--(it->second.counter))) {

					F_pool::destroy_object(device_p, it->second.object, input);
				}

#ifdef NRHI_THREAD_SAFE
				spin_lock_.unlock();
#endif
			}

		};

		template<typename... Fs__>
		auto T_platform_object_member_tuple(Fs__&&... members)
			-> TG_tuple<std::remove_const_t<std::remove_reference_t<Fs__>>...>
		{

			return {
				members...
			};
		}

#define NRHI_PLATFORM_OBJECT_POOL_GENERATED_BODY() \
			public:                                         \
				static F_platform_object_wrapper_map map;\
				static ncpp::u64 default_id;

#define NRHI_PLATFORM_OBJECT_POOL_DEFINE(...) \
			typename __VA_ARGS__::F_platform_object_wrapper_map __VA_ARGS__::map; \
			ncpp::u64 __VA_ARGS__::default_id = 0;

#define NRHI_PLATFORM_OBJECT_HASH_MEMBERS_FUNCTION(...) \
			ncpp::u64 hash_members() const noexcept {            \
            	return ncpp::TF_hash_type_memory<          \
					decltype(nrhi::utilities::T_platform_object_member_tuple(__VA_ARGS__)) \
				>()(                                                \
					nrhi::utilities::T_platform_object_member_tuple(__VA_ARGS__)    \
				);                                    \
			}

	}

}
