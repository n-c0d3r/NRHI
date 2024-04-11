#include <nrhi/directx11/system.hpp>
#include <nrhi/dxgi/factory.hpp>



namespace nrhi {

    void HD_directx11_system::initialize(){

        F_dxgi_factory_helper::initialize();

    }
    void HD_directx11_system::release(){

        F_dxgi_factory_helper::release();

    }

}