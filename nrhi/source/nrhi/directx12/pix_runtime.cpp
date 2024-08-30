#include <nrhi/directx12/pix_runtime.hpp>



#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
namespace nrhi
{
    HMODULE HD_directx12_pix_runtime::module_handle_ = 0;
    F_pix_begin_event_on_command_list HD_directx12_pix_runtime::begin_event_on_command_list_ = 0;
    F_pix_end_event_on_command_list HD_directx12_pix_runtime::end_event_on_command_list_ = 0;
    F_pix_set_marker_on_command_list HD_directx12_pix_runtime::set_marker_on_command_list_ = 0;



    void HD_directx12_pix_runtime::initialize()
    {
        // try to load DLL from static path
        module_handle_ = LoadLibrary(NCPP_TEXT(NRHI_WIN_PIX_EVENT_RUNTIME_DLL_PATH_X64));

        // try to load DLL from dynamic path
        if(!module_handle_)
        {
            module_handle_ = LoadLibrary(NCPP_TEXT("WinPixEventRuntime.dll"));
        }

        NCPP_ASSERT(module_handle_) << "not found WinPixEventRuntime.dll";

        begin_event_on_command_list_ = (F_pix_begin_event_on_command_list)GetProcAddress(module_handle_, "PIXBeginEventOnCommandList");
        end_event_on_command_list_   = (F_pix_end_event_on_command_list)GetProcAddress(module_handle_, "PIXEndEventOnCommandList");
        set_marker_on_command_list_  = (F_pix_set_marker_on_command_list)GetProcAddress(module_handle_, "PIXSetMarkerOnCommandList");
    }
    void HD_directx12_pix_runtime::release()
    {
        NCPP_ASSERT(module_handle_);
        FreeLibrary(module_handle_);
    }
}
#endif