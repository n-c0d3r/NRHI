#include <nrhi/.hpp>



namespace nrhi {

#ifdef NRHI_DRIVER_MULTIPLE
    namespace internal {
        ncpp::i32 driver_index = 0;
    }
#endif



    void try_update_map_enums(){

        NRHI_ENUM_TRY_UPDATE_MAP(E_format);
        NRHI_ENUM_TRY_UPDATE_MAP(E_resource_flag);

    }

}