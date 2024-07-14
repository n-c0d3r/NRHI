
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing functions")

include(NRHI/Drivers/DirectX12/ImplementDevice)
include(NRHI/Drivers/DirectX12/ImplementSystem)
include(NRHI/Drivers/DirectX12/ImplementDriver)
include(NRHI/Drivers/DirectX12/ImplementAdapter)
include(NRHI/Drivers/DirectX12/ImplementCommandQueue)
include(NRHI/Drivers/DirectX12/ImplementCommandList)
include(NRHI/Drivers/DirectX12/ImplementCommandAllocator)
include(NRHI/Drivers/DirectX12/ImplementSwapChain)
include(NRHI/Drivers/DirectX12/ImplementFence)
include(NRHI/Drivers/DirectX12/ImplementResource)

message(STATUS "<NRHI::Drivers::DirectX12> Implement functions done")