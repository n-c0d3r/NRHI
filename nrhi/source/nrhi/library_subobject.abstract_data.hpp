#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/utilities/abstract_data.hpp>
#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
#include <nrhi/library_subobject.hpp>
#endif



#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
namespace nrhi
{
#ifdef NRHI_DRIVER_DIRECTX_12
#define NRHI_DRIVER_DIRECTX_12_LIBRARY_SUBOBJECT_TYPE CD3DX12_DXIL_LIBRARY_SUBOBJECT
#else
#define NRHI_DRIVER_DIRECTX_12_LIBRARY_SUBOBJECT_TYPE u8
#endif



    namespace internal
    {
        NRHI_DEFINE_ABSTRACT_DATA(
            A_library_subobject,
            H_library_subobject,
            NRHI_DRIVER_DIRECTX_12_LIBRARY_SUBOBJECT_TYPE
        );
    }



    struct NRHI_API F_library_subobject : public internal::A_library_subobject
    {
    public:
        NCPP_FORCE_INLINE F_library_subobject() noexcept :
            internal::A_library_subobject()
        {}
        NCPP_FORCE_INLINE ~F_library_subobject() noexcept
        {}

    public:
        NCPP_FORCE_INLINE void set_binary(const TG_span<u8>& binary) noexcept
        {
            H_library_subobject::set_binary(
                payload_p(),
                binary
            );
        }
    };
}
#endif