
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing resource bind flag")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_FLAG_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_flag.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_FLAG_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_flag.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_resource_flag"
    TYPE "ncpp::u32"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_FLAG_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_FLAG_ENUM_CPP_FILE_PATH}"
    VALUES
        NONE "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0, 0)"
        INPUT_BUFFER "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x1, 0)"
        INDEX_BUFFER "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x2, 0)"
        CONSTANT_BUFFER "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x4, 0)"
        SHADER_RESOURCE "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x8, 0)"
        UNORDERED_ACCESS "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x10, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)"
        RENDER_TARGET "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x20, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)"
        DEPTH_STENCIL "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x40, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)"

        CROSS_ADAPTER "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x80, D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER)"
        SIMULTANEOUS "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x100, D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS)"
        VIDEO_DECODE_REFERENCE_ONLY "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x200, D3D12_RESOURCE_FLAG_VIDEO_DECODE_REFERENCE_ONLY)"
        VIDEO_ENCODE_REFERENCE_ONLY "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x400, D3D12_RESOURCE_FLAG_VIDEO_ENCODE_REFERENCE_ONLY)"
        RAYTRACING_ACCELERATION_STRUCTURE "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x800, D3D12_RESOURCE_FLAG_RAYTRACING_ACCELERATION_STRUCTURE)"

        MIPS_GENERATABLE "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x1000, 0)"
        STRUCTURED "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x2000, 0)"
        INDIRECT "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x4000, 0)"
        TEXTURE_CUBE "NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_FLAG(0x8000, 0)"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement resource bind flag done")