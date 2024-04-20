#pragma once

/** @file nrhi/swapchain_base.hpp
*
*   Implement swapchain base class.
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

#include <nrhi/format.hpp>
#include <nrhi/sample_desc.hpp>
#include <nrhi/resource_base.hpp>
#include <nrhi/resource_view_base.hpp>
#include <nrhi/texture.hpp>

#pragma endregion



namespace nrhi {

    class A_command_queue;



    struct F_swapchain_desc {
        E_format format = E_format::R8G8B8A8_UNORM;
        F_sample_desc sample_desc;
    };



    class NRHI_API A_swapchain {

    private:
        TK_valid<A_command_queue> command_queue_p_;
        TK_valid<A_surface> surface_p_;
        F_swapchain_desc desc_;

    protected:
        U_texture_2d_handle back_texture_2d_p_;
        U_rtv_handle back_rtv_p_;

    public:
        NCPP_FORCE_INLINE TK_valid<A_command_queue> command_queue_p() noexcept { return command_queue_p_; }
        NCPP_FORCE_INLINE TK_valid<A_surface> surface_p() noexcept { return surface_p_; }
        NCPP_FORCE_INLINE const F_swapchain_desc& desc() const noexcept { return desc_; }
        NCPP_FORCE_INLINE K_valid_rtv_handle back_rtv_p() noexcept {
            return NCPP_FHANDLE_VALID(back_rtv_p_);
        }



    protected:
        A_swapchain(
            TK_valid<A_command_queue> command_queue_p,
            TK_valid<A_surface> surface_p,
            const F_swapchain_desc& desc
        );

    public:
        virtual ~A_swapchain();

    };

}
