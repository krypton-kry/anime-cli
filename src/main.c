#include <mpv/client.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <gumbo.h>
#include <assert.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define internal static
#define global  static
#define persist static

#include "log.h"
#include "mpv.h"
#include "curl_util.h"
#include "string_util.h"
#include "gogoanime.h"

/*
* TODO(krypton) :
* Refactor
* feature to play next/previous video
* feature to select quality
*/
internal void clear_screen()
{
    printf("\e[1;1H\e[2J");
}

int main(int argc, char** argv)
{
    log_set_level(LOG_ERROR);
    
    if (argc < 2)
    { 
        printf("Usage : %s search_query", argv[0]);
        return 1;
    }
    
    MemStr chunk, chunk1, chunk2, chunk3;
    chunk.memory = malloc(1);
    chunk.size = 0;
    chunk1.memory = malloc(1);
    chunk1.size = 0;
    chunk2.memory = malloc(1);
    chunk2.size = 0;
    chunk3.memory = malloc(1);
    chunk3.size = 0;
    
    
    // Get search results and get input from user
    search *search_s = malloc(sizeof(search) * 30);
    
    char search_url[256];
    sprintf(search_url, "https://gogoanime.pe/search.html?keyword=%s", argv[1]);
    
    curl_get(search_url, &chunk);
    log_trace(chunk.memory);
    
    GumboOutput* output = gumbo_parse(chunk.memory);
    search_for_links(output->root, search_s);
    
    // TODO(krypton) : Find why everthing has 2 values
    for (int i = 0; i < 30; i +=2)
    {
        if (search_s[i].title)
        {
            printf( GREEN "%i.\t" CYAN "%s" RESET"\n", (i/2) + 1, search_s[i].title);
        }
    }
    printf("\nSelect anime: ");
    
    
    char input[5];
    scanf("%s", input);
    int res = atoi(input);
    
    //clear_screen();
    
    // Get total number of eps and user input again
    char anime_url[256];
    sprintf(anime_url, "https://gogoanime.pe%s", search_s[(res*2)-1].href);
    
    log_debug(search_s[(res*2)-1].href);
    
    char **split_s = split_str(search_s[(res*2)-1].href, "/");
    char *anime_slug = split_s[2];
    
    curl_get(anime_url, &chunk1);
    
    GumboOutput* result = gumbo_parse(chunk1.memory);
    
    int total_eps= 0;
    get_num_episodes(result->root, &total_eps);
    
    printf(GREEN "Enter episode to play from " CYAN "1-%i : " RESET , total_eps);
    scanf("%s", input);
    int ep_num = atoi(input);
    if (ep_num > total_eps || ep_num < 1) printf(RED "Ba-Baka It's not like I'll crash or anything");
    
    // Get embeded video link
    char epi_link[256];
    sprintf(epi_link, "https://gogoanime.pe/%s-episode-%i", anime_slug, ep_num);
    log_debug(epi_link);
    
    curl_get(epi_link, &chunk2);
    GumboOutput *epi_res = gumbo_parse(chunk2.memory);
    
    char *url = malloc(256);
    get_embed_link(epi_res->root, url);
    log_debug(url);
    
    char referer_url[256];
    sprintf(referer_url, "https:%s", url);
    log_debug(referer_url);
    
    // Get actual m3u8 link and start playing it!
    curl_get(referer_url, &chunk3);
    
    int pos_begin = strpos(chunk3.memory, "https://www");
    int pos_end = strpos(chunk3.memory, ".m3u8");
    
    // +5 for .m3u8
    char *final_url = substr(chunk3.memory, pos_begin, pos_end+5);
    log_debug("%s\n", final_url);
    
    char headers[256];
    sprintf(headers, "Referer: %s", referer_url);
    
    play_video(final_url, headers);
    
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    
    free(search_s);
    free(split_s);
    free(url);
    
    free(chunk.memory);
    free(chunk1.memory);
    free(chunk2.memory);
    free(chunk3.memory);
    
    return 0;
}

