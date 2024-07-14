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

#ifdef NRHI_ENABLE_DXGI
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#endif

#ifdef NRHI_DRIVER_DIRECTX_11
#include <d3d11.h>
#endif
#ifdef NRHI_DRIVER_DIRECTX_12
#include <d3d12.h>
#include <nrhi/directx12/d3dx12.h>
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
	#define NRHI_DRIVER_IF(...) if(__VA_ARGS__)
#else
	#define NRHI_DRIVER_IF(...) if constexpr (__VA_ARGS__)
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
    #define NRHI_ENUM_CASE(Value, ...) if((Value) == NCPP_GLUE(NRHI_INTERNAL_ENUM_TEMP_, NCPP_LINE)) { __VA_ARGS__; NRHI_ENUM_BREAK; }
    #define NRHI_ENUM_DEFAULT(...) { __VA_ARGS__; NRHI_ENUM_BREAK; }

#else
    #define NRHI_ENUM_SWITCH(Value, ...) NCPP_EXPAND(\
                    switch(Value)\
                    {\
                        __VA_ARGS__;\
                    };\
                )
    #define NRHI_ENUM_BREAK break
    #define NRHI_ENUM_CASE(Value, ...) case Value: { __VA_ARGS__; NRHI_ENUM_BREAK; }
    #define NRHI_ENUM_DEFAULT(...) default: { __VA_ARGS__; NRHI_ENUM_BREAK; }
                
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
#define NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_RESOURCE_BINDING(...) NRHI_DRIVER_IF(nrhi::H_driver::is_support_simple_resource_binding()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_SUPPORT_SIMPLE_RESOURCE_BINDING(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_support_simple_resource_binding()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_RESOURCE_BINDING(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_SUPPORT_SIMPLE_RESOURCE_BINDING(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(nrhi::H_driver::is_support_simple_resource_management()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_support_simple_resource_management()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION
#define NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_WORK_SUBMISSION(...) NRHI_DRIVER_IF(nrhi::H_driver::is_support_simple_work_submission()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_SUPPORT_SIMPLE_WORK_SUBMISSION(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_support_simple_work_submission()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_WORK_SUBMISSION(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_SUPPORT_SIMPLE_WORK_SUBMISSION(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_BINDING(...) \
			NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_RESOURCE_BINDING(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple resource binding (required by this function) is not supported by current driver"; }
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) \
			NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple resource management (required by this function) is not supported by current driver"; }
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(...) \
			NRHI_DRIVER_ENABLE_IF_SUPPORT_SIMPLE_WORK_SUBMISSION(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple work submission (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_BINDING(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_BINDING(...)
#endif
#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...)
#endif
#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_SUPPORT_SIMPLE_RESOURCE_BINDING(...) \
			NRHI_DRIVER_DISABLE_IF_SUPPORT_SIMPLE_RESOURCE_BINDING(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple resource binding (forbidden by this function) is supported by current driver"; }
#define NRHI_DRIVER_FORBID_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) \
			NRHI_DRIVER_DISABLE_IF_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple resource management (forbidden by this function) is supported by current driver"; }
#define NRHI_DRIVER_FORBID_SUPPORT_SIMPLE_WORK_SUBMISSION(...) \
			NRHI_DRIVER_DISABLE_IF_SUPPORT_SIMPLE_WORK_SUBMISSION(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple work submission (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
#define NRHI_DRIVER_FORBID_SUPPORT_SIMPLE_RESOURCE_BINDING(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_SUPPORT_SIMPLE_RESOURCE_BINDING(...)
#endif
#ifndef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_FORBID_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...)
#endif
#ifndef NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION
#define NRHI_DRIVER_FORBID_SUPPORT_SIMPLE_WORK_SUBMISSION(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_SUPPORT_SIMPLE_WORK_SUBMISSION(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#define NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(nrhi::H_driver::is_support_advanced_resource_binding()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_support_advanced_resource_binding()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(nrhi::H_driver::is_support_advanced_resource_management()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_support_advanced_resource_management()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
#define NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(nrhi::H_driver::is_support_advanced_work_submission()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_support_advanced_work_submission()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(...) \
			NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_RESOURCE_BINDING(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource binding (required by this function) is not supported by current driver"; }
#define NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) \
			NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource management (required by this function) is not supported by current driver"; }
#define NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_WORK_SUBMISSION(...) \
			NRHI_DRIVER_ENABLE_IF_SUPPORT_ADVANCED_WORK_SUBMISSION(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced work submission (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_BINDING(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_BINDING(...)
#endif
#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...)
#endif
#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_SUPPORT_SIMPLE_WORK_SUBMISSION(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_SUPPORT_ADVANCED_RESOURCE_BINDING(...) \
			NRHI_DRIVER_DISABLE_IF_SUPPORT_ADVANCED_RESOURCE_BINDING(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource binding (forbidden by this function) is supported by current driver"; }
#define NRHI_DRIVER_FORBID_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) \
			NRHI_DRIVER_DISABLE_IF_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource management (forbidden by this function) is supported by current driver"; }
#define NRHI_DRIVER_FORBID_SUPPORT_ADVANCED_WORK_SUBMISSION(...) \
			NRHI_DRIVER_DISABLE_IF_SUPPORT_ADVANCED_WORK_SUBMISSION(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced work submission (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#define NRHI_DRIVER_FORBID_SUPPORT_ADVANCED_RESOURCE_BINDING(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_SUPPORT_ADVANCED_RESOURCE_BINDING(...)
#endif
#ifndef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_FORBID_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...)
#endif
#ifndef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
#define NRHI_DRIVER_FORBID_SUPPORT_ADVANCED_WORK_SUBMISSION(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_SUPPORT_ADVANCED_WORK_SUBMISSION(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#define NRHI_DRIVER_DIRECTX_12_ID_MASK___RESOURCE_TYPE 0x11000000
#define NRHI_DRIVER_DIRECTX_12_ID_OFFSET___RESOURCE_TYPE 6
#define NRHI_DRIVER_DIRECTX_12_REMOVE_ID___RESOURCE_TYPE(...) (\
                (~NRHI_DRIVER_DIRECTX_12_ID_MASK___RESOURCE_TYPE) \
                & (ncpp::u32(__VA_ARGS__))\
            )
#define NRHI_DRIVER_DIRECTX_12_REMOVE_PAYLOAD___RESOURCE_TYPE(...) (\
                NRHI_DRIVER_DIRECTX_12_ID_MASK___RESOURCE_TYPE \
                & (ncpp::u32(__VA_ARGS__))\
            )
#define NRHI_DRIVER_DIRECTX_12_GET_ID___RESOURCE_TYPE(...) (\
                NRHI_DRIVER_DIRECTX_12_REMOVE_PAYLOAD___RESOURCE_TYPE(ncpp::u32(__VA_ARGS__))\
                >> NRHI_DRIVER_DIRECTX_12_ID_OFFSET___RESOURCE_TYPE\
            )
#define NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_TYPE(ID, ...) (\
                NRHI_DRIVER_DIRECTX_12_REMOVE_ID___RESOURCE_TYPE(ncpp::u32(__VA_ARGS__))\
                | NRHI_DRIVER_DIRECTX_12_REMOVE_PAYLOAD___RESOURCE_TYPE(ncpp::u32(__VA_ARGS__))\
            )

#define NRHI_DRIVER_DIRECTX_12_MAP___RESOURCE_TYPE___TO___RESOURCE_DIMENSION(...) (\
                (D3D12_RESOURCE_DIMENSION)(\
                    NRHI_DRIVER_DIRECTX_12_REMOVE_ID___RESOURCE_TYPE(ncpp::u32(__VA_ARGS__))\
                )\
            )

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#define NRHI_DRIVER_DIRECTX_12_ID_MASK___RESOURCE_BIND_FLAG 0x11110000
#define NRHI_DRIVER_DIRECTX_12_ID_OFFSET___RESOURCE_BIND_FLAG 4
#define NRHI_DRIVER_DIRECTX_12_REMOVE_ID___RESOURCE_BIND_FLAG(...) (\
                (~NRHI_DRIVER_DIRECTX_12_ID_MASK___RESOURCE_BIND_FLAG) \
                & (ncpp::u32(__VA_ARGS__))\
            )
#define NRHI_DRIVER_DIRECTX_12_REMOVE_PAYLOAD___RESOURCE_BIND_FLAG(...) (\
                NRHI_DRIVER_DIRECTX_12_ID_MASK___RESOURCE_BIND_FLAG \
                & (ncpp::u32(__VA_ARGS__))\
            )
#define NRHI_DRIVER_DIRECTX_12_GET_ID___RESOURCE_BIND_FLAG(...) (\
                NRHI_DRIVER_DIRECTX_12_REMOVE_PAYLOAD___RESOURCE_BIND_FLAG(ncpp::u32(__VA_ARGS__))\
                >> NRHI_DRIVER_DIRECTX_12_ID_OFFSET___RESOURCE_BIND_FLAG\
            )
#define NRHI_DRIVER_DIRECTX_12_GENERATE___RESOURCE_BIND_FLAG(ID, ...) (\
                NRHI_DRIVER_DIRECTX_12_REMOVE_ID___RESOURCE_BIND_FLAG(ncpp::u32(__VA_ARGS__))\
                | NRHI_DRIVER_DIRECTX_12_REMOVE_PAYLOAD___RESOURCE_BIND_FLAG(ncpp::u32(__VA_ARGS__))\
            )

#define NRHI_DRIVER_DIRECTX_12_MAP___RESOURCE_BIND_FLAG___TO___RESOURCE_FLAG(...) (\
                (D3D12_RESOURCE_FLAGS)(\
                    NRHI_DRIVER_DIRECTX_12_REMOVE_ID___RESOURCE_BIND_FLAG(ncpp::u32(__VA_ARGS__))\
                )\
            )

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
        constexpr ncpp::i32 driver_index = NRHI_DRIVER_INDEX_DEFAULT;
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
