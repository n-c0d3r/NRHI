#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/utilities/abstract_data.hpp>
#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
#include <nrhi/state_object_properties.hpp>
#endif



#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
namespace nrhi
{
#ifdef NRHI_DRIVER_DIRECTX_12
#define NRHI_DRIVER_DIRECTX_12_STATE_OBJECT_PROPERTIES_TYPE ID3D12StateObjectProperties*
#else
#define NRHI_DRIVER_DIRECTX_12_STATE_OBJECT_PROPERTIES_TYPE u8
#endif



    namespace internal
    {
        NRHI_DEFINE_ABSTRACT_DATA(
            A_state_object_properties,
            H_state_object_properties,
            NRHI_DRIVER_DIRECTX_12_STATE_OBJECT_PROPERTIES_TYPE
        );
    }



    struct NRHI_API F_state_object_properties : public internal::A_state_object_properties
    {
    public:
        NCPP_FORCE_INLINE F_state_object_properties() noexcept :
            internal::A_state_object_properties()
        {}
        NCPP_FORCE_INLINE ~F_state_object_properties() noexcept
        {}
        NCPP_FORCE_INLINE F_state_object_properties(const F_state_object_properties& x) noexcept :
            internal::A_state_object_properties(x)
        {}
        NCPP_FORCE_INLINE F_state_object_properties& operator = (const F_state_object_properties& x) noexcept
        {
            ((internal::A_state_object_properties&)*this) = x;
            return *this;
        }
        NCPP_FORCE_INLINE F_state_object_properties(F_state_object_properties&& x) noexcept :
            internal::A_state_object_properties(eastl::move(x))
        {}
        NCPP_FORCE_INLINE F_state_object_properties& operator = (F_state_object_properties&& x) noexcept
        {
            ((internal::A_state_object_properties&)*this) = eastl::move(x);
            return *this;
        }

    public:
        NCPP_FORCE_INLINE F_state_object_properties(TKPA_valid<A_state_object> state_object_p)
        {
            H_state_object_properties::construct_with_state_object(
                (void*)this,
                state_object_p
            );
        }

    public:
        b8 is_valid() const noexcept;
        b8 is_null() const noexcept;
        operator b8() const noexcept;

    public:
        NCPP_FORCE_INLINE F_program_id program_id(const G_string& name) noexcept
        {
            return H_state_object_properties::program_id(
                (void*)this,
                name
            );
        }
    };

    NCPP_FORCE_INLINE b8 operator == (const F_state_object_properties& a, const F_state_object_properties& b) noexcept
    {
        return H_state_object_properties::compare(
            (void*)&a,
            (void*)&b
        );
    }

    NCPP_FORCE_INLINE b8 F_state_object_properties::is_valid() const noexcept
    {
        return (*this == F_state_object_properties {});
    }
    NCPP_FORCE_INLINE b8 F_state_object_properties::is_null() const noexcept
    {
        return (*this != F_state_object_properties {});
    }
    NCPP_FORCE_INLINE F_state_object_properties::operator b8() const noexcept
    {
        return is_valid();
    }
}
#endif