#pragma once

/** @file nrhi/resource_view_base.hpp
*
*   Implement resource view base class.
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
#include <nrhi/resource_base.hpp>
#include <nrhi/resource_desc.hpp>
#include <nrhi/resource_view_desc.hpp>
#include <nrhi/resource_view_type.hpp>
#include <nrhi/resource_view_handle.hpp>

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#include <nrhi/descriptor_base.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;
    class A_resource_view;



	u64& inject_resource_view_generation(TKPA_valid<A_resource_view> resource_view_p) noexcept;



    class NRHI_API A_resource_view : public A_device_child
	{

	public:
		friend u64& nrhi::inject_resource_view_generation(TKPA_valid<A_resource_view> resource_view_p) noexcept;



    private:
        F_resource_view_desc desc_;
		ED_resource_type resource_type_;

	protected:
		u64 generation_ = 0;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		F_descriptor descriptor_;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

    public:
        NCPP_FORCE_INLINE const F_resource_view_desc& desc() const noexcept { return desc_; }
        NCPP_FORCE_INLINE ED_resource_type resource_type() const noexcept { return resource_type_; }

		NCPP_FORCE_INLINE u64 generation() const noexcept { return generation_; }
		NCPP_FORCE_INLINE void set_generation_unsafe(u64 value) noexcept { generation_ = value; }
		NCPP_FORCE_INLINE b8 is_valid_generation() const noexcept {

			return (generation_ == desc_.resource_p->generation());
		}

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		NCPP_FORCE_INLINE const F_descriptor& descriptor() const noexcept {

			return descriptor_;
		}
		NCPP_FORCE_INLINE void set_descriptor_unsafe(const F_descriptor& value) noexcept {

			descriptor_ = value;
		}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING



    protected:
        A_resource_view(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc,
            ED_resource_view_type overrided_type
        );
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		A_resource_view(
			TKPA_valid<A_device> device_p,
			const F_resource_view_desc& desc,
			const F_descriptor& descriptor,
			ED_resource_view_type overrided_type
		);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

    public:
        virtual ~A_resource_view();

	public:
		NCPP_OBJECT(A_resource_view);

	public:
		virtual void rebuild();
		virtual void rebuild(
			const F_resource_view_desc& desc
		);
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		virtual void rebuild_with_descriptor(
			const F_resource_view_desc& desc,
			const F_descriptor& descriptor
		);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		void guarantee_generation();

	protected:
		void finalize_rebuild();
		void finalize_rebuild(
			const F_resource_view_desc& desc
		);
#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		void finalize_rebuild_with_descriptor(
			const F_resource_view_desc& desc,
			const F_descriptor& descriptor
		);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

    };



	NCPP_FORCE_INLINE F_resource_view_desc& inject_resource_view_desc(TKPA_valid<A_resource_view> resource_view_p) noexcept {

		return (F_resource_view_desc&)(resource_view_p->desc());
	}
	NCPP_FORCE_INLINE u64& inject_resource_view_generation(TKPA_valid<A_resource_view> resource_view_p) noexcept {

		return resource_view_p->generation_;
	}

}
