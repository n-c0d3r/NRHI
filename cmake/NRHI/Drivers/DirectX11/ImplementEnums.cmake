
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing enums")

include(NRHI/Drivers/DXGI/FormatHelper)
include(NRHI/Drivers/DirectX11/ImplementResourceFlag)
include(NRHI/Drivers/DirectX11/ImplementCommandListType)
include(NRHI/Drivers/DirectX11/ImplementResourceHeapType)
include(NRHI/Drivers/DirectX11/ImplementDescriptorHeapType)
include(NRHI/Drivers/DirectX11/ImplementDescriptorRangeType)
include(NRHI/Drivers/DirectX11/ImplementRootParamType)
include(NRHI/Drivers/DirectX11/ImplementShaderVisibility)
include(NRHI/Drivers/DirectX11/ImplementResourceBindFlag)
include(NRHI/Drivers/DirectX11/ImplementResourceMiscFlag)



NRHI_DXGI_ImplementFormats(
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement enums done")