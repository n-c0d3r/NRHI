
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing enums")

include(NRHI/Drivers/DXGI/FormatHelper)
include(NRHI/Drivers/DirectX11/ImplementCommandListType)
include(NRHI/Drivers/DirectX11/ImplementRootParamType)
include(NRHI/Drivers/DirectX11/ImplementResourceBindFlag)
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



NRHI_DXGI_ImplementFormats(
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement enums done")