
add_subdirectory(hello_nrhi)

add_subdirectory(adapter)
add_subdirectory(device)
add_subdirectory(command_queue)
add_subdirectory(command_list)
if(NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION)
    add_subdirectory(command_allocator)
endif()
add_subdirectory(fence)
add_subdirectory(swapchain)
add_subdirectory(resource)
add_subdirectory(resource_binding)

add_subdirectory(nsl)