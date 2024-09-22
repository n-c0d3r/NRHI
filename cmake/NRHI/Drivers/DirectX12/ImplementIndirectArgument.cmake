
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing indirect argument")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_INDIRECT_ARGUMENT_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/indirect_argument.hpp")
NCPP_SetGlobal(NRHI_INDIRECT_ARGUMENT_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/indirect_argument.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_indirect_argument"
    DRIVER_SPECIFIC_NAME "HD_directx12_indirect_argument"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_INDIRECT_ARGUMENT_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_INDIRECT_ARGUMENT_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/indirect_argument.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "size" "u32(
            const F_indirect_argument_desc& desc
        )"
        "alignment" "u32(
            ED_indirect_argument_type type
        )"

        "draw" "void(
			void* data_p,
			u32 vertex_count,
			u32 vertex_offset
		)"
        "draw_indexed" "void(
			void* data_p,
			u32 index_count,
			u32 index_offset,
			u32 vertex_offset
		)"
        "draw_instanced" "void(
			void* data_p,
			u32 vertex_count_per_instance,
			u32 instance_count,
			u32 vertex_offset,
			u32 instance_offset
		)"
        "draw_indexed_instanced" "void(
			void* data_p,
			u32 index_count_per_instance,
			u32 instance_count,
			u32 index_offset,
			u32 vertex_offset,
			u32 instance_offset
		)"
        "dispatch" "void(
			void* data_p,
			F_vector3_u32 thread_group_counts
		)"
        "input_buffer" "void(
			void* data_p,
			F_resource_gpu_virtual_address gpu_virtual_address,
			u32 size,
			u32 stride
		)"
        "index_buffer" "void(
			void* data_p,
			F_resource_gpu_virtual_address gpu_virtual_address,
			u32 size,
			ED_format format
		)"
        "constants" "void(
			void* data_p,
			const TG_span<u32>& values
		)"
        "constant_buffer" "void(
			void* data_p,
			F_resource_gpu_virtual_address gpu_virtual_address,
			u32 size
		)"
        "srv" "void(
			void* data_p,
			F_descriptor_gpu_address gpu_virtual_address
		)"
        "uav" "void(
			void* data_p,
			F_descriptor_gpu_address gpu_virtual_address
		)"
        "dispatch_mesh" "void(
			void* data_p,
			F_vector3_u32 thread_group_counts
		)"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement indirect argument done")