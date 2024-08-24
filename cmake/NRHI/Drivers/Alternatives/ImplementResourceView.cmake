
message(STATUS "<NRHI::Drivers::Alternatives> Start implementing resource view")

include(NRHI/Utilities/AlternativeHelper)



NCPP_SetGlobal(NRHI_RESOURCE_VIEW_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_view.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_VIEW_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_view.cpp")



NRHI_AlternativeHelper_CreateAlternative(
    NAMESPACE "nrhi"
    NAME "H_resource_view"
    MAIN_HPP_FILE_PATH "${NRHI_RESOURCE_VIEW_HPP_FILE_PATH}"
    MAIN_CPP_FILE_PATH "${NRHI_RESOURCE_VIEW_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/resource_view_base.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
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
        "release_driver_specific_implementation" "void(TKPA_valid<A_resource_view> resource_view_p)"
)

message(STATUS "<NRHI::Drivers::Alternatives> Implement resource view done")