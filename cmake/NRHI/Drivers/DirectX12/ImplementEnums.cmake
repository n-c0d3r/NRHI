
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing enums")

include(NRHI/Drivers/DXGI/FormatHelper)



NRHI_DXGI_ImplementFormats(
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    DRIVER_LOWER_CASE_NAME "directx_12"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement enums done")