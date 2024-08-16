
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing enums")

include(NRHI/Drivers/DXGI/FormatHelper)
include(NRHI/Drivers/DirectX11/ImplementCommandListType)
include(NRHI/Drivers/DirectX11/ImplementResourceFlag)
include(NRHI/Drivers/DirectX11/ImplementResourceType)
include(NRHI/Drivers/DirectX11/ImplementResourceHeapType)
include(NRHI/Drivers/DirectX11/ImplementResourceViewType)
include(NRHI/Drivers/DirectX11/ImplementPipelineStateType)
include(NRHI/Drivers/DirectX11/ImplementShaderType)
include(NRHI/Drivers/DirectX11/ImplementCullMode)
include(NRHI/Drivers/DirectX11/ImplementFillMode)
include(NRHI/Drivers/DirectX11/ImplementDepthComparisonFunc)
include(NRHI/Drivers/DirectX11/ImplementPrimitiveTopology)
include(NRHI/Drivers/DirectX11/ImplementClearFlag)
include(NRHI/Drivers/DirectX11/ImplementTextureCubeFace)
include(NRHI/Drivers/DirectX11/ImplementFilter)
include(NRHI/Drivers/DirectX11/ImplementTexCoordAddressMode)
include(NRHI/Drivers/DirectX11/ImplementTextureCopyLocationType)
include(NRHI/Drivers/DirectX11/ImplementBlendFactor)
include(NRHI/Drivers/DirectX11/ImplementBlendOperation)
include(NRHI/Drivers/DirectX11/ImplementColorWriteMode)
include(NRHI/Drivers/DirectX11/ImplementInputClassification)



NRHI_DXGI_ImplementFormats(
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement enums done")