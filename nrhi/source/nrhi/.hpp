#pragma once



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/**
*	The root namespace of nrhi library.
*/
namespace nrhi { }



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

#include <nrhi/format.hpp>
#include <nrhi/command_list_type.hpp>
#include <nrhi/descriptor_heap_type.hpp>
#include <nrhi/descriptor_range_type.hpp>
#include <nrhi/root_param_type.hpp>
#include <nrhi/shader_visibility.hpp>
#include <nrhi/resource_bind_flag.hpp>
#include <nrhi/resource_type.hpp>
#include <nrhi/resource_heap_type.hpp>
#include <nrhi/resource_view_type.hpp>
#include <nrhi/pipeline_type.hpp>
#include <nrhi/shader_type.hpp>
#include <nrhi/descriptor_base.hpp>
#include <nrhi/root_signature_base.hpp>
#include <nrhi/resource_base.hpp>
#include <nrhi/resource_desc.hpp>
#include <nrhi/resource_view_desc.hpp>
#include <nrhi/pipeline_desc.hpp>
#include <nrhi/shader_desc.hpp>
#include <nrhi/frame_buffer_desc.hpp>
#include <nrhi/driver.hpp>
#include <nrhi/system.hpp>
#include <nrhi/device.hpp>
#include <nrhi/adapter.hpp>
#include <nrhi/command_queue.hpp>
#include <nrhi/command_list.hpp>
#include <nrhi/swapchain.hpp>
#include <nrhi/resource_view.hpp>
#include <nrhi/resource.hpp>
#include <nrhi/buffer.hpp>
#include <nrhi/texture.hpp>
#include <nrhi/format_helper.hpp>
#include <nrhi/shader.hpp>
#include <nrhi/shader_compiler.hpp>
#include <nrhi/shader_class.hpp>
#include <nrhi/shader_blob.hpp>
#include <nrhi/shader_factory.hpp>
#include <nrhi/pipeline.hpp>
#include <nrhi/compute_pipeline.hpp>
#include <nrhi/graphics_pipeline.hpp>
#include <nrhi/frame_buffer.hpp>

#ifdef NRHI_DRIVER_DIRECTX_11
#include <nrhi/directx11/buffer.hpp>
#include <nrhi/directx11/texture.hpp>
#include <nrhi/directx11/shader_resource_view.hpp>
#include <nrhi/directx11/render_target_view.hpp>
#include <nrhi/directx11/graphics_pipeline.hpp>
#include <nrhi/directx11/compute_pipeline.hpp>
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

    NRHI_API void initialize_system(u32 driver_index = 0);
    NRHI_API void release_system();

}
