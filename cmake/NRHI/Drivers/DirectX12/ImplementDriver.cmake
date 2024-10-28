
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing driver")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_DRIVER_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/driver.hpp")
NCPP_SetGlobal(NRHI_DRIVER_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/driver.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_driver"
    DRIVER_SPECIFIC_NAME "HD_directx12_driver"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_DRIVER_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_DRIVER_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/driver.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
        "interface_support_simple_resource_binding" "bool()"
        "interface_support_simple_indirect_commands" "bool()"
        "interface_support_simple_resource_management" "bool()"
        "interface_support_simple_work_submission" "bool()"
        "interface_support_advanced_resource_binding" "bool()"
        "interface_support_advanced_indirect_commands" "bool()"
        "interface_support_advanced_resource_management" "bool()"
        "interface_support_advanced_work_submission" "bool()"
        "full_support_simple_resource_binding" "bool()"
        "full_support_simple_indirect_commands" "bool()"
        "full_support_simple_resource_management" "bool()"
        "full_support_simple_work_submission" "bool()"
        "full_support_advanced_resource_binding" "bool()"
        "full_support_advanced_indirect_commands" "bool()"
        "full_support_advanced_resource_management" "bool()"
        "full_support_advanced_work_submission" "bool()"
        "support_simple_resource_binding" "bool()"
        "support_simple_indirect_commands" "bool()"
        "support_simple_resource_management" "bool()"
        "support_simple_work_submission" "bool()"
        "support_advanced_resource_binding" "bool()"
        "support_advanced_indirect_commands" "bool()"
        "support_advanced_resource_management" "bool()"
        "support_advanced_work_submission" "bool()"
        "support_mesh_shader" "bool()"
        "support_amplification_shader" "bool()"
        "support_state_object" "bool()"
        "support_work_graphs" "bool()"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement driver done")