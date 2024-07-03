#pragma once

/** @file nrhi/prerequisites.hpp
*   @brief The file that is always included on the top.
*
*   Includes required headers,...
*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



namespace nrhi {}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#pragma region Includes

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/version.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/config.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <ncpp/.hpp>
#include <nmath/.hpp>
#include <nsurface/.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_DIRECTX_11
#include <d3d11.h>
#endif
#ifdef NRHI_DRIVER_DIRECTX_12
#include <d3d12.h>
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <spirv.hpp>
#include <spirv_cross.hpp>
#include <spirv_hlsl.hpp>
#include <spirv_glsl.hpp>
#include <spirv_msl.hpp>
#include <spirv_cpp.hpp>

#pragma endregion



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#pragma region Macros

////////////////////////////////////////////////////////////////////////////////////
//  DLL macros
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DLL

    #ifdef NRHI_DLL_IMPLEMENTATION

        #ifdef EA_COMPILER_MSVC
            #define NRHI_API      __declspec(dllexport)
            #define NRHI_LOCAL
        #elif defined(__CYGWIN__)
            #define NRHI_API      __attribute__((dllexport))
            #define NRHI_LOCAL
        #elif (defined(__GNUC__) && (__GNUC__ >= 4))
            #define NRHI_API      __attribute__ ((visibility("default")))
            #define NRHI_LOCAL    __attribute__ ((visibility("hidden")))
        #else
            #error "Unknown compiler"
        #endif

    #else

        #ifdef EA_COMPILER_MSVC
            #define NRHI_API      __declspec(dllimport)
            #define NRHI_LOCAL
        #elif defined(__CYGWIN__)
            #define NRHI_API      __attribute__((dllimport))
            #define NRHI_LOCAL
        #elif (defined(__GNUC__) && (__GNUC__ >= 4))
            #define NRHI_API      __attribute__ ((visibility("default")))
            #define NRHI_LOCAL    __attribute__ ((visibility("hidden")))
        #else
            #error "Unknown compiler"
        #endif

    #endif

#else

    #define NRHI_API
    #define NRHI_LOCAL

#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#define NRHI_USING_NLIB_NAMESPACES() \
	using namespace ncpp;\
	using namespace nmath;\
	using namespace nsurface;

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#define NRHI_ENUM_TRY_UPDATE_MAP(...) NCPP_EXPAND(__VA_ARGS__##___nrhi_enum_internal::try_update_map())
#define NRHI_FUNCTION_CLASS_TRY_UPDATE_MAP(Clear, ...) NCPP_EXPAND(__VA_ARGS__##___nrhi_function_class_internal::try_update_map(Clear))

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_MULTIPLE
    #define NRHI_DRIVER_FORWARD_FUNC_P(...) (__VA_ARGS__)
#else
    #define NRHI_DRIVER_FORWARD_FUNC_P(...) (&(__VA_ARGS__))
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_MULTIPLE
    #define NRHI_ENUM_SWITCH(Value, ...) NCPP_EXPAND(\
                    {\
                        auto NCPP_GLUE(NRHI_INTERNAL_ENUM_TEMP_, NCPP_LINE) = Value;\
                        \
                        __VA_ARGS__;\
                        \
                        NCPP_GLUE(NRHI_INTERNAL_ENUM_BREAK_POINT_, NCPP_LINE):\
                        {}\
                    };\
                )
    #define NRHI_ENUM_BREAK goto NCPP_GLUE(NRHI_INTERNAL_ENUM_BREAK_POINT_, NCPP_LINE)
    #define NRHI_ENUM_CASE(...) if((__VA_ARGS__) != NCPP_GLUE(NRHI_INTERNAL_ENUM_TEMP_, NCPP_LINE)) NRHI_ENUM_BREAK;\
    
    #define NRHI_ENUM_DEFAULT(...) \

#else
    #define NRHI_ENUM_SWITCH(Value, ...) NCPP_EXPAND(\
                    switch(Value)\
                    {\
                        __VA_ARGS__;\
                    };\
                )
    #define NRHI_ENUM_BREAK break
    #define NRHI_ENUM_CASE(...) case __VA_ARGS__: \
                
    #define NRHI_ENUM_DEFAULT(...) \
                default: \
                
#endif
#pragma endregion



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



namespace nrhi {

    NRHI_USING_NLIB_NAMESPACES();



    namespace internal {

        NRHI_API void try_update_map_enums();
        NRHI_API void try_update_map_functions(bool clear = false);
        NRHI_API void try_update_map(bool clear = false);

    }



#ifdef NRHI_DRIVER_MULTIPLE
    namespace internal {
        extern NRHI_API ncpp::i32 driver_index;
        inline ncpp::b8 try_set_driver_index(ncpp::i32 new_driver_index) noexcept {

            if(new_driver_index >= NRHI_DRIVER_COUNT)
                return false;

            internal::driver_index = new_driver_index;

            internal::try_update_map();

            return true;
        }
    }
    NCPP_FORCE_INLINE ncpp::i32 driver_index() noexcept {

        return internal::driver_index;
    }
#else
    namespace internal {
        constexpr ncpp::i32 driver_index = 0;
        inline ncpp::b8 try_set_driver_index(ncpp::i32 new_driver_index) {

            if (new_driver_index != internal::driver_index)
                return false;

            internal::try_update_map();

            return true;
        }
    }
    constexpr ncpp::i32 driver_index() {

        return internal::driver_index;
    }
#endif

}
