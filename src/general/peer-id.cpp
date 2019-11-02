/**
 * PeerId
 *
 * @file peer-id.cpp
 * @author Matthew Knight
 * @date 2019-09-12
 */

#include "libp2p/general/peer-id.hpp"
#include "peer-id.pb.h"

#include <cryptopp/osrng.h>
#include <cryptopp/xed25519.h>
#include <multiformats/cid.hpp>
#include <multiformats/multihash.hpp>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

namespace {
    Libp2p::PeerId::KeyType pb_to_key_type(::KeyType key_type) {
        switch (key_type) {
        case ::KeyType::Rsa:
            return Libp2p::PeerId::KeyType::Rsa;
        case ::KeyType::Ed25519:
            return Libp2p::PeerId::KeyType::Ed25519;
        case ::KeyType::Secp256k1:
            return Libp2p::PeerId::KeyType::Secp256k1;
        case ::KeyType::Ecdsa:
            return Libp2p::PeerId::KeyType::Ecdsa;
        }

        // if not found throw exception
        throw std::runtime_error("unknown key type for peer id");
    }

    ::KeyType key_type_to_pb(Libp2p::PeerId::KeyType key_type) {
        switch (key_type) {
        case Libp2p::PeerId::KeyType::Rsa:
            return ::KeyType::Rsa;
        case Libp2p::PeerId::KeyType::Ed25519:
            return ::KeyType::Ed25519;
        case Libp2p::PeerId::KeyType::Secp256k1:
            return ::KeyType::Secp256k1;
        case Libp2p::PeerId::KeyType::Ecdsa:
            return ::KeyType::Ecdsa;
        }

        // if not found throw exception
        throw std::runtime_error("unknown key type for peer id");
    }
} // namespace

namespace Libp2p {
    /** @brief Generate new peer-id */
    PeerId::PeerId(PeerId::KeyType key_type)
        : key_type(key_type) {

        // generate public and private keys
        switch (key_type) {
        case Libp2p::PeerId::KeyType::Rsa: {
            EVP_PKEY* pkey = nullptr;
            EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
            if (!ctx)
                throw std::runtime_error("ctx");
            if (EVP_PKEY_keygen_init(ctx) <= 0)
                throw std::runtime_error("keygen_init");
            if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0)
                throw std::runtime_error("set_rsa_kgen_bits");
            if (EVP_PKEY_keygen(ctx, &pkey) <= 0)
                throw std::runtime_error("keygen");

            int len;
            unsigned char* buf;
            X509* x509 = X509_new();
            if (X509_set_pubkey(x509, pkey) <= 0)
                throw std::runtime_error("x509");

            // public is DER encoded pkix(X.509) format

            len = i2d_X509(x509, nullptr);
            if (len < 0)
                throw std::runtime_error("x509 len");
            std::fill_n(std::back_inserter(public_key), len, 0);
            buf = public_key.data();
            len = i2d_X509(x509, &buf);
            if (len < 0)
                throw std::runtime_error("x509 encode");

            // private is PKCS1 using ASN.1 DER
            RSA* rsa = EVP_PKEY_get1_RSA(pkey);
            len = i2d_RSAPrivateKey(rsa, nullptr);
            if (len < 0)
                throw std::runtime_error("private key len");
            std::fill_n(std::back_inserter(private_key), len, 0);
            buf = private_key.data();
            len = i2d_RSAPrivateKey(rsa, &buf);
            if (len < 0)
                throw std::runtime_error("private key encode");

            X509_free(x509);
            RSA_free(rsa);
            EVP_PKEY_CTX_free(ctx);
            break;
        }
        case Libp2p::PeerId::KeyType::Ed25519: {
            CryptoPP::AutoSeededRandomPool prng;
            CryptoPP::ed25519::Signer signer;
            signer.AccessPrivateKey().GenerateRandom(prng);

            auto const& exp = dynamic_cast<const CryptoPP::ed25519PrivateKey&>(
                                  signer.GetPrivateKey())
                                  .GetPrivateExponent();
            std::cout << exp << std::endl;
            break;
        }
        default:
            throw std::runtime_error("unsupported key type for peer id");
        }
    }

    /** @brief Convert public key to CID */
    Multiformats::Cid PeerId::to_cid() {
        PublicKey pb;
        std::vector<std::uint8_t> serialized;

        pb.set_type(key_type_to_pb(key_type));
        pb.set_data(public_key.data(), public_key.size());
        std::fill_n(std::back_inserter(serialized), pb.ByteSizeLong(), 0);
        pb.SerializeToArray(serialized.data(), serialized.size());

        return {
            1, 0x72,
            Multiformats::Multihash{serialized.cbegin(), serialized.cend()}};
    }
} // namespace Libp2p
