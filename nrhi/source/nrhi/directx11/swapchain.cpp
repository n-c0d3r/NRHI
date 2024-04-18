#include <nrhi/directx11/swapchain.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/command_queue.hpp>
#include <nrhi/dxgi/factory.hpp>



namespace nrhi {

    F_directx11_swapchain::F_directx11_swapchain(
        TK_valid<A_command_queue> command_queue_p,
        TK_valid<A_surface> surface_p,
        const F_swapchain_desc& desc
    ) :
        A_swapchain(
            command_queue_p,
            surface_p,
            desc
        )
    {

        auto surface_desc = surface_p->desc();

        DXGI_SWAP_CHAIN_DESC dxgi_swapchain_desc;
        ZeroMemory(&dxgi_swapchain_desc, sizeof(dxgi_swapchain_desc));
        dxgi_swapchain_desc.BufferCount = 1;
        dxgi_swapchain_desc.BufferDesc.Width = surface_desc.size.x;
        dxgi_swapchain_desc.BufferDesc.Height = surface_desc.size.y;
        dxgi_swapchain_desc.BufferDesc.Format = (DXGI_FORMAT)(desc.format);
        dxgi_swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
        dxgi_swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
        dxgi_swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxgi_swapchain_desc.OutputWindow = surface_p.T_cast<F_windows_surface>()->handle();
        dxgi_swapchain_desc.SampleDesc.Count = 1;
        dxgi_swapchain_desc.SampleDesc.Quality = 0;
        dxgi_swapchain_desc.Windowed = TRUE;

        F_dxgi_factory_helper::factory_p()->CreateSwapChain(
            command_queue_p->device_p().T_cast<F_directx11_device>()->d3d11_device_p(),
            &dxgi_swapchain_desc,
            &dxgi_swapchain_p_
        );

    }
    F_directx11_swapchain::~F_directx11_swapchain(){

        if(dxgi_swapchain_p_)
            dxgi_swapchain_p_->Release();
    }



    TU<A_swapchain> HD_directx11_swapchain::create(
        TK_valid<A_command_queue> command_queue_p,
        TK_valid<A_surface> surface_p,
        const F_swapchain_desc& desc
    ){

        return TU<F_directx11_swapchain>()(
            command_queue_p,
            surface_p,
            desc
        );
    }

}