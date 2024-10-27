#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/state_object_type.hpp>
#include <nrhi/work_graph_subobject.abstract_data.hpp>
#include <nrhi/library_subobject.abstract_data.hpp>



namespace nrhi
{
    class NRHI_API HD_directx12_state_object_builder
    {
    public:
        static NCPP_FORCE_INLINE void construct_default(void* payload_p) noexcept
        {
            new(payload_p) CD3DX12_STATE_OBJECT_DESC();
        }
        static NCPP_FORCE_INLINE void destruct(void* payload_p) noexcept
        {
            ((CD3DX12_STATE_OBJECT_DESC*)payload_p)->~CD3DX12_STATE_OBJECT_DESC();
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
            new(payload_p) CD3DX12_STATE_OBJECT_DESC(
                eastl::move(
                    *((CD3DX12_STATE_OBJECT_DESC*)x_payload_p)
                )
            );
        }
        static NCPP_FORCE_INLINE void operator_move(void* payload_p, void* x_payload_p) noexcept
        {
            *((CD3DX12_STATE_OBJECT_DESC*)payload_p) = eastl::move(*((CD3DX12_STATE_OBJECT_DESC*)x_payload_p));
        }

    public:
        static NCPP_FORCE_INLINE void construct_with_type(
            void* payload_p,
            ED_state_object_type type
        ) noexcept
        {
            new(payload_p) CD3DX12_STATE_OBJECT_DESC(D3D12_STATE_OBJECT_TYPE(type));
        }

    public:
        static NCPP_FORCE_INLINE F_work_graph_subobject& add_work_graph(
            void* payload_p
        ) noexcept
        {
            return *(
                (F_work_graph_subobject*)(
                    ((CD3DX12_STATE_OBJECT_DESC*)payload_p)->CreateSubobject<
                        CD3DX12_WORK_GRAPH_SUBOBJECT
                    >()
                )
            );
        }
        static NCPP_FORCE_INLINE F_library_subobject& add_library(
            void* payload_p
        ) noexcept
        {
            return *(
                (F_library_subobject*)(
                    ((CD3DX12_STATE_OBJECT_DESC*)payload_p)->CreateSubobject<
                        CD3DX12_DXIL_LIBRARY_SUBOBJECT
                    >()
                )
            );
        }
    };
}