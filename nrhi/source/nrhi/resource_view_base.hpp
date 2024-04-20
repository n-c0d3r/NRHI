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
#include <nrhi/resource_view_desc.hpp>
#include <nrhi/resource_view_type.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;
    class A_resource_view;



    NCPP_FHANDLE_TEMPLATE(A_resource_view)
    struct TF_srv_handle {

        NCPP_FHANDLE_GENERATED_BODY(TF_srv_handle, A_resource_view);

    };

    using U_srv_handle = TF_srv_handle<TU<A_resource_view>>;
    using S_srv_handle = TF_srv_handle<TS<A_resource_view>>;
    using K_srv_handle = TF_srv_handle<TK<A_resource_view>>;

    using S_valid_srv_handle = TF_srv_handle<TS_valid<A_resource_view>>;
    using K_valid_srv_handle = TF_srv_handle<TK_valid<A_resource_view>>;



    NCPP_FHANDLE_TEMPLATE(A_resource_view)
    struct TF_uav_handle {

        NCPP_FHANDLE_GENERATED_BODY(TF_uav_handle, A_resource_view);

    };

    using U_uav_handle = TF_uav_handle<TU<A_resource_view>>;
    using S_uav_handle = TF_uav_handle<TS<A_resource_view>>;
    using K_uav_handle = TF_uav_handle<TK<A_resource_view>>;

    using S_valid_uav_handle = TF_uav_handle<TS_valid<A_resource_view>>;
    using K_valid_uav_handle = TF_uav_handle<TK_valid<A_resource_view>>;



    NCPP_FHANDLE_TEMPLATE(A_resource_view)
    struct TF_rtv_handle {

        NCPP_FHANDLE_GENERATED_BODY(TF_rtv_handle, A_resource_view);

    };

    using U_rtv_handle = TF_rtv_handle<TU<A_resource_view>>;
    using S_rtv_handle = TF_rtv_handle<TS<A_resource_view>>;
    using K_rtv_handle = TF_rtv_handle<TK<A_resource_view>>;

    using S_valid_rtv_handle = TF_rtv_handle<TS_valid<A_resource_view>>;
    using K_valid_rtv_handle = TF_rtv_handle<TK_valid<A_resource_view>>;



    NCPP_FHANDLE_TEMPLATE(A_resource_view)
    struct TF_dsv_handle {

        NCPP_FHANDLE_GENERATED_BODY(TF_dsv_handle, A_resource_view);

    };

    using U_dsv_handle = TF_dsv_handle<TU<A_resource_view>>;
    using S_dsv_handle = TF_dsv_handle<TS<A_resource_view>>;
    using K_dsv_handle = TF_dsv_handle<TK<A_resource_view>>;

    using S_valid_dsv_handle = TF_dsv_handle<TS_valid<A_resource_view>>;
    using K_valid_dsv_handle = TF_dsv_handle<TK_valid<A_resource_view>>;



    NCPP_FHANDLE_TEMPLATE(A_resource_view)
    struct TF_sampler_handle {

        NCPP_FHANDLE_GENERATED_BODY(TF_sampler_handle, A_resource_view);

    };

    using U_sampler_handle = TF_sampler_handle<TU<A_resource_view>>;
    using S_sampler_handle = TF_sampler_handle<TS<A_resource_view>>;
    using K_sampler_handle = TF_sampler_handle<TK<A_resource_view>>;

    using S_valid_sampler_handle = TF_sampler_handle<TS_valid<A_resource_view>>;
    using K_valid_sampler_handle = TF_sampler_handle<TK_valid<A_resource_view>>;



    class NRHI_API A_resource_view {

    private:
        TK_valid<A_device> device_p_;
        F_resource_view_desc desc_;

    public:
        NCPP_FORCE_INLINE TK_valid<A_device> device_p() noexcept { return device_p_; }
        NCPP_FORCE_INLINE F_resource_view_desc desc() noexcept { return desc_; }



    protected:
        A_resource_view(
            TK_valid<A_device> device_p,
            const F_resource_view_desc& desc,
            E_resource_view_type overrided_type
        );

    public:
        virtual ~A_resource_view();

    };

}
