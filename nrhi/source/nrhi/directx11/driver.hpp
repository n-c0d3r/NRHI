#pragma once

/** @file nrhi/directx11/driver.hpp
*
*   Implement directx11 driver.
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



#pragma region Includes

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/prerequisites.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/driver_base.hpp>

#pragma endregion



namespace nrhi {

    class NRHI_API HD_directx11_driver {

    public:
		static constexpr b8 is_interface_support_simple_resource_binding() { return true; }
		static constexpr b8 is_interface_support_simple_resource_management() { return true; }
		static constexpr b8 is_interface_support_simple_work_submission() { return true; }
		static constexpr b8 is_interface_support_advanced_resource_binding() { return false; }
		static constexpr b8 is_interface_support_advanced_resource_management() { return false; }
		static constexpr b8 is_interface_support_advanced_work_submission() { return false; }
		static constexpr b8 is_full_support_simple_resource_binding() { return true; }
		static constexpr b8 is_full_support_simple_resource_management() { return true; }
		static constexpr b8 is_full_support_simple_work_submission() { return true; }
		static constexpr b8 is_full_support_advanced_resource_binding() { return false; }
		static constexpr b8 is_full_support_advanced_resource_management() { return false; }
		static constexpr b8 is_full_support_advanced_work_submission() { return false; }
		static constexpr b8 is_support_simple_resource_binding() { return true; }
		static constexpr b8 is_support_simple_resource_management() { return true; }
		static constexpr b8 is_support_simple_work_submission() { return true; }
        static constexpr b8 is_support_advanced_resource_binding() { return false; }
        static constexpr b8 is_support_advanced_resource_management() { return false; }
        static constexpr b8 is_support_advanced_work_submission() { return false; }

    };

}
