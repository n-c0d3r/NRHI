
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing resource bind flag")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_FLAG_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_flag.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_FLAG_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_flag.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_resource_flag"
    TYPE "ncpp::u64"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_FLAG_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_FLAG_ENUM_CPP_FILE_PATH}"
    FLAG_OPERATORS
    VALUES
        NONE 0
        INPUT_BUFFER "NRHI_DRIVER_DIRECTX_11_GENERATE___RESOURCE_FLAG(D3D11_BIND_VERTEX_BUFFER, 0)"
        INDEX_BUFFER "NRHI_DRIVER_DIRECTX_11_GENERATE___RESOURCE_FLAG(D3D11_BIND_INDEX_BUFFER, 0)"
        CONSTANT_BUFFER "NRHI_DRIVER_DIRECTX_11_GENERATE___RESOURCE_FLAG(D3D11_BIND_CONSTANT_BUFFER, 0)"
        SHADER_RESOURCE "NRHI_DRIVER_DIRECTX_11_GENERATE___RESOURCE_FLAG(D3D11_BIND_SHADER_RESOURCE, 0)"
        UNORDERED_ACCESS "NRHI_DRIVER_DIRECTX_11_GENERATE___RESOURCE_FLAG(D3D11_BIND_UNORDERED_ACCESS, 0)"
        RENDER_TARGET "NRHI_DRIVER_DIRECTX_11_GENERATE___RESOURCE_FLAG(D3D11_BIND_RENDER_TARGET, 0)"
        DEPTH_STENCIL "NRHI_DRIVER_DIRECTX_11_GENERATE___RESOURCE_FLAG(D3D11_BIND_DEPTH_STENCIL, 0)"

        MIPS_GENERATABLE "NRHI_DRIVER_DIRECTX_11_GENERATE___RESOURCE_FLAG(0, D3D11_RESOURCE_MISC_GENERATE_MIPS)"
        STRUCTURED "NRHI_DRIVER_DIRECTX_11_GENERATE___RESOURCE_FLAG(0, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)"
        INDIRECT_ARGUMENT_BUFFER"NRHI_DRIVER_DIRECTX_11_GENERATE___RESOURCE_FLAG(0, D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS)"
        TEXTURE_CUBE "NRHI_DRIVER_DIRECTX_11_GENERATE___RESOURCE_FLAG(0, D3D11_RESOURCE_MISC_TEXTURECUBE)"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement resource bind flag done")