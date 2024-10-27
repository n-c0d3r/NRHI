#pragma once

#include <nrhi/prerequisites.hpp>



namespace nrhi
{
    class A_root_signature;



    class NRHI_API HD_directx12_local_root_signature_subobject
    {
    public:
        static NCPP_FORCE_INLINE void construct_default(void* payload_p) noexcept
        {
            new(payload_p) CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT();
        }
        static NCPP_FORCE_INLINE void destruct(void* payload_p) noexcept
        {
            ((CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT*)payload_p)->~CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT();
        }
        static NCPP_FORCE_INLINE void construct_copy(void* payload_p, void* x_payload_p) noexcept
        {
            NCPP_ASSERT(false) << "not allowed";
        }
        static NCPP_FORCE_INLINE void operator_copy(void* payload_p, void* x_payload_p) noexcept
        {
            NCPP_ASSERT(false) << "not allowed";
        }
        static NCPP_FORCE_INLINE void construct_move(void* payload_p, void* x_payload_p) noexcept
        {
            NCPP_ASSERT(false) << "not allowed";
        }
        static NCPP_FORCE_INLINE void operator_move(void* payload_p, void* x_payload_p) noexcept
        {
            NCPP_ASSERT(false) << "not allowed";
        }

    public:
        static void set_root_signature_p(void* payload_p, TKPA_valid<A_root_signature> root_signature_p);
    };
}
