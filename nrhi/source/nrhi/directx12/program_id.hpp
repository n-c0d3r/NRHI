#pragma once

#include <nrhi/prerequisites.hpp>



namespace nrhi
{
    class NRHI_API HD_directx12_program_id
    {
    public:
        static NCPP_FORCE_INLINE void construct_default(void* payload_p) noexcept
        {
        }
        static NCPP_FORCE_INLINE void destruct(void* payload_p) noexcept
        {
        }
        static NCPP_FORCE_INLINE void construct_copy(void* payload_p, void* x_payload_p) noexcept
        {
            new(payload_p) D3D12_PROGRAM_IDENTIFIER(
                *((D3D12_PROGRAM_IDENTIFIER*)x_payload_p)
            );
        }
        static NCPP_FORCE_INLINE void operator_copy(void* payload_p, void* x_payload_p) noexcept
        {
            *((D3D12_PROGRAM_IDENTIFIER*)payload_p) = *((D3D12_PROGRAM_IDENTIFIER*)x_payload_p);
        }
        static NCPP_FORCE_INLINE void construct_move(void* payload_p, void* x_payload_p) noexcept
        {
            new(payload_p) D3D12_PROGRAM_IDENTIFIER(
                eastl::move(
                    *((D3D12_PROGRAM_IDENTIFIER*)x_payload_p)
                )
            );
        }
        static NCPP_FORCE_INLINE void operator_move(void* payload_p, void* x_payload_p) noexcept
        {
            *((D3D12_PROGRAM_IDENTIFIER*)payload_p) = eastl::move(*((D3D12_PROGRAM_IDENTIFIER*)x_payload_p));
        }
    };
}