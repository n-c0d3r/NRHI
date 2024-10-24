
message(STATUS "<NRHI::Drivers::DirectX12> Start checking DirectX 12 Agility SDK")

include(FetchContent)

set(NRHI_DIRECTX12_AGILITY_SDK_VERSION_MAJOR "614" CACHE INTERNAL "")
set(NRHI_DIRECTX12_AGILITY_SDK_VERSION_MINOR "1"   CACHE INTERNAL "")
set(NRHI_DIRECTX12_AGILITY_SDK_NUGET_URL "https://www.nuget.org/api/v2/package/Microsoft.Direct3D.D3D12/1.${NRHI_DIRECTX12_AGILITY_SDK_VERSION_MAJOR}.${NRHI_DIRECTX12_AGILITY_SDK_VERSION_MINOR}")

FetchContent_Declare(nrhi_directx12_agility_sdk URL ${NRHI_DIRECTX12_AGILITY_SDK_NUGET_URL})
FetchContent_MakeAvailable(nrhi_directx12_agility_sdk)

set(NRHI_DIRECTX12_AGILITY_SDK_D3D12_CORE_DLL      "${nrhi_directx12_agility_sdk_SOURCE_DIR}/build/native/bin/x64/D3D12Core.dll"      CACHE INTERNAL "")
set(NRHI_DIRECTX12_AGILITY_SDK_D3D12_SDK_LAYERS_DLL "${nrhi_directx12_agility_sdk_SOURCE_DIR}/build/native/bin/x64/d3d12SDKLayers.dll" CACHE INTERNAL "")

set(NRHI_DIRECTX12_AGILITY_SDK_DIR ${nrhi_directx12_agility_sdk_SOURCE_DIR} CACHE INTERNAL "")

message(STATUS "<NRHI::Drivers::DirectX12> Check DirectX 12 Agility SDK done")