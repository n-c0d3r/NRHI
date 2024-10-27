#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/utilities/abstract_data.hpp>
#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
#include <nrhi/state_object_builder.hpp>
#endif



#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
namespace nrhi
{
#ifdef NRHI_DRIVER_DIRECTX_12
#define NRHI_DRIVER_DIRECTX_12_STATE_OBJECT_BUILDER_TYPE CD3DX12_STATE_OBJECT_DESC
#else
#define NRHI_DRIVER_DIRECTX_12_STATE_OBJECT_BUILDER_TYPE u8
#endif



    namespace internal
    {
        NRHI_DEFINE_ABSTRACT_DATA(
            A_state_object_builder,
            H_state_object_builder,
            NRHI_DRIVER_DIRECTX_12_STATE_OBJECT_BUILDER_TYPE
        );
    }



    struct NRHI_API F_state_object_builder : public internal::A_state_object_builder
    {
    public:
        NCPP_FORCE_INLINE F_state_object_builder() noexcept :
            internal::A_state_object_builder()
        {}
        NCPP_FORCE_INLINE ~F_state_object_builder() noexcept
        {}
        NCPP_FORCE_INLINE F_state_object_builder(F_state_object_builder&& x) noexcept :
            internal::A_state_object_builder(eastl::move(x))
        {}
        NCPP_FORCE_INLINE F_state_object_builder& operator = (F_state_object_builder&& x) noexcept
        {
            ((internal::A_state_object_builder&)*this) = eastl::move(x);
            return *this;
        }

    public:
        NCPP_FORCE_INLINE F_state_object_builder(ED_state_object_type type) noexcept
        {
            H_state_object_builder::construct_with_type(
                this,
                type
            );
        }

    public:
        NCPP_FORCE_INLINE F_state_object_config_subobject& add_state_object_config() noexcept
        {
            return H_state_object_builder::add_state_object_config(
                this
            );
        }
        NCPP_FORCE_INLINE F_global_root_signature_subobject& add_global_root_signature_subobject() noexcept
        {
            return H_state_object_builder::add_global_root_signature_subobject(
                this
            );
        }
        NCPP_FORCE_INLINE F_local_root_signature_subobject& add_local_root_signature_subobject() noexcept
        {
            return H_state_object_builder::add_local_root_signature_subobject(
                this
            );
        }
        NCPP_FORCE_INLINE F_work_graph_subobject& add_work_graph() noexcept
        {
            return H_state_object_builder::add_work_graph(
                this
            );
        }
        NCPP_FORCE_INLINE F_library_subobject& add_library() noexcept
        {
            return H_state_object_builder::add_library(
                this
            );
        }

    public:
        NCPP_FORCE_INLINE TU<A_state_object> build(TKPA_valid<A_device> device_p) noexcept
        {
            return H_state_object_builder::build(
                this,
                device_p
            );
        }
        NCPP_FORCE_INLINE TU<A_state_object> add_to(TKPA_valid<A_state_object> src_state_object_p) noexcept
        {
            return H_state_object_builder::add_to(
                this,
                src_state_object_p
            );
        }
    };
}
#endif