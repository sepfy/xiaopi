#include <string>
#include <iostream>
#include <openssl/hmac.h>
#include <openssl/buffer.h>

#include "utility/security.h"

namespace utility {

std::string Security::Base64Encode(const unsigned char* buffer, size_t length) {
  BIO *bio, *b64;
  BUF_MEM *buffer_ptr;

  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);

  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
  BIO_write(bio, buffer, length);
  BIO_flush(bio);
  BIO_get_mem_ptr(bio, &buffer_ptr);
  BIO_set_close(bio, BIO_NOCLOSE);
  std::string output(buffer_ptr->data);
  BIO_free_all(bio);

  return output;
}

std::string Security::Hmac(std::string data, std::string key) {

    unsigned char digest[EVP_MAX_MD_SIZE] = {'\0'};
    unsigned int digest_len = 0;

    HMAC(EVP_sha256(), (char*)key.c_str(), key.length(), (unsigned char*)data.c_str(), data.length(), digest, &digest_len);

    return Base64Encode(digest, digest_len);
}

} // namespace utility
