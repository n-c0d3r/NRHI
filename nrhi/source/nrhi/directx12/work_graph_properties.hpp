#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/work_graph_desc.hpp>



namespace nrhi
{
    class A_state_object;



    class NRHI_API HD_directx12_work_graph_properties
    {
    public:
        static NCPP_FORCE_INLINE void construct_default(void* payload_p) noexcept
        {
            new(payload_p) ID3D12WorkGraphProperties*(0);
        }
        static NCPP_FORCE_INLINE void destruct(void* payload_p) noexcept
        {
        }
        static NCPP_FORCE_INLINE void construct_copy(void* payload_p, void* x_payload_p) noexcept
        {
            new(payload_p) ID3D12WorkGraphProperties*(
                *((ID3D12WorkGraphProperties**)x_payload_p)
            );
        }
        static NCPP_FORCE_INLINE void operator_copy(void* payload_p, void* x_payload_p) noexcept
        {
            *((ID3D12WorkGraphProperties**)payload_p) = *((ID3D12WorkGraphProperties**)x_payload_p);
        }
        static NCPP_FORCE_INLINE void construct_move(void* payload_p, void* x_payload_p) noexcept
        {
            new(payload_p) ID3D12WorkGraphProperties*(
                *((ID3D12WorkGraphProperties**)x_payload_p)
            );
        }
        static NCPP_FORCE_INLINE void operator_move(void* payload_p, void* x_payload_p) noexcept
        {
            *((ID3D12WorkGraphProperties**)payload_p) = *((ID3D12WorkGraphProperties**)x_payload_p);
        }

    public:
        static void construct_with_state_object(
            void* payload_p,
            TKPA_valid<A_state_object> state_object_p
        );

    public:
        static NCPP_FORCE_INLINE b8 compare(void* payload_p, void* x_payload_p) noexcept
        {
            return (*((ID3D12WorkGraphProperties**)payload_p) == *((ID3D12WorkGraphProperties**)x_payload_p));
        }
    };
}