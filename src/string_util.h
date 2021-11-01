/* date = November 1st 2021 11:33 am */

#ifndef STRING_UTIL_H
#define STRING_UTIL_H

internal char** _strsplit( const char* s, const char* delim, size_t* nb ) {
	void* data;
	char* _s = ( char* )s;
	const char** ptrs;
	size_t
		ptrsSize,
    nbWords = 1,
    sLen = strlen( s ),
    delimLen = strlen( delim );
    
	while ( ( _s = strstr( _s, delim ) ) ) {
		_s += delimLen;
		++nbWords;
	}
	ptrsSize = ( nbWords + 1 ) * sizeof( char* );
	ptrs =
        data = malloc( ptrsSize + sLen + 1 );
	if ( data ) {
		*ptrs =
            _s = strcpy( ( ( char* )data ) + ptrsSize, s );
		if ( nbWords > 1 ) {
			while ( ( _s = strstr( _s, delim ) ) ) {
				*_s = '\0';
				_s += delimLen;
				*++ptrs = _s;
			}
		}
		*++ptrs = NULL;
	}
	if ( nb ) {
		*nb = data ? nbWords : 0;
	}
	return data;
}

internal char** split_str(char* str, char* delim)
{
    return _strsplit( str, delim, 0);
}

char* substr(const char *src, int m, int n)
{
    int len = n - m;
    
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
    strncpy(dest, (src + m), len);
    
    return dest;
}

int strpos(char *haystack, char *needle)
{
    char *p = strstr(haystack, needle);
    if (p)
        return p - haystack;
    return 0;
}

#endif //STRING_UTIL_H
