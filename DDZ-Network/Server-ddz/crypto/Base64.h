#ifndef SERVER_DDZ_BASE64_H
#define SERVER_DDZ_BASE64_H

#include <string>

class Base64
{
public:
    Base64() = default;

    // 编码
    std::string encode(std::string data);
    std::string encode(const char* data, int length);

    // 解码
    std::string decode(std::string data);
    std::string decode(const char* data, int length);

private:

};

#endif //SERVER_DDZ_BASE64_H
