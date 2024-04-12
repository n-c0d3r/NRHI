
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing functions")

include(NRHI/Drivers/DirectX11/ImplementDevice)
include(NRHI/Drivers/DirectX11/ImplementSystem)
include(NRHI/Drivers/DirectX11/ImplementCommandQueue)
include(NRHI/Drivers/DirectX11/ImplementCommandList)
include(NRHI/Drivers/DirectX11/ImplementSwapChain)
include(NRHI/Drivers/DXGI/AdapterHelper)



NRHI_DXGI_ImplementAdapter(
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement functions done")