#include <nrhi/directx11/system.hpp>
#include <nrhi/directx11/graphics_pipeline_state.hpp>
#include <nrhi/dxgi/factory.hpp>



namespace nrhi {

    void HD_directx11_system::initialize(){

        F_dxgi_factory_helper::initialize();

		F_directx11_graphics_pipeline_state::initialize_pools();

    }
    void HD_directx11_system::release(){

		F_directx11_graphics_pipeline_state::release_pools();

        F_dxgi_factory_helper::release();

    }

}