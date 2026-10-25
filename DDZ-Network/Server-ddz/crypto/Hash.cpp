#include "Hash.h"
#include <cassert>

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
