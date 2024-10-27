#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/debug_name.hpp>


namespace nrhi
{
    class NRHI_API HD_directx12_work_graph_subobject
    {
    public:
        static NCPP_FORCE_INLINE void construct_default(void* payload_p) noexcept
        {
            new(payload_p) CD3DX12_WORK_GRAPH_SUBOBJECT();
        }
        static NCPP_FORCE_INLINE void destruct(void* payload_p) noexcept
        {
            ((CD3DX12_WORK_GRAPH_SUBOBJECT*)payload_p)->~CD3DX12_WORK_GRAPH_SUBOBJECT();
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
        static NCPP_FORCE_INLINE void include_all_available_nodes(void* payload_p) noexcept
        {
            ((CD3DX12_WORK_GRAPH_SUBOBJECT*)payload_p)->IncludeAllAvailableNodes();
        }
        static NCPP_FORCE_INLINE void set_name(void* payload_p, const G_string& value) noexcept
        {
		    auto wvalue = G_to_wstring(value);
            ((CD3DX12_WORK_GRAPH_SUBOBJECT*)payload_p)->SetProgramName(wvalue.c_str());
        }
    };
}
