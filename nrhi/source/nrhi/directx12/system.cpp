#include <nrhi/directx12/system.hpp>
#include <nrhi/directx12/factory.hpp>
#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
#include <nrhi/directx12/pix_runtime.hpp>
#endif



#ifdef NRHI_ENABLE_DIRECTX_12_AGILITY_SDK
	extern "C"
	{
		__declspec(dllexport) extern const UINT D3D12SDKVersion = NRHI_DIRECTX12_AGILITY_SDK_VERSION_MAJOR;
	}
	extern "C"
	{
		__declspec(dllexport) extern const char* D3D12SDKPath = NRHI_DIRECTX12_AGILITY_SDK_DIR;
	}
#endif



namespace nrhi
{
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

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
		HD_directx12_pix_runtime::initialize();
#endif

        F_directx12_factory_helper::initialize();
    }
    void HD_directx12_system::release(){

    	F_directx12_factory_helper::release();

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
		HD_directx12_pix_runtime::release();
#endif
    }
}
