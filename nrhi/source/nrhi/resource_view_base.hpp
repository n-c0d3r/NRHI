#pragma once

/** @file nrhi/resource_view_base.hpp
*
*   Implement resource view base class.
*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#pragma region Includes

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/prerequisites.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/resource_desc.hpp>
#include <nrhi/resource_view_type.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;
    class A_resource_view;



    class NRHI_API A_resource_view {

    private:
        TK_valid<A_device> device_p_;
        TK_valid<A_resource> resource_p_;
        E_resource_view_type type_;

    public:
        NCPP_FORCE_INLINE TK_valid<A_device> device_p() noexcept { return device_p_; }
        NCPP_FORCE_INLINE TK_valid<A_resource> resource_p() noexcept { return resource_p_; }
        NCPP_FORCE_INLINE E_resource_view_type type() noexcept { return type_; }



    protected:
        A_resource_view(
            TK_valid<A_device> device_p,
            TK_valid<A_resource> resource_p,
            E_resource_view_type type
        );

    public:
        virtual ~A_resource_view();

    };

}
