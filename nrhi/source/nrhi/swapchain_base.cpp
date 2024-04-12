#include <nrhi/swapchain_base.hpp>



namespace nrhi {

    A_swapchain::A_swapchain(
        TK_valid<A_command_queue> command_queue_p,
        TK_valid<A_surface> surface_p,
        const F_swapchain_desc& desc
    ) :
        command_queue_p_(command_queue_p),
        surface_p_(surface_p),
        desc_(desc)
    {

    }
    A_swapchain::~A_swapchain(){

    }

}