
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing resource flag")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_VIEW_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/view_type.hpp")
NCPP_SetGlobal(NRHI_VIEW_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/view_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "E_view_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    DRIVER_LOWER_CASE_NAME "directx_11"
    TARGET_HPP_FILE_PATH "${NRHI_VIEW_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_VIEW_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        NONE 0
        RENDER_TARGET 1
        DEPTH_STENCIL 2
        SHADER_RESOURCE 3
        UNORDERED_ACCESS 4
        CONSTANT_BUFFER 5
        SAMPLERS 6
        VERTEX_BUFFER 7
        INDEX_BUFFER 8
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement resource flag done")