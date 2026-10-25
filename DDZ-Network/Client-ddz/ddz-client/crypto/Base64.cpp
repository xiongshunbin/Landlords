#include "Base64.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

Base64::Base64(QObject *parent)
    : QObject{parent}
{}

QByteArray Base64::encode(QByteArray data)
{
    return encode(data.data(), data.size());
}

QByteArray Base64::encode(const char *data, int length)
{
    // 创建 BIO 对象
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new(BIO_s_mem());

    // 组织 BIO 链: b64 -> mem
    BIO_push(b64, mem);

    // 对数据进行编码
    BIO_write(b64, data, length);
    BIO_flush(b64);

    // 将 BIO 内存对象中的数据取出
    BUF_MEM* ptr;
    BIO_get_mem_ptr(b64, &ptr);
    QByteArray str(ptr->data, ptr->length);

    // 释放资源
    BIO_free_all(b64);

    return str;
}

QByteArray Base64::decode(QByteArray data)
{
    return decode(data.data(), data.size());
}

QByteArray Base64::decode(const char *data, int length)
{
    // 创建 BIO 对象
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new(BIO_s_mem());

    // 组织 BIO 链: b64 -> mem
    BIO_push(b64, mem);

    // 将待解码数据写入到 mem
    BIO_write(mem, data, length);

    // 解码
    char* buf = new char[length];
    int ret = BIO_read(b64, buf, length);
    QByteArray out(buf, ret);

    // 释放资源
    BIO_free_all(b64);
    delete [] buf;

    return out;
}
