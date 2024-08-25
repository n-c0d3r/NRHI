
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing resource view")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_RESOURCE_VIEW_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_view.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_VIEW_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_view.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_resource_view"
    DRIVER_SPECIFIC_NAME "HD_directx12_resource_view"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_VIEW_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_VIEW_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/resource_view.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
        "create_with_descriptor" "TU<A_resource_view>(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc,
            const F_descriptor& descriptor
        )"
        "create_srv_with_descriptor" "U_srv_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc,
            const F_descriptor& descriptor
        )"
        "create_uav_with_descriptor" "U_uav_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc,
            const F_descriptor& descriptor
        )"
        "create_rtv_with_descriptor" "U_rtv_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc,
            const F_descriptor& descriptor
        )"
        "create_dsv_with_descriptor" "U_dsv_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc,
            const F_descriptor& descriptor
        )"
        "create_unmanaged_with_descriptor" "TU<A_resource_view>(
            TKPA_valid<A_device> device_p,
            const F_descriptor& descriptor,
			ED_resource_view_type overrided_type
        )"
        "create_unmanaged_srv_with_descriptor" "U_srv_handle(
            TKPA_valid<A_device> device_p,
            const F_descriptor& descriptor
        )"
        "create_unmanaged_uav_with_descriptor" "U_uav_handle(
            TKPA_valid<A_device> device_p,
            const F_descriptor& descriptor
        )"
        "create_unmanaged_rtv_with_descriptor" "U_rtv_handle(
            TKPA_valid<A_device> device_p,
            const F_descriptor& descriptor
        )"
        "create_unmanaged_dsv_with_descriptor" "U_dsv_handle(
            TKPA_valid<A_device> device_p,
            const F_descriptor& descriptor
        )"

    ALTERNATIVE_VALUES
        "create" "TU<A_resource_view>(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc
        )"
        "create_srv" "U_srv_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc
        )"
        "create_uav" "U_uav_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc
        )"
        "create_rtv" "U_rtv_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc
        )"
        "create_dsv" "U_dsv_handle(
            TKPA_valid<A_device> device_p,
            const F_resource_view_desc& desc
        )"
        "create_default_srv" "U_srv_handle(
            TKPA_valid<A_resource> resource_p
        )"
        "create_default_uav" "U_uav_handle(
            TKPA_valid<A_resource> resource_p
        )"
        "create_default_rtv" "U_rtv_handle(
            TKPA_valid<A_resource> resource_p
        )"
        "create_default_dsv" "U_dsv_handle(
            TKPA_valid<A_resource> resource_p
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement resource view done")