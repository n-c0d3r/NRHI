#pragma once

/** @file nrhi/directx11/resource.hpp
*
*   Implement directx11 resource.
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

#include <nrhi/resource_base.hpp>
#include <nrhi/buffer.hpp>

#pragma endregion



namespace nrhi {

    class A_device;



    class NRHI_API F_directx11_resource : public A_resource {

    private:
        ID3D11Resource* d3d11_resource_p_ = 0;

    public:
        NCPP_FORCE_INLINE ID3D11Resource* d3d11_resource_p() noexcept { return d3d11_resource_p_; }

    public:
        F_directx11_resource(
            TK_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            E_resource_type overrided_type
        );
        F_directx11_resource(
            TK_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            E_resource_type overrided_type,
            ID3D11Resource* d3d11_resource_p
        );
        virtual ~F_directx11_resource();

    };



    class NRHI_API HD_directx11_resource {

    public:
        static TU<A_resource> create(
            TK_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        );

    public:
        static U_buffer_handle create_buffer(
            TK_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        );
        static U_buffer_handle create_structured_buffer(
            TK_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        );
        static U_buffer_handle create_single_elemented_buffer(
            TK_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        );

    };

}
