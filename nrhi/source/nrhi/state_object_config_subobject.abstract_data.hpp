#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/utilities/abstract_data.hpp>
#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
#include <nrhi/state_object_config_subobject.hpp>
#endif



#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
namespace nrhi
{
#ifdef NRHI_DRIVER_DIRECTX_12
#define NRHI_DRIVER_DIRECTX_12_STATE_OBJECT_CONFIG_SUBOBJECT_TYPE CD3DX12_STATE_OBJECT_CONFIG_SUBOBJECT
#else
#define NRHI_DRIVER_DIRECTX_12_STATE_OBJECT_CONFIG_SUBOBJECT_TYPE u8
#endif



    namespace internal
    {
        NRHI_DEFINE_ABSTRACT_DATA(
            A_state_object_config_subobject,
            H_state_object_config_subobject,
            NRHI_DRIVER_DIRECTX_12_STATE_OBJECT_CONFIG_SUBOBJECT_TYPE
        );
    }



    struct NRHI_API F_state_object_config_subobject : public internal::A_state_object_config_subobject
    {
    public:
        NCPP_FORCE_INLINE F_state_object_config_subobject() noexcept :
            internal::A_state_object_config_subobject()
        {}
        NCPP_FORCE_INLINE ~F_state_object_config_subobject() noexcept
        {}

    public:
        NCPP_FORCE_INLINE void set_flags(ED_state_object_flag flags) noexcept
        {
            return H_state_object_config_subobject::set_flags(
                this,
                flags
            );
        }
    };
}
#endif