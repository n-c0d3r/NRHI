
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing enums")

include(NRHI/Drivers/DXGI/FormatHelper)
include(NRHI/Drivers/DirectX12/ImplementRootParamType)
include(NRHI/Drivers/DirectX12/ImplementDescriptorHeapType)
include(NRHI/Drivers/DirectX12/ImplementDescriptorHeapFlag)
include(NRHI/Drivers/DirectX12/ImplementDescriptorRangeType)
include(NRHI/Drivers/DirectX12/ImplementFormatHelper)
include(NRHI/Drivers/DirectX12/ImplementCommandListType)
include(NRHI/Drivers/DirectX12/ImplementFenceFlag)
include(NRHI/Drivers/DirectX12/ImplementResourceBindFlag)
include(NRHI/Drivers/DirectX12/ImplementResourceType)
include(NRHI/Drivers/DirectX12/ImplementResourceState)
include(NRHI/Drivers/DirectX12/ImplementResourceHeapType)
include(NRHI/Drivers/DirectX12/ImplementResourceLayout)
include(NRHI/Drivers/DirectX12/ImplementResourceViewType)
include(NRHI/Drivers/DirectX12/ImplementFilter)
include(NRHI/Drivers/DirectX12/ImplementTexCoordAddressMode)
include(NRHI/Drivers/DirectX12/ImplementPipelineStateType)
include(NRHI/Drivers/DirectX12/ImplementPrimitiveTopology)
include(NRHI/Drivers/DirectX12/ImplementShaderType)
include(NRHI/Drivers/DirectX12/ImplementShaderVisibility)



NRHI_DXGI_ImplementFormats(
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement enums done")