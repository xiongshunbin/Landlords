#include "Hash.h"
#include <cassert>
#include <memory>

Hash::Hash(HashAlgorithm algorithm)
{
    m_algorithm = algorithm;
    m_ctx = EVP_MD_CTX_new();
    assert(m_ctx != NULL);

    int ret = EVP_DigestInit_ex(m_ctx, (HashMethods.at(algorithm).first)(), NULL);
    assert(ret == 1);
}

Hash::~Hash()
{
    if (m_ctx != NULL)
        EVP_MD_CTX_free(m_ctx);
}

void Hash::addData(std::string data)
{
    addData(data.data(), data.size());
}

void Hash::addData(const char *data, int length)
{
    int ret = EVP_DigestUpdate(m_ctx, data, length);
    assert(ret == 1);
}

std::string Hash::result(Type type)
{
    unsigned char md[HashMethods.at(m_algorithm).second];
    unsigned int len = 0;
    int ret = EVP_DigestFinal_ex(m_ctx, md, &len);
    assert(ret == 1);
    if (type == Type::Hex)
    {
        char res[len * 2];
        for (int i = 0; i < len; i++)
        {
            sprintf(&res[i * 2], "%02x", md[i]);
        }
        return std::string(res, len * 2);
    }
    return std::string(reinterpret_cast<char*>(md), len);
}

std::string Hash::hash(std::string data, HashAlgorithm algorithm, Type type)
{
    std::shared_ptr<EVP_MD_CTX> p_ctx(EVP_MD_CTX_new(), [](EVP_MD_CTX* ctx) {
        if (ctx != NULL)
        {
            EVP_MD_CTX_free(ctx);
        }
    });
    assert(p_ctx != NULL);

    int ret = EVP_DigestInit_ex(p_ctx.get(), (HashMethods.at(algorithm).first)(), NULL);
    assert(ret == 1);

    ret = EVP_DigestUpdate(p_ctx.get(), data.data(), data.size());
    assert(ret == 1);

    unsigned char md[HashMethods.at(algorithm).second];
    unsigned int len = 0;
    ret = EVP_DigestFinal_ex(p_ctx.get(), md, &len);
    assert(ret == 1);
    if (type == Type::Hex)
    {
        char res[len * 2];
        for (int i = 0; i < len; i++)
        {
            sprintf(&res[i * 2], "%02x", md[i]);
        }
        return std::string(res, len * 2);
    }
    return std::string(reinterpret_cast<char*>(md), len);
}

int Hash::hashLength(HashAlgorithm algorithm)
{
    auto it = HashMethods.find(algorithm);
    if (it == HashMethods.end())
        return -1;

    return it->second.second;
}
