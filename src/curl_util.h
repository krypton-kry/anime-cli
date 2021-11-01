/* date = November 1st 2021 8:32 am */

#ifndef CURL_UTIL_H
#define CURL_UTIL_H

typedef struct 
{
	char *memory;
	size_t size;
} MemStr;

internal size_t write_cb(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	MemStr *mem = (MemStr *)userp;
    
	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    
	if (!ptr)
	{
		printf("\033[0;31mNot enough memory, realloc() returned NULL\033[0m");
		return 0;
	}
    
	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
    
	return realsize;
}

internal void curl_get(char* url, MemStr* chunk)
{
    CURLcode ret;
    CURL *hnd;
    
    hnd = curl_easy_init();
    
    curl_easy_setopt(hnd, CURLOPT_URL, url);
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "chrome/73");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd,CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_cb);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *)chunk);
    
    ret = curl_easy_perform(hnd);
    
    curl_easy_cleanup(hnd);
    hnd = NULL;
    
}
#endif //CURL_UTIL_H
