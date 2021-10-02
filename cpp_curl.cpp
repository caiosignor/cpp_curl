#include <iostream>
#include <curl/curl.h>
#include "cJSON/cJSON.h"
#include "private.h"
#include <ctime> /* time_t, struct tm, time, gmtime */

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

//cURL make a http post request
std::string curlPost(std::string url, cJSON *data)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cJSON_Print(data));
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

cJSON *get(std::string url)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    cJSON *ret = NULL;
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        ret = cJSON_Parse(readBuffer.c_str());
    }
    return ret;
}

/**
 * @brief Get the last entries of the database based on timestamp
 * 
 * @return cJSON* object
 */
cJSON *getLastEntrie(cJSON *objects)
{
    cJSON *last = objects->child;
    cJSON *ret = last;
    do
    {
        ret = last;
        last = last->next;
    } while (last != NULL);
    return ret;
}
int main()
{
    std::time_t t = std::time(0); // t is an integer type

    cJSON *data = cJSON_CreateObject();
    cJSON_AddNumberToObject(data, "time", t);
    std::string result = curlPost(PRIVATE_URL, data);
    cJSON *saved_files = get(PRIVATE_URL);
    getLastEntrie(saved_files);
    cJSON_Delete(data);

    return 0;
}
