#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/utilities/abstract_data.hpp>
#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
#include <nrhi/work_graph_properties.hpp>
#endif



#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
namespace nrhi
{
#ifdef NRHI_DRIVER_DIRECTX_12
#define NRHI_DRIVER_DIRECTX_12_WORK_GRAPH_PROPERTIES_TYPE ID3D12WorkGraphProperties*
#else
#define NRHI_DRIVER_DIRECTX_12_WORK_GRAPH_PROPERTIES_TYPE u8
#endif



    namespace internal
    {
        NRHI_DEFINE_ABSTRACT_DATA(
            A_work_graph_properties,
            H_work_graph_properties,
            NRHI_DRIVER_DIRECTX_12_WORK_GRAPH_PROPERTIES_TYPE
        );
    }



    struct NRHI_API F_work_graph_properties : public internal::A_work_graph_properties
    {
    public:
        NCPP_FORCE_INLINE F_work_graph_properties() noexcept :
            internal::A_work_graph_properties()
        {}
        NCPP_FORCE_INLINE ~F_work_graph_properties() noexcept
        {}
        NCPP_FORCE_INLINE F_work_graph_properties(const F_work_graph_properties& x) noexcept :
            internal::A_work_graph_properties(x)
        {}
        NCPP_FORCE_INLINE F_work_graph_properties& operator = (const F_work_graph_properties& x) noexcept
        {
            ((internal::A_work_graph_properties&)*this) = x;
            return *this;
        }
        NCPP_FORCE_INLINE F_work_graph_properties(F_work_graph_properties&& x) noexcept :
            internal::A_work_graph_properties(eastl::move(x))
        {}
        NCPP_FORCE_INLINE F_work_graph_properties& operator = (F_work_graph_properties&& x) noexcept
        {
            ((internal::A_work_graph_properties&)*this) = eastl::move(x);
            return *this;
        }

    public:
        NCPP_FORCE_INLINE F_work_graph_properties(TKPA_valid<A_state_object> state_object_p)
        {
            H_work_graph_properties::construct_with_state_object(
                (void*)this,
                state_object_p
            );
        }

    public:
        b8 is_valid() const noexcept;
        b8 is_null() const noexcept;
        operator b8() const noexcept;
    };

    NCPP_FORCE_INLINE b8 operator == (const F_work_graph_properties& a, const F_work_graph_properties& b) noexcept
    {
        return H_work_graph_properties::compare(
            (void*)&a,
            (void*)&b
        );
    }

    NCPP_FORCE_INLINE b8 F_work_graph_properties::is_valid() const noexcept
    {
        return (*this == F_work_graph_properties {});
    }
    NCPP_FORCE_INLINE b8 F_work_graph_properties::is_null() const noexcept
    {
        return (*this != F_work_graph_properties {});
    }
    NCPP_FORCE_INLINE F_work_graph_properties::operator b8() const noexcept
    {
        return is_valid();
    }
}
#endif