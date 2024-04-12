#pragma once

/** @file nrhi/directx11/command_queue.hpp
*
*   Implement directx11 command_queue.
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

#include <nrhi/command_queue_base.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_command_list;



    class NRHI_API F_directx11_command_queue : public A_command_queue {

    private:
        ID3D11DeviceContext* d3d11_device_context_p_ = 0;

    public:
        NCPP_FORCE_INLINE ID3D11DeviceContext* d3d11_device_context_p() noexcept { return d3d11_device_context_p_; }



    public:
        F_directx11_command_queue(TK_valid<A_device> device_p, const F_command_queue_desc& desc);
        ~F_directx11_command_queue();

    };



    class NRHI_API HD_directx11_command_queue {

    public:
        static TU<A_command_queue> create(TK_valid<A_device> device_p, const F_command_queue_desc& desc);

        static void execute_command_lists(
            TK_valid<A_command_queue> command_queue_p,
            TG_span<TK_valid<A_command_list>> command_list_p_span
        );

    };

}
