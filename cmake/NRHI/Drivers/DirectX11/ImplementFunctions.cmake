
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing functions")

include(NRHI/Drivers/DirectX11/ImplementDriver)
include(NRHI/Drivers/DirectX11/ImplementDevice)
include(NRHI/Drivers/DirectX11/ImplementSystem)
include(NRHI/Drivers/DirectX11/ImplementCommandQueue)
include(NRHI/Drivers/DirectX11/ImplementCommandList)
include(NRHI/Drivers/DirectX11/ImplementSwapChain)
include(NRHI/Drivers/DirectX11/ImplementResourceView)
include(NRHI/Drivers/DirectX11/ImplementResource)
include(NRHI/Drivers/DirectX11/ImplementFormatHelper)
include(NRHI/Drivers/DirectX11/ImplementShaderCompiler)
include(NRHI/Drivers/DirectX11/ImplementShaderClass)
include(NRHI/Drivers/DirectX11/ImplementShader)
include(NRHI/Drivers/DirectX11/ImplementShaderBlob)
include(NRHI/Drivers/DirectX11/ImplementPipelineState)
include(NRHI/Drivers/DirectX11/ImplementFrameBuffer)
include(NRHI/Drivers/DXGI/AdapterHelper)



NRHI_DXGI_ImplementAdapter(
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement functions done")