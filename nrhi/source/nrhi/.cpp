#include <nrhi/.hpp>



namespace nrhi {

#ifdef NRHI_DRIVER_MULTIPLE
    namespace internal {
        NRHI_DEFINE_SAFE_LINKED_VARIABLE(driver_index, ncpp::i32);
    }
#endif



    void try_update_map_enums(){

        NRHI_ENUM_TRY_UPDATE_MAP(E_format);
        NRHI_ENUM_TRY_UPDATE_MAP(E_resource_flag);

    }

}