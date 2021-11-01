/* date = November 1st 2021 0:45 pm */

#ifndef GOGOANIME_H
#define GOGOANIME_H

typedef struct
{
    char* href;
    char* title;
} search;

internal void search_callback(const char* title, const char* href, search *s)
{
    persist int a = 0;
    s += a;
    
    s->href = (char*) href;
    s->title = (char*) title;
    
    a++;
}

internal void num_episodes_callback(int ep_end, int *i)
{
    *i = ep_end;
}

internal void get_embed_link_callback(char *data, char* url)
{
    strcpy(url, data);
}

internal void search_for_links(GumboNode* node, search *s) 
{
    if(node->type != GUMBO_NODE_ELEMENT) return;
    
    GumboAttribute *href, *title;
    
    if ( node->v.element.tag == GUMBO_TAG_A) 
    {
        
        if ( (href = gumbo_get_attribute(&node->v.element.attributes, "href")) && strstr(href->value, "/category"))
        {
            
            log_trace(href->value);
            title = gumbo_get_attribute(&node->v.element.attributes, "title");
            search_callback(title->value, href->value, s);
            
        }
    }
    
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) 
    {
        search_for_links(children->data[i], s);
    }
    
}

internal void get_num_episodes(GumboNode* node, int *num) 
{
    //NOTE(krypton): every anime starts from 1 right? right??
    if(node->type != GUMBO_NODE_ELEMENT) return;
    
    GumboAttribute *ep_end;
    if ( node->v.element.tag == GUMBO_TAG_A && 
        (ep_end = gumbo_get_attribute(&node->v.element.attributes, "ep_end")) ) 
    {
        num_episodes_callback(atoi(ep_end->value), num);
    }
    
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) 
    {
        get_num_episodes(children->data[i], num);
    }
    
}

internal void get_embed_link(GumboNode *node, char* url)
{
    if(node->type != GUMBO_NODE_ELEMENT) return;
    
    GumboAttribute *data;
    if ( node->v.element.tag == GUMBO_TAG_A) 
    {
        if ( (data = gumbo_get_attribute(&node->v.element.attributes, "data-video")) && strstr(data->value, "/embedplus?id=") )
            get_embed_link_callback((char*)data->value, url);
    }
    
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) 
    {
        get_embed_link(children->data[i], url);
    }
    
}

#endif //GOGOANIME_H
