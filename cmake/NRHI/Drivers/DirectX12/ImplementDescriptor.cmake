
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing descriptor")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_DESCRIPTOR_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/descriptor.hpp")
NCPP_SetGlobal(NRHI_DESCRIPTOR_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/descriptor.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_descriptor"
    DRIVER_SPECIFIC_NAME "HD_directx12_descriptor"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_DESCRIPTOR_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_DESCRIPTOR_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/descriptor.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_device;
        }"
    VALUES
        "initialize_resource_view" "void(
            TKPA_valid<A_descriptor_heap> heap_p,
            F_descriptor_cpu_address cpu_address,
            const F_resource_view_desc& desc
        )"
        "initialize_srv" "void(
            TKPA_valid<A_descriptor_heap> heap_p,
            F_descriptor_cpu_address cpu_address,
            const F_resource_view_desc& desc
        )"
        "initialize_uav" "void(
            TKPA_valid<A_descriptor_heap> heap_p,
            F_descriptor_cpu_address cpu_address,
            const F_resource_view_desc& desc
        )"
        "initialize_rtv" "void(
            TKPA_valid<A_descriptor_heap> heap_p,
            F_descriptor_cpu_address cpu_address,
            const F_resource_view_desc& desc
        )"
        "initialize_dsv" "void(
            TKPA_valid<A_descriptor_heap> heap_p,
            F_descriptor_cpu_address cpu_address,
            const F_resource_view_desc& desc
        )"
        "initialize_sampler_state" "void(
            TKPA_valid<A_descriptor_heap> heap_p,
            F_descriptor_cpu_address cpu_address,
            const F_resource_view_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement descriptor done")