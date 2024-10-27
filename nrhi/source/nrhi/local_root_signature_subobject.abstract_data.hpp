#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/utilities/abstract_data.hpp>
#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
#include <nrhi/local_root_signature_subobject.hpp>
#endif



#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
namespace nrhi
{
#ifdef NRHI_DRIVER_DIRECTX_12
#define NRHI_DRIVER_DIRECTX_12_LOCAL_ROOT_SIGNATURE_SUBOBJECT_TYPE CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT
#else
#define NRHI_DRIVER_DIRECTX_12_LOCAL_ROOT_SIGNATURE_SUBOBJECT_TYPE u8
#endif



    namespace internal
    {
        NRHI_DEFINE_ABSTRACT_DATA(
            A_local_root_signature_subobject,
            H_local_root_signature_subobject,
            NRHI_DRIVER_DIRECTX_12_LOCAL_ROOT_SIGNATURE_SUBOBJECT_TYPE
        );
    }



    struct NRHI_API F_local_root_signature_subobject : public internal::A_local_root_signature_subobject
    {
    public:
        NCPP_FORCE_INLINE F_local_root_signature_subobject() noexcept :
            internal::A_local_root_signature_subobject()
        {}
        NCPP_FORCE_INLINE ~F_local_root_signature_subobject() noexcept
        {}

    public:
        NCPP_FORCE_INLINE void set_root_signature_p(TKPA_valid<A_root_signature> root_signature_p)
        {
            H_local_root_signature_subobject::set_root_signature_p(
                (void*)this,
                root_signature_p
            );
        }
    };
}
#endif