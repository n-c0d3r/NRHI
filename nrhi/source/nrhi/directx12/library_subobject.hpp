#pragma once

#include <nrhi/prerequisites.hpp>



namespace nrhi
{
    class NRHI_API HD_directx12_library_subobject
    {
    public:
        static NCPP_FORCE_INLINE void construct_default(void* payload_p) noexcept
        {
            new(payload_p) CD3DX12_DXIL_LIBRARY_SUBOBJECT();
        }
        static NCPP_FORCE_INLINE void destruct(void* payload_p) noexcept
        {
            ((CD3DX12_DXIL_LIBRARY_SUBOBJECT*)payload_p)->~CD3DX12_DXIL_LIBRARY_SUBOBJECT();
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
        static NCPP_FORCE_INLINE void set_binary(void* payload_p, const TG_span<u8>& binary) noexcept
        {
            CD3DX12_SHADER_BYTECODE d3d12_shader_bytecode(
                (const void*)(binary.data()),
                sz(binary.size())
            );

            ((CD3DX12_DXIL_LIBRARY_SUBOBJECT*)payload_p)->SetDXILLibrary(
                &d3d12_shader_bytecode
            );
        }
    };
}