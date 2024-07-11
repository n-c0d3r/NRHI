#include <nrhi/directx11/system.hpp>
#include <nrhi/directx11/factory.hpp>



namespace nrhi {

    void HD_directx11_system::initialize(){

        F_directx11_factory_helper::initialize();
    }
    void HD_directx11_system::release(){

        F_directx11_factory_helper::release();
    }

}