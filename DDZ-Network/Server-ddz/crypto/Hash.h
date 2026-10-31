#ifndef SERVER_DDZ_HASH_H
#define SERVER_DDZ_HASH_H

#include <string>
#include <map>
#include <google/protobuf/stubs/strutil.h>
#include <openssl/evp.h>
#include <openssl/md4.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

using HashFunc = const EVP_MD *(*)(void);

enum class HashAlgorithm : char
{
    Md4,
    Md5,
    Sha1,
    Sha224,
    Sha256,
    Sha384,
    Sha512,
    Sha3_224,
    Sha3_256,
    Sha3_384,
    Sha3_512,
};

const std::map<HashAlgorithm, std::pair<HashFunc, int>> HashMethods = {
    {HashAlgorithm::Md4, {EVP_md4, MD4_DIGEST_LENGTH}},
    {HashAlgorithm::Md5, {EVP_md5, MD5_DIGEST_LENGTH}},
    {HashAlgorithm::Sha1, {EVP_sha1, SHA_DIGEST_LENGTH}},
    {HashAlgorithm::Sha224, {EVP_sha224, SHA224_DIGEST_LENGTH}},
    {HashAlgorithm::Sha256, {EVP_sha256, SHA256_DIGEST_LENGTH}},
    {HashAlgorithm::Sha384, {EVP_sha384, SHA384_DIGEST_LENGTH}},
    {HashAlgorithm::Sha512, {EVP_sha512, SHA512_DIGEST_LENGTH}},
    {HashAlgorithm::Sha3_224, {EVP_sha3_224, SHA224_DIGEST_LENGTH}},
    {HashAlgorithm::Sha3_256, {EVP_sha3_256, SHA256_DIGEST_LENGTH}},
    {HashAlgorithm::Sha3_384, {EVP_sha3_384, SHA384_DIGEST_LENGTH}},
    {HashAlgorithm::Sha3_512, {EVP_sha3_512, SHA512_DIGEST_LENGTH}}
};

class Hash
{
public:
    enum class Type : char
    {
        Binary,
        Hex
    };

    explicit Hash(HashAlgorithm algorithm);
    ~Hash();

    // 添加数据
    void addData(std::string data);
    void addData(const char *data, int length);

    // 计算哈希值
    std::string result(Type type = Type::Hex);

    static std::string hash(std::string data, HashAlgorithm algorithm, Type type = Type::Binary);
    static int hashLength(HashAlgorithm algorithm);

private:
    EVP_MD_CTX* m_ctx = NULL;
    HashAlgorithm m_algorithm;
};

#endif //SERVER_DDZ_HASH_H
