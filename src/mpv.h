/* date = November 1st 2021 8:28 am */

#ifndef MPV_H
#define MPV_H

internal void check_mpv_error(int status)
{
    if ( status < 0 )
    {
        log_error("MPV_API : %s\n", mpv_error_string(status));
        exit(1);
    }
    
}

internal void play_video(char* url, char* header)
{
    mpv_handle* ctx = mpv_create();
    
    if ( !ctx ) log_error("MPV_API : Failed to create context!"), exit(1);
    
    int val = 1;
    check_mpv_error(mpv_set_option_string(ctx, "input-default-bindings", "yes"));
    mpv_set_option_string(ctx, "input-vo-keyboard", "yes");
    check_mpv_error(mpv_set_option(ctx, "osc", MPV_FORMAT_FLAG, &val));
    
    //NOTE(krypton): should it exit if no headers are given?
    if ( strlen(header) )
    {
        char headers[1024];
        sprintf(headers, "Referer: %s", header);
        check_mpv_error(mpv_set_option_string(ctx, "http-header-fields", header));
    }
    
    check_mpv_error(mpv_initialize(ctx));
    
    const char* cmd[] = { "loadfile", url, 0 };
    check_mpv_error(mpv_command(ctx, cmd));
    
    while(1)
    {
        mpv_event* event = mpv_wait_event(ctx, 10000);
        log_debug("event: %s\n", mpv_event_name(event->event_id));
        if ( event->event_id == MPV_EVENT_SHUTDOWN ) break;
    }
    
    mpv_terminate_destroy(ctx);
    
}

#endif //MPV_H
