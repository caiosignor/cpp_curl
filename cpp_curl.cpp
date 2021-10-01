#include <iostream>
#include <curl/curl.h>
#include "cJSON/cJSON.h"
#include "private.h"

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

int main()
{
    cJSON *saved_files = get(PRIVATE_URL);
    cout << "Saved files" << cJSON_Print(saved_files) << endl;
    cJSON *data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "name", "test");
    cJSON_AddNumberToObject(data, "slot", 1);
    std::string result = curlPost(PRIVATE_URL, data);
    cout << result << endl;
    return 0;
}
