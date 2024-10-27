#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/state_object_flag.hpp>



namespace nrhi
{
    class NRHI_API HD_directx12_state_object_config_subobject
    {
    public:
        static NCPP_FORCE_INLINE void construct_default(void* payload_p) noexcept
        {
            new(payload_p) CD3DX12_STATE_OBJECT_CONFIG_SUBOBJECT();
        }
        static NCPP_FORCE_INLINE void destruct(void* payload_p) noexcept
        {
            ((CD3DX12_STATE_OBJECT_CONFIG_SUBOBJECT*)payload_p)->~CD3DX12_STATE_OBJECT_CONFIG_SUBOBJECT();
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
        static NCPP_FORCE_INLINE void set_flags(void* payload_p, ED_state_object_flag flags) noexcept
        {
            ((CD3DX12_STATE_OBJECT_CONFIG_SUBOBJECT*)payload_p)->SetFlags(D3D12_STATE_OBJECT_FLAGS(flags));
        }
    };
}
