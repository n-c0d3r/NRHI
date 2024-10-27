
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing state subobject type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_STATE_SUBOBJECT_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/state_subobject_type.hpp")
NCPP_SetGlobal(NRHI_STATE_SUBOBJECT_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/state_subobject_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_state_subobject_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_STATE_SUBOBJECT_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_STATE_SUBOBJECT_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        STATE_OBJECT_CONFIG D3D12_STATE_SUBOBJECT_TYPE_STATE_OBJECT_CONFIG
        GLOBAL_ROOT_SIGNATURE D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE
        LOCAL_ROOT_SIGNATURE D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE
        NODE_MASK D3D12_STATE_SUBOBJECT_TYPE_NODE_MASK
        DXIL_LIBRARY D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY
        SUBOBJECT_TO_EXPORTS_ASSOCIATION D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION
        DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION D3D12_STATE_SUBOBJECT_TYPE_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION
        WORK_GRAPH D3D12_STATE_SUBOBJECT_TYPE_WORK_GRAPH
        STREAM_OUTPUT D3D12_STATE_SUBOBJECT_TYPE_STREAM_OUTPUT
        BLEND D3D12_STATE_SUBOBJECT_TYPE_BLEND
        SAMPLE_MASK D3D12_STATE_SUBOBJECT_TYPE_SAMPLE_MASK
        RASTERIZER D3D12_STATE_SUBOBJECT_TYPE_RASTERIZER
        DEPTH_STENCIL D3D12_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL
        INPUT_LAYOUT D3D12_STATE_SUBOBJECT_TYPE_INPUT_LAYOUT
        IB_STRIP_CUT_VALUE D3D12_STATE_SUBOBJECT_TYPE_IB_STRIP_CUT_VALUE
        PRIMITIVE_TOPOLOGY D3D12_STATE_SUBOBJECT_TYPE_PRIMITIVE_TOPOLOGY
        RENDER_TARGET_FORMATS D3D12_STATE_SUBOBJECT_TYPE_RENDER_TARGET_FORMATS
        DEPTH_STENCIL_FORMAT D3D12_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL_FORMAT
        SAMPLE_DESC D3D12_STATE_SUBOBJECT_TYPE_SAMPLE_DESC
        FLAGS D3D12_STATE_SUBOBJECT_TYPE_FLAGS
        VIEW_INSTANCING D3D12_STATE_SUBOBJECT_TYPE_VIEW_INSTANCING
        GENERIC_PROGRAM D3D12_STATE_SUBOBJECT_TYPE_GENERIC_PROGRAM
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement state subobject type done")