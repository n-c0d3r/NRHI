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

#include <nrhi/enums.hpp>
#include <nrhi/functions.hpp>

#include <nrhi/resource_desc.hpp>
#include <nrhi/resource_view_desc.hpp>
#include <nrhi/pipeline_state_desc.hpp>
#include <nrhi/shader_desc.hpp>
#include <nrhi/frame_buffer_desc.hpp>
#include <nrhi/root_signature_desc.hpp>

#include <nrhi/buffer.hpp>
#include <nrhi/texture.hpp>

#include <nrhi/shader_factory.hpp>

#include <nrhi/resource_barrier.hpp>

#include <nrhi/nsl_shader_compiler.hpp>

#ifdef NRHI_DRIVER_DIRECTX_11
#include <nrhi/directx11/buffer.hpp>
#include <nrhi/directx11/texture.hpp>
#include <nrhi/directx11/shader_resource_view.hpp>
#include <nrhi/directx11/render_target_view.hpp>
#include <nrhi/directx11/depth_stencil_view.hpp>
#include <nrhi/directx11/graphics_pipeline_state.hpp>
#include <nrhi/directx11/compute_pipeline_state.hpp>
#endif

#ifdef NRHI_DRIVER_DIRECTX_12
#include <nrhi/directx12/committed_resource.hpp>
#endif

#include <nrhi/device.external_use_only.inl>
#include <nrhi/command_list.external_use_only.inl>
#include <nrhi/command_queue.external_use_only.inl>
#include <nrhi/swapchain.external_use_only.inl>
#include <nrhi/frame_buffer.external_use_only.inl>
#include <nrhi/fence.external_use_only.inl>

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
#include <nrhi/descriptor_heap.external_use_only.inl>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

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

    NRHI_API void initialize_system(u32 driver_index = NRHI_DRIVER_INDEX_DEFAULT);
    NRHI_API void release_system();

}
