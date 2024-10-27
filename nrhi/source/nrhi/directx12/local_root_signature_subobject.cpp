#include <nrhi/directx12/local_root_signature_subobject.hpp>
#include <nrhi/directx12/root_signature.hpp>



namespace nrhi
{
    void HD_directx12_local_root_signature_subobject::set_root_signature_p(void* payload_p, TKPA_valid<A_root_signature> root_signature_p)
    {
        ((CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT*)payload_p)->SetRootSignature(
            root_signature_p.T_cast<F_directx12_root_signature>()->d3d12_root_signature_p()
        );
    }
}