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
#include <nrhi/pipeline_state_type.hpp>
#include <nrhi/shader_type.hpp>
#include <nrhi/cull_mode.hpp>
#include <nrhi/fill_mode.hpp>
#include <nrhi/depth_comparison_func.hpp>
#include <nrhi/primitive_topology.hpp>
#include <nrhi/clear_flag.hpp>
#include <nrhi/texture_cube_face.hpp>

#include <nrhi/descriptor_base.hpp>
#include <nrhi/root_signature_base.hpp>
#include <nrhi/resource_base.hpp>
#include <nrhi/resource_desc.hpp>
#include <nrhi/resource_view_desc.hpp>
#include <nrhi/pipeline_state_desc.hpp>
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
#include <nrhi/nsl_shader_compiler.hpp>
#include <nrhi/shader_class.hpp>
#include <nrhi/shader_blob.hpp>
#include <nrhi/shader_factory.hpp>
#include <nrhi/pipeline_state.hpp>
#include <nrhi/compute_pipeline_state.hpp>
#include <nrhi/graphics_pipeline_state.hpp>
#include <nrhi/frame_buffer.hpp>
#include <nrhi/sampler_state.hpp>

#ifdef NRHI_DRIVER_DIRECTX_11
#include <nrhi/directx11/buffer.hpp>
#include <nrhi/directx11/texture.hpp>
#include <nrhi/directx11/shader_resource_view.hpp>
#include <nrhi/directx11/render_target_view.hpp>
#include <nrhi/directx11/depth_stencil_view.hpp>
#include <nrhi/directx11/graphics_pipeline_state.hpp>
#include <nrhi/directx11/compute_pipeline_state.hpp>
#endif

#include <nrhi/command_list.external_use_only.inl>
#include <nrhi/command_queue.external_use_only.inl>
#include <nrhi/swapchain.external_use_only.inl>
#include <nrhi/frame_buffer.external_use_only.inl>

#include <nrhi/texture_handle.external_use_only.inl>

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
