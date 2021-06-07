#include <stdio.h>
#include <curl/curl.h>

#include "utility/plog.h"
#include "utility/http.h"

namespace utility {

long Http::Post(const char *url, const char *data) {

  CURL *curl;
  CURLcode res;
  long http_code = 0;
 
  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    /* Now specify the POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
 
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK) {
      PLOGE("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    else { 
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    }

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();

  return http_code;
}


long Http::Put(const char *url, const char *data) {

  CURL *curl;
  CURLcode res;
  long http_code = 0;
 
  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, url);
    /* Now specify the PUT data */
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
 
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK) {
      PLOGE("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    else { 
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    }

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();

  return http_code;

}

} // namespace utility
