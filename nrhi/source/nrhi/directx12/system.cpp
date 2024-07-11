#include <nrhi/directx12/system.hpp>
#include <nrhi/directx12/factory.hpp>



namespace nrhi {

    void HD_directx12_system::initialize(){

        F_directx12_factory_helper::initialize();
    }
    void HD_directx12_system::release(){

		F_directx12_factory_helper::release();
    }

}