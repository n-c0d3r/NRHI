
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing enums")

include(NRHI/Drivers/DXGI/FormatHelper)



NRHI_DXGI_ImplementFormats(
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    DRIVER_LOWER_CASE_NAME "directx_11"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement enums done")