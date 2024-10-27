#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/program_id.abstract_data.hpp>



namespace nrhi
{
    class A_state_object;



    class NRHI_API HD_directx12_state_object_properties
    {
    public:
        static NCPP_FORCE_INLINE void construct_default(void* payload_p) noexcept
        {
            new(payload_p) ID3D12StateObjectProperties*(0);
        }
        static NCPP_FORCE_INLINE void destruct(void* payload_p) noexcept
        {
        }
        static NCPP_FORCE_INLINE void construct_copy(void* payload_p, void* x_payload_p) noexcept
        {
            new(payload_p) ID3D12StateObjectProperties*(
                *((ID3D12StateObjectProperties**)x_payload_p)
            );
        }
        static NCPP_FORCE_INLINE void operator_copy(void* payload_p, void* x_payload_p) noexcept
        {
            *((ID3D12StateObjectProperties**)payload_p) = *((ID3D12StateObjectProperties**)x_payload_p);
        }
        static NCPP_FORCE_INLINE void construct_move(void* payload_p, void* x_payload_p) noexcept
        {
            new(payload_p) ID3D12StateObjectProperties*(
                *((ID3D12StateObjectProperties**)x_payload_p)
            );
        }
        static NCPP_FORCE_INLINE void operator_move(void* payload_p, void* x_payload_p) noexcept
        {
            *((ID3D12StateObjectProperties**)payload_p) = *((ID3D12StateObjectProperties**)x_payload_p);
        }

    public:
        static void construct_with_state_object(
            void* payload_p,
            TKPA_valid<A_state_object> state_object_p
        );

    public:
        static NCPP_FORCE_INLINE b8 compare(void* payload_p, void* x_payload_p) noexcept
        {
            return (*((ID3D12StateObjectProperties**)payload_p) == *((ID3D12StateObjectProperties**)x_payload_p));
        }

    public:
        static NCPP_FORCE_INLINE F_program_id program_id(void* payload_p, const G_string& name) noexcept
        {
            ID3D12StateObjectProperties* d3d12_state_object_properties_p = *((ID3D12StateObjectProperties**)payload_p);

            ID3D12StateObjectProperties1* d3d12_state_object_properties_1_p;
            HRESULT hr = d3d12_state_object_properties_p->QueryInterface(IID_PPV_ARGS(&d3d12_state_object_properties_1_p));
            NCPP_ASSERT(SUCCEEDED(hr));

            D3D12_PROGRAM_IDENTIFIER d3d12_program_identifier = d3d12_state_object_properties_1_p->GetProgramIdentifier(
                G_to_wstring(name).c_str()
            );

            return *((F_program_id*)&d3d12_program_identifier);
        }
    };
}