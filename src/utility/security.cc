#include <string>
#include <iostream>
#include <vector>
#include <assert.h>
#include <openssl/hmac.h>
#include <openssl/buffer.h>

#include "utility/security.h"

namespace utility {
namespace security {

std::string Base64Encode(const unsigned char* buffer, size_t length) {
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

// Copy from Bitcoin
std::string Base58Encode(const unsigned char* pbegin, size_t len) {

  static const char* pszBase58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
  const unsigned char* pend = pbegin + len;
  // Skip & count leading zeroes.
  int zeroes = 0;
  int length = 0;
  while (pbegin != pend && *pbegin == 0) {
    pbegin++;
    zeroes++;
  }
  // Allocate enough space in big-endian base58 representation.
  int size = (pend - pbegin) * 138 / 100 + 1; // log(256) / log(58), rounded up.
  std::vector<unsigned char> b58(size);
  // Process the bytes.
  while (pbegin != pend) {
    int carry = *pbegin;
    int i = 0;
    // Apply "b58 = b58 * 256 + ch".
    for (std::vector<unsigned char>::reverse_iterator it = b58.rbegin(); (carry != 0 || i < length) && (it != b58.rend()); it++, i++) {
      carry += 256 * (*it);
      *it = carry % 58;
      carry /= 58;
    }

    assert(carry == 0);
    length = i;
    pbegin++;
  }
  // Skip leading zeroes in base58 result.
  std::vector<unsigned char>::iterator it = b58.begin() + (size - length);
  while (it != b58.end() && *it == 0)
    it++;
  // Translate the result into a string.
  std::string str;
  str.reserve(zeroes + (b58.end() - it));
  str.assign(zeroes, '1');
  while (it != b58.end())
    str += pszBase58[*(it++)];
  return str;
}

std::string Hmac(std::string data, std::string key) {

    unsigned char digest[EVP_MAX_MD_SIZE] = {'\0'};
    unsigned int digest_len = 0;

    HMAC(EVP_sha256(), (char*)key.c_str(), key.length(), (unsigned char*)data.c_str(), data.length(), digest, &digest_len);

    return Base64Encode(digest, digest_len);
}

} // namespace security
} // namespace utility
