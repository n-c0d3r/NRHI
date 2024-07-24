
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing resource")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_RESOURCE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_resource"
    DRIVER_SPECIFIC_NAME "HD_directx12_resource"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/resource.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
        "create_committed" "TU<A_resource>(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_committed_buffer" "U_buffer_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_committed_indirect_buffer" "U_indirect_buffer_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_committed_structured_buffer" "U_structured_buffer_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_committed_texture_1d" "U_texture_1d_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_committed_texture_2d" "U_texture_2d_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_committed_texture_3d" "U_texture_3d_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_committed_texture_2d_array" "U_texture_2d_array_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_committed_texture_cube" "U_texture_cube_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement resource done")