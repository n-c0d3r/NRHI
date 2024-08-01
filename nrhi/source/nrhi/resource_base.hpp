#pragma once

/** @file nrhi/resource_base.hpp
*
*   Implement resource base class.
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

#include <nrhi/device_child.hpp>
#include <nrhi/resource_desc.hpp>
#include <nrhi/resource_type.hpp>
#include <nrhi/mapped_subresource.hpp>
#include <nrhi/resource_gpu_virtual_address.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
	class A_resource_heap;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT



	u64& inject_resource_generation(TKPA_valid<A_resource> resource_p) noexcept;



    class NRHI_API A_resource : public A_device_child {

	public:
		friend u64& nrhi::inject_resource_generation(TKPA_valid<A_resource> resource_p) noexcept;



    private:
        F_resource_desc desc_;
        F_initial_resource_data initial_data_;

	protected:
		u64 generation_ = 0;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		u64 heap_offset_;
		TK<A_object> memory_p_;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

    public:
        NCPP_FORCE_INLINE const F_resource_desc& desc() const noexcept { return desc_; }
        NCPP_FORCE_INLINE const F_initial_resource_data& initial_data() const noexcept { return initial_data_; }

		NCPP_FORCE_INLINE u64 generation() const noexcept { return generation_; }
		NCPP_FORCE_INLINE void set_generation_unsafe(u64 value) noexcept { generation_ = value; }

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		NCPP_FORCE_INLINE u64 heap_offset() const noexcept {

			NCPP_ASSERT(management_type() == E_resource_management_type::PLACED);
			return heap_offset_;
		}
		NCPP_FORCE_INLINE void set_heap_offset_unsafe(u64 value) noexcept {

			NCPP_ASSERT(management_type() == E_resource_management_type::PLACED);
			heap_offset_ = value;
		}
		NCPP_FORCE_INLINE TKPA<A_object> memory_p() const noexcept { return memory_p_; }
		NCPP_FORCE_INLINE void set_memory_p_unsafe(TKPA<A_object> value) noexcept { memory_p_ = value; }
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT



    protected:
        A_resource(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            ED_resource_type overrided_type
        );

    public:
        virtual ~A_resource();

	public:
		NCPP_OBJECT(A_resource);

	public:
		virtual void rebuild(
			const F_initial_resource_data& initial_data,
			const F_resource_desc& desc
		);

	protected:
		void finalize_rebuild(
			const F_initial_resource_data& initial_data,
			const F_resource_desc& desc
		);

	public:
		F_mapped_subresource map(u32 subresource_index);
		void unmap(u32 subresource_index);

	public:
		F_resource_gpu_virtual_address gpu_virtual_address();

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
		virtual E_resource_management_type management_type() const;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

    };



	NCPP_FORCE_INLINE F_resource_desc& inject_resource_desc(TKPA_valid<A_resource> resource_p) noexcept {

		return (F_resource_desc&)(resource_p->desc());
	}
	NCPP_FORCE_INLINE u64& inject_resource_generation(TKPA_valid<A_resource> resource_p) noexcept {

		return resource_p->generation_;
	}

}
