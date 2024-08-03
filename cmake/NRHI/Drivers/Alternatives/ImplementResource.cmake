
message(STATUS "<NRHI::Drivers::Alternatives> Start implementing resource")

include(NRHI/Utilities/AlternativeHelper)



NCPP_SetGlobal(NRHI_RESOURCE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource.cpp")



NRHI_AlternativeHelper_CreateAlternative(
    NAMESPACE "nrhi"
    NAME "H_resource"
    MAIN_HPP_FILE_PATH "${NRHI_RESOURCE_HPP_FILE_PATH}"
    MAIN_CPP_FILE_PATH "${NRHI_RESOURCE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/resource_base.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
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
        "map" "F_mapped_subresource(
            TKPA_valid<A_resource> resource_p,
            u32 subresource_index
        )"
        "unmap" "void(
            TKPA_valid<A_resource> resource_p,
            u32 subresource_index
        )"
)

message(STATUS "<NRHI::Drivers::Alternatives> Implement resource done")