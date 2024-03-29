#include <nrhi/.hpp>



namespace nrhi {

#ifdef NRHI_DRIVER_MULTIPLE
    namespace internal {
        NRHI_DEFINE_SAFE_LINKED_VARIABLE(driver_index, ncpp::i32);
    }
#endif



    void init(const F_init_desc& desc){

        try_set_driver_index(desc.driver_index);

        NRHI_ENUM_TRY_UPDATE_MAP(E_format);

    }
    void release(){

    }

}