
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing texture cube face")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_TEXTURE_CUBE_FACE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/texture_cube_face.hpp")
NCPP_SetGlobal(NRHI_TEXTURE_CUBE_FACE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/texture_cube_face.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_texture_cube_face"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_TEXTURE_CUBE_FACE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_TEXTURE_CUBE_FACE_ENUM_CPP_FILE_PATH}"
    VALUES
        RIGHT 0
        LEFT 1
        UP 2
        DOWN 3
        FORWARD 4
        BACK 5
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement texture cube face done")