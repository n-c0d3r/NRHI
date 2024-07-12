
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing enums")

include(NRHI/Drivers/DXGI/FormatHelper)
include(NRHI/Drivers/DirectX12/ImplementRootParamType)
include(NRHI/Drivers/DirectX12/ImplementDescriptorHeapType)
include(NRHI/Drivers/DirectX12/ImplementDescriptorRangeType)
include(NRHI/Drivers/DirectX12/ImplementFormatHelper)



NRHI_DXGI_ImplementFormats(
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement enums done")