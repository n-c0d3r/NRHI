#include <nrhi/.hpp>



namespace nrhi {

    namespace internal {

        namespace {

            b8 is_system_initialized = false;
            b8 is_system_released = false;

            struct F_system_release_checker {

                ~F_system_release_checker() {

                    NCPP_ASSERT(!(is_system_initialized && !is_system_released)) << "system have to be released";
                }

            };

            F_system_release_checker system_release_checker;

        }

    }

}



namespace nrhi {

    void initialize_system(u32 driver_index) {

        NCPP_ASSERT(!internal::is_system_initialized) << "system was already initialized and currently running";

        b8 hr = internal::try_set_driver_index(driver_index);
        NCPP_ASSERT(hr) << "can't set driver index to " << driver_index;

        H_system::initialize();

        internal::is_system_initialized = true;
        internal::is_system_released = false;
    }
    void release_system() {

        NCPP_ASSERT(internal::is_system_initialized) << "system is not initialized";

        H_system::release();

        internal::is_system_released = true;
    }



#ifdef NRHI_DRIVER_MULTIPLE
    namespace internal {
        ncpp::i32 driver_index = 0;
    }
#endif



    namespace internal {

        void try_update_map_enums(){

            NRHI_ENUM_TRY_UPDATE_MAP(E_format);
            NRHI_ENUM_TRY_UPDATE_MAP(E_resource_flag);
            NRHI_ENUM_TRY_UPDATE_MAP(E_command_list_type);
            NRHI_ENUM_TRY_UPDATE_MAP(E_resource_heap_type);
            NRHI_ENUM_TRY_UPDATE_MAP(E_descriptor_heap_type);
            NRHI_ENUM_TRY_UPDATE_MAP(E_descriptor_range_type);
            NRHI_ENUM_TRY_UPDATE_MAP(E_root_param_type);
            NRHI_ENUM_TRY_UPDATE_MAP(E_shader_visibility);
            NRHI_ENUM_TRY_UPDATE_MAP(E_resource_bind_flag);
            NRHI_ENUM_TRY_UPDATE_MAP(E_resource_misc_flag);

        }
        void try_update_map_functions(){

            NRHI_FUNCTION_CLASS_TRY_UPDATE_MAP(H_system);
            NRHI_FUNCTION_CLASS_TRY_UPDATE_MAP(H_device);
            NRHI_FUNCTION_CLASS_TRY_UPDATE_MAP(H_adapter);
            NRHI_FUNCTION_CLASS_TRY_UPDATE_MAP(H_command_queue);
            NRHI_FUNCTION_CLASS_TRY_UPDATE_MAP(H_command_list);
            NRHI_FUNCTION_CLASS_TRY_UPDATE_MAP(H_swapchain);

        }
        void try_update_map(){

            try_update_map_enums();
            try_update_map_functions();

        }

    }

}