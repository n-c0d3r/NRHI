
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing functions")

include(NRHI/Drivers/DirectX12/ImplementDevice)
include(NRHI/Drivers/DirectX12/ImplementSystem)
include(NRHI/Drivers/DXGI/AdapterHelper)



NRHI_DXGI_ImplementAdapter(
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement functions done")