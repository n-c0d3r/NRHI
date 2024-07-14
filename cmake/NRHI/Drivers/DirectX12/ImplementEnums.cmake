
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing enums")

include(NRHI/Drivers/DXGI/FormatHelper)
include(NRHI/Drivers/DirectX12/ImplementRootParamType)
include(NRHI/Drivers/DirectX12/ImplementDescriptorHeapType)
include(NRHI/Drivers/DirectX12/ImplementDescriptorRangeType)
include(NRHI/Drivers/DirectX12/ImplementFormatHelper)
include(NRHI/Drivers/DirectX12/ImplementCommandListType)
include(NRHI/Drivers/DirectX12/ImplementFenceFlag)
include(NRHI/Drivers/DirectX12/ImplementResourceBindFlag)
include(NRHI/Drivers/DirectX12/ImplementResourceType)
include(NRHI/Drivers/DirectX12/ImplementResourceUsage)



NRHI_DXGI_ImplementFormats(
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement enums done")