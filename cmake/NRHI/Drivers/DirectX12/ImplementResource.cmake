
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
        "create_placed" "TU<A_resource>(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc,
            TKPA_valid<A_resource_heap> heap_p,
            u64 heap_offset
        )"
        "create_placed_buffer" "U_buffer_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc,
            TKPA_valid<A_resource_heap> heap_p,
            u64 heap_offset
        )"
        "create_placed_texture_1d" "U_texture_1d_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc,
            TKPA_valid<A_resource_heap> heap_p,
            u64 heap_offset
        )"
        "create_placed_texture_2d" "U_texture_2d_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc,
            TKPA_valid<A_resource_heap> heap_p,
            u64 heap_offset
        )"
        "create_placed_texture_3d" "U_texture_3d_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc,
            TKPA_valid<A_resource_heap> heap_p,
            u64 heap_offset
        )"
        "create_placed_texture_2d_array" "U_texture_2d_array_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc,
            TKPA_valid<A_resource_heap> heap_p,
            u64 heap_offset
        )"
        "create_reserved" "TU<A_resource>(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_reserved_buffer" "U_buffer_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_reserved_texture_1d" "U_texture_1d_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_reserved_texture_2d" "U_texture_2d_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_reserved_texture_3d" "U_texture_3d_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "create_reserved_texture_2d_array" "U_texture_2d_array_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_desc& desc
        )"
        "map" "F_mapped_subresource(
            TKPA_valid<A_resource> resource_p,
            u32 subresource_index
        )"
        "unmap" "void(
            TKPA_valid<A_resource> resource_p,
            u32 subresource_index
        )"
        "gpu_virtual_address" "F_resource_gpu_virtual_address(
			TKPA_valid<A_resource> resource_p
		)"
		"first_pitch" "sz(u32 element_stride, u32 count)"
		"second_pitch" "sz(u32 first_pitch, u32 count)"
		"third_pitch" "sz(u32 second_pitch, u32 count)"
		"subresource_index" "u32(
			u32 array_slice,
			u32 mip_level,
			u32 mip_level_count
		)"
		"subresource_index_with_plane_slice" "u32(
			u32 array_slice,
			u32 mip_level,
			u32 mip_level_count,
			u32 array_size,
			u32 plane_slice
		)"
		"mip_divisor" "F_vector3_u32(
            ED_resource_type type
        )"

    ALTERNATIVE_VALUES
        "create" "TU<A_resource>(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc
        )"
        "create_buffer" "U_buffer_handle(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        )"
        "create_texture_1d" "U_texture_1d_handle(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        )"
        "create_texture_2d" "U_texture_2d_handle(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        )"
        "create_texture_3d" "U_texture_3d_handle(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        )"
        "create_texture_2d_array" "U_texture_2d_array_handle(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement resource done")