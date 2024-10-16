
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing device")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_DEVICE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/device.hpp")
NCPP_SetGlobal(NRHI_DEVICE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/device.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_device"
    DRIVER_SPECIFIC_NAME "HD_directx12_device"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_DEVICE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_DEVICE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/device.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_adapter;
        }"
    VALUES
        "create" "TU<A_device>(TKPA_valid<A_adapter> adapter_p)"
        "descriptor_increment_size" "u64(
			TKPA_valid<A_device> device_p,
			ED_descriptor_heap_type descriptor_heap_type
		)"
        "resource_heap_tier" "E_resource_heap_tier(
			TKPA_valid<A_device> device_p
		)"
        "hlsl_highest_shader_model" "eastl::pair<u32, u32>(
    		TKPA_valid<A_device> device_p
    	)"
        "min_wave_size" "u32(
    		TKPA_valid<A_device> device_p
    	)"
        "max_wave_size" "u32(
    		TKPA_valid<A_device> device_p
    	)"
        "total_lane_count" "u32(
    		TKPA_valid<A_device> device_p
    	)"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement device done")