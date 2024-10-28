#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/utilities/abstract_data.hpp>
#if defined(NRHI_DRIVER_SUPPORT_WORK_GRAPHS) && defined(NRHI_DRIVER_SUPPORT_STATE_OBJECT)
#include <nrhi/work_graph_subobject.hpp>
#endif



#if defined(NRHI_DRIVER_SUPPORT_WORK_GRAPHS) && defined(NRHI_DRIVER_SUPPORT_STATE_OBJECT)
namespace nrhi
{
#ifdef NRHI_DRIVER_DIRECTX_12
#define NRHI_DRIVER_DIRECTX_12_WORK_GRAPH_SUBOBJECT_TYPE CD3DX12_WORK_GRAPH_SUBOBJECT
#else
#define NRHI_DRIVER_DIRECTX_12_WORK_GRAPH_SUBOBJECT_TYPE u8
#endif



    namespace internal
    {
        NRHI_DEFINE_ABSTRACT_DATA(
            A_work_graph_subobject,
            H_work_graph_subobject,
            NRHI_DRIVER_DIRECTX_12_WORK_GRAPH_SUBOBJECT_TYPE
        );
    }



    struct NRHI_API F_work_graph_subobject : public internal::A_work_graph_subobject
    {
    public:
        NCPP_FORCE_INLINE F_work_graph_subobject() noexcept :
            internal::A_work_graph_subobject()
        {}
        NCPP_FORCE_INLINE ~F_work_graph_subobject() noexcept
        {}

    public:
        NCPP_FORCE_INLINE void include_all_available_nodes() noexcept
        {
            H_work_graph_subobject::include_all_available_nodes(
                this
            );
        }
        NCPP_FORCE_INLINE void set_name(const G_string& value) noexcept
        {
            H_work_graph_subobject::set_name(
                this,
                value
            );
        }
        NCPP_FORCE_INLINE void add_entrypoint(const F_node_id& node_id) noexcept
        {
            H_work_graph_subobject::add_entrypoint(
                (void*)this,
                node_id
            );
        }
    };
}
#endif