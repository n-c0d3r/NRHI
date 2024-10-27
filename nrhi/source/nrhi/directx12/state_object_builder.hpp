#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/state_object_type.hpp>
#include <nrhi/state_object_config_subobject.abstract_data.hpp>
#include <nrhi/global_root_signature_subobject.abstract_data.hpp>
#include <nrhi/local_root_signature_subobject.abstract_data.hpp>
#include <nrhi/work_graph_subobject.abstract_data.hpp>
#include <nrhi/library_subobject.abstract_data.hpp>



namespace nrhi
{
    class A_device;
    class A_state_object;



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
        static NCPP_FORCE_INLINE F_state_object_config_subobject& add_state_object_config(
            void* payload_p
        ) noexcept
        {
            return *(
                (F_state_object_config_subobject*)(
                    ((CD3DX12_STATE_OBJECT_DESC*)payload_p)->CreateSubobject<
                        CD3DX12_STATE_OBJECT_CONFIG_SUBOBJECT
                    >()
                )
            );
        }
        static NCPP_FORCE_INLINE F_global_root_signature_subobject& add_global_root_signature_subobject(
            void* payload_p
        ) noexcept
        {
            return *(
                (F_global_root_signature_subobject*)(
                    ((CD3DX12_STATE_OBJECT_DESC*)payload_p)->CreateSubobject<
                        CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT
                    >()
                )
            );
        }
        static NCPP_FORCE_INLINE F_local_root_signature_subobject& add_local_root_signature_subobject(
            void* payload_p
        ) noexcept
        {
            return *(
                (F_local_root_signature_subobject*)(
                    ((CD3DX12_STATE_OBJECT_DESC*)payload_p)->CreateSubobject<
                        CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT
                    >()
                )
            );
        }
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

    public:
        static TU<A_state_object> build(
            void* payload_p,
            TKPA_valid<A_device> device_p
        );
        static TU<A_state_object> add_to(
            void* payload_p,
            TKPA_valid<A_state_object> src_state_object_p
        );
    };
}