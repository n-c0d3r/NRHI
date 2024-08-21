#include <nrhi/directx12/system.hpp>
#include <nrhi/directx12/factory.hpp>



namespace nrhi {

    void HD_directx12_system::initialize(){

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
			debugController->EnableDebugLayer();

			// Microsoft::WRL::ComPtr<ID3D12Debug1> debugController1;
			// if (SUCCEEDED(debugController.As(&debugController1))) {
			// 	debugController1->SetEnableGPUBasedValidation(TRUE);
			// }
		}
#endif

        F_directx12_factory_helper::initialize();
    }
    void HD_directx12_system::release(){

		F_directx12_factory_helper::release();
    }

}