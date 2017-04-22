#include "stdafx.h"
#include "cipher.h"

namespace Crypto{

Cipher::Cipher():m_PrivateKey(RSA_new(), RSA_free),m_PublicKey(RSA_new(), RSA_free)
{
    //Initialize openssl components
    ERR_load_CRYPTO_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_CONFIG, NULL);

    ptrRSA rsaPair(RSA_new(), RSA_free);
    std::unique_ptr<BIGNUM, decltype(&::BN_free)> pBignum(BN_new(), BN_free);


    if(!BN_set_word(pBignum.get(), RSA_F4))
    {
        qCritical()<<"Could not create bignum word"<<ERR_error_string(ERR_get_error(),NULL);
    }

    //Generate RSA key pair
    if(!RSA_generate_key_ex(rsaPair.get(),2048,pBignum.get(),NULL))
    {
        qCritical()<<"Could not generate RSA key pair"<<ERR_error_string(ERR_get_error(),NULL);
    }

    m_PrivateKey = ptrRSA(RSAPrivateKey_dup(rsaPair.get()), RSA_free);
    m_PublicKey = ptrRSA(RSAPublicKey_dup(rsaPair.get()), RSA_free);
}

Crypto::Cipher::~Cipher()
{
    ERR_free_strings();
}

QByteArray Cipher::GetPublicKey()
{
    QByteArray PublicKeyByte;
    ptrBIO bio(BIO_new(BIO_s_mem()), BIO_free);
    BUF_MEM* pBio = NULL;

    PEM_write_bio_RSAPublicKey(bio.get(), m_PublicKey.get());
    BIO_get_mem_ptr(bio.get(), &pBio);

    PublicKeyByte.append(pBio->data);
    return PublicKeyByte;
}

QByteArray Cipher::EncryptData(QByteArray publicKey, QByteArray &data)
{
    QByteArray buffer;
    int dataLen = data.size();
    const unsigned char* str = static_cast<unsigned char*>(static_cast<void*>(data.data()));

    //Read Public Key from QByteArray to RSA struct
    ptrBIO pBio (BIO_new(BIO_s_mem()), BIO_free);
    BIO_write(pBio.get(), publicKey.constData(), publicKey.length());
    ptrRSA rsaKey(PEM_read_bio_RSAPublicKey(pBio.get(), NULL, NULL, NULL), RSA_free);

    int keyLen = RSA_size(rsaKey.get());
    unsigned char* encryptedData = static_cast<unsigned char*>(malloc(keyLen));

    int resultLen = RSA_public_encrypt(dataLen, str, encryptedData, rsaKey.get(), RSA_PKCS1_OAEP_PADDING);

    if(resultLen == -1)
    {
        qCritical()<<"Could not encrypt: "<<ERR_error_string(ERR_get_error(),NULL);
        return buffer;
    }

    buffer = QByteArray(static_cast<char*>(static_cast<void*>(encryptedData)), resultLen);

    return buffer;
}

QByteArray Cipher::DecryptData(QByteArray &data)
{
    QByteArray buffer;

    const unsigned char* str = static_cast<unsigned char*>(static_cast<void*>(data.data()));
    int keyLen = RSA_size(m_PrivateKey.get());

    unsigned char* decryptedData = static_cast<unsigned char*>(malloc(keyLen));
    int resultLen = RSA_private_decrypt(keyLen, str, decryptedData, m_PrivateKey.get(), RSA_PKCS1_OAEP_PADDING);
    if(resultLen == -1)
    {
        qCritical()<<"Could not decrypt: "<<ERR_error_string(ERR_get_error(),NULL);
        return buffer;
    }

    buffer = QByteArray(static_cast<char*>(static_cast<void*>(decryptedData)), resultLen);
    return buffer;
}

}
