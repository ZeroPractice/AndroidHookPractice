//
// Created by bingghost on 2017/4/7.
//

#include "cstring.h"

typedef unsigned char u_char;


void* cstring::memccpy(void *dst, const void *src, int c, size_t n) {
    char *q = (char*)dst;
    const char *p = (char*)src;
    const char *p_end = p + n;
    char ch = ~(char) c;  /* ensure ch != c */

    for (; ;) {
        if (ch == c || p >= p_end) break;
        *q++ = ch = *p++;
    }

    if (p >= p_end && ch != c)
        return NULL;

    return q;
}

void* cstring::memchr(const void *s, int c, size_t n) {
    const unsigned char *p = (const unsigned char *)s;
    const unsigned char *end = p + n;

    for (; ;) {
        if (p >= end || p[0] == c) break;
        p++;
        if (p >= end || p[0] == c) break;
        p++;
        if (p >= end || p[0] == c) break;
        p++;
        if (p >= end || p[0] == c) break;
        p++;
    }
    if (p >= end)
        return NULL;
    else
        return (void *) p;
}

int cstring::memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *end1 = p1 + n;
    const unsigned char *p2 = (const unsigned char *)s2;
    int d = 0;

    for (; ;) {
        if (d || p1 >= end1) break;
        d = (int) *p1++ - (int) *p2++;

        if (d || p1 >= end1) break;
        d = (int) *p1++ - (int) *p2++;

        if (d || p1 >= end1) break;
        d = (int) *p1++ - (int) *p2++;

        if (d || p1 >= end1) break;
        d = (int) *p1++ - (int) *p2++;
    }
    return d;
}

void* cstring::memmem(const void *haystack, size_t n, const void *needle, size_t m) {
    if (m > n || !m || !n)
        return NULL;

    if (__builtin_expect((m > 1), 1)) {
        const unsigned char *y = (const unsigned char *) haystack;
        const unsigned char *x = (const unsigned char *) needle;
        size_t j = 0;
        size_t k = 1, l = 2;

        if (x[0] == x[1]) {
            k = 2;
            l = 1;
        }
        while (j <= n - m) {
            if (x[1] != y[j + 1]) {
                j += k;
            } else {
                if (!memcmp(x + 2, y + j + 2, m - 2) && x[0] == y[j])
                    return (void *) &y[j];
                j += l;
            }
        }
    } else {
        /* degenerate case */
        return memchr(haystack, ((unsigned char *) needle)[0], n);
    }
    return NULL;
}

void* cstring::memmove(void *dst, const void *src, size_t n) {
    const char *p = (const char *) src;
    char *q = (char *) dst;
    /* We can use the optimized memcpy if the destination is below the
     * source (i.e. q < p), or if it is completely over it (i.e. q >= p+n).
     */
    if (__builtin_expect((q < p) || ((size_t) (q - p) >= n), 1)) {
        return memcpy(dst, src, n);
    } else {
#define PRELOAD_DISTANCE 64
        /* a semi-optimized memmove(). we're preloading the src and dst buffers
         * as we go */
        size_t c0, c1, i;
        p += n;
        q += n;
        /* note: we preload the destination as well, because the 1-byte at a time
         * copy below doesn't take advantage of the write-buffer, we need
         * to use the cache instead as a poor man's write-combiner */
        __builtin_prefetch(p - 1);
        __builtin_prefetch(q - 1);
        if (PRELOAD_DISTANCE > 32) {
            __builtin_prefetch(p - (32 + 1));
            __builtin_prefetch(q - (32 + 1));
        }
        /* do the prefetech as soon as possible, prevent the compiler to
         * reorder the instructions above the prefetch */
        asm volatile("":::"memory");
        c0 = n & 0x1F; /* cache-line is 32 bytes */
        c1 = n >> 5;
        while (c1--) {
            /* ARMv6 can have up to 3 memory access outstanding */
            __builtin_prefetch(p - (PRELOAD_DISTANCE + 1));
            __builtin_prefetch(q - (PRELOAD_DISTANCE + 1));
            asm volatile("":::"memory");
            for (i = 0; i < 32; i++) {
                *--q = *--p;
            }
        }
        while (c0--) {
            *--q = *--p;
        }
    }

    return dst;
}

/*
* sizeof(word) MUST BE A POWER OF TWO
* SO THAT wmask BELOW IS ALL ONES
*/
typedef long word;        /* "word" used for optimal copy speed */

#define    wsize    sizeof(word)
#define    wmask    (wsize - 1)

/*
* Copy a block of memory, handling overlap.
* This is the routine that actually implements
* (the portable versions of) bcopy, memcpy, and memmove.
*/
void* cstring::memcpy(void *dst0, const void *src0, size_t length) {
    char *dst = (char*)dst0;
    const char *src = (const char *)src0;
    size_t t;

    if (length == 0 || dst == src)        /* nothing to do */
        goto done;

    /*
     * Macros: loop-t-times; and loop-t-times, t>0
     */
#define    TLOOP(s) if (t) TLOOP1(s)
#define    TLOOP1(s) do { s; } while (--t)

    if ((unsigned long) dst < (unsigned long) src) {
        /*
         * Copy forward.
         */
        t = (long) src;    /* only need low bits */
        if ((t | (long) dst) & wmask) {
            /*
             * Try to align operands.  This cannot be done
             * unless the low bits match.
             */
            if ((t ^ (long) dst) & wmask || length < wsize)
                t = length;
            else
                t = wsize - (t & wmask);
            length -= t;
            TLOOP1(*dst++ = *src++);
        }
        /*
         * Copy whole words, then mop up any trailing bytes.
         */
        t = length / wsize;
        TLOOP(*(word *) dst = *(word *) src;
                      src += wsize;
                      dst += wsize);
        t = length & wmask;
        TLOOP(*dst++ = *src++);
    } else {
        /*
         * Copy backwards.  Otherwise essentially the same.
         * Alignment works as before, except that it takes
         * (t&wmask) bytes to align, not wsize-(t&wmask).
         */
        src += length;
        dst += length;
        t = (long) src;
        if ((t | (long) dst) & wmask) {
            if ((t ^ (long) dst) & wmask || length <= wsize)
                t = length;
            else
                t &= wmask;
            length -= t;
            TLOOP1(*--dst = *--src);
        }
        t = length / wsize;
        TLOOP(src -= wsize;
                      dst -= wsize;
                      *(word *) dst = *(word *) src);
        t = length & wmask;
        TLOOP(*--dst = *--src);
    }
    done:
    return (dst0);
}

void* cstring::memrchr(const void *s, int c, size_t n) {
    if (n > 0) {
        const char *p = (const char *) s;
        const char *q = p + n;

        while (1) {
            q--;
            if (q < p || q[0] == c) break;
            q--;
            if (q < p || q[0] == c) break;
            q--;
            if (q < p || q[0] == c) break;
            q--;
            if (q < p || q[0] == c) break;
        }
        if (q >= p)
            return (void *) q;
    }
    return NULL;
}

void cstring::memswap(void *m1, void *m2, size_t n) {
    char *p = (char *) m1;
    char *p_end = p + n;
    char *q = (char *) m2;

    while (p < p_end) {
        char tmp = *p;
        *p = *q;
        *q = tmp;
        p++;
        q++;
    }
}

void* cstring::memset(void *dst, int c, size_t n) {
    char *q = (char *) dst;
    char *end = q + n;

    for (; ;) {
        if (q >= end) break;
        *q++ = (char) c;
        if (q >= end) break;
        *q++ = (char) c;
        if (q >= end) break;
        *q++ = (char) c;
        if (q >= end) break;
        *q++ = (char) c;
    }

    return dst;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* This array is designed for mapping upper and lower case letter
* together for a case independent comparison.  The mappings are
* based upon ascii character sequences.
*/
static const u_char charmap[] = {
        '\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007',
        '\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017',
        '\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027',
        '\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037',
        '\040', '\041', '\042', '\043', '\044', '\045', '\046', '\047',
        '\050', '\051', '\052', '\053', '\054', '\055', '\056', '\057',
        '\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
        '\070', '\071', '\072', '\073', '\074', '\075', '\076', '\077',
        '\100', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
        '\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
        '\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
        '\170', '\171', '\172', '\133', '\134', '\135', '\136', '\137',
        '\140', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
        '\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
        '\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
        '\170', '\171', '\172', '\173', '\174', '\175', '\176', '\177',
        '\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
        '\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217',
        '\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227',
        '\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237',
        '\240', '\241', '\242', '\243', '\244', '\245', '\246', '\247',
        '\250', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
        '\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
        '\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',
        '\300', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
        '\310', '\311', '\312', '\313', '\314', '\315', '\316', '\317',
        '\320', '\321', '\322', '\323', '\324', '\325', '\326', '\327',
        '\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337',
        '\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
        '\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
        '\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
        '\370', '\371', '\372', '\373', '\374', '\375', '\376', '\377',
};

int cstring::strcasecmp(const char *s1, const char *s2) {
    const u_char *cm = charmap;
    const u_char *us1 = (const u_char *) s1;
    const u_char *us2 = (const u_char *) s2;

    while (cm[*us1] == cm[*us2++])
        if (*us1++ == '\0')
            return (0);
    return (cm[*us1] - cm[*--us2]);
}

int cstring::strncasecmp(const char *s1, const char *s2, size_t n) {
    if (n != 0) {
        const u_char *cm = charmap;
        const u_char *us1 = (const u_char *) s1;
        const u_char *us2 = (const u_char *) s2;

        do {
            if (cm[*us1] != cm[*us2++])
                return (cm[*us1] - cm[*--us2]);
            if (*us1++ == '\0')
                break;
        } while (--n != 0);
    }
    return (0);
}

char* cstring::index(const char *p, int ch) {
    for (; ; ++p) {
        if (*p == ch)
            return ((char *) p);
        if (!*p)
            return ((char *) NULL);
    }
    /* NOTREACHED */
}

/*
* Find the first occurrence of find in s, ignore case.
*/
char* cstring::strcasestr(const char *s, const char *find) {
    char c, sc;
    size_t len;

    if ((c = *find++) != 0) {
        c = (char) tolower((unsigned char) c);
        len = cstring::strlen(find);
        do {
            do {
                if ((sc = *s++) == 0)
                    return (NULL);
            } while ((char) tolower((unsigned char) sc) != c);
        } while (strncasecmp(s, find, len) != 0);
        s--;
    }
    return ((char *) s);
}

size_t cstring::strlen(const char *str) {
    const char *s;
    for (s = str; *s; ++s);
    return (s - str);
}

char* cstring::strcat(char *s, const char *append) {
    char *save = s;

    for (; *s; ++s);
    while ((*s++ = *append++) != '\0');
    return (save);
}

char* cstring::strchr(const char *p, int ch) {
    for (; ; ++p) {
        if (*p == ch)
            return ((char *) p);
        if (!*p)
            return ((char *) NULL);
    }
    /* NOTREACHED */
}

int cstring::strcmp(const char *s1, const char *s2) {
    while (*s1 == *s2++)
        if (*s1++ == 0)
            return (0);
    return (*(unsigned char *) s1 - *(unsigned char *) --s2);
}

char* cstring::strcpy(char *to, const char *from) {
    char *save = to;

    for (; (*to = *from) != '\0'; ++from, ++to);
    return (save);
}

size_t cstring::strlcat(char *dst, const char *src, size_t siz) {
    char *d = dst;
    const char *s = src;
    size_t n = siz;
    size_t dlen;

    /* Find the end of dst and adjust bytes left but don't go past end */
    while (n-- != 0 && *d != '\0')
        d++;
    dlen = d - dst;
    n = siz - dlen;

    if (n == 0)
        return (dlen + cstring::strlen(s));
    while (*s != '\0') {
        if (n != 1) {
            *d++ = *s;
            n--;
        }
        s++;
    }
    *d = '\0';

    return (dlen + (s - src));    /* count does not include NUL */
}

/*
* Copy src to string dst of size siz.  At most siz-1 characters
* will be copied.  Always NUL terminates (unless siz == 0).
* Returns cstring::strlen(src); if retval >= siz, truncation occurred.
*/
size_t cstring::strlcpy(char *dst, const char *src, size_t siz) {
    char *d = dst;
    const char *s = src;
    size_t n = siz;

    /* Copy as many bytes as will fit */
    if (n != 0) {
        while (--n != 0) {
            if ((*d++ = *s++) == '\0')
                break;
        }
    }

    /* Not enough room in dst, add NUL and traverse rest of src */
    if (n == 0) {
        if (siz != 0)
            *d = '\0';        /* NUL-terminate dst */
        while (*s++);
    }

    return (s - src - 1);    /* count does not include NUL */
}

/*
* Concatenate src on the end of dst.  At most cstring::strlen(dst)+n+1 bytes
* are written at dst (at most n+1 bytes being appended).  Return dst.
*/
char* cstring::strncat(char *dst, const char *src, size_t n) {
    if (n != 0) {
        char *d = dst;
        const char *s = src;

        while (*d != 0)
            d++;
        do {
            if ((*d = *s++) == 0)
                break;
            d++;
        } while (--n != 0);
        *d = 0;
    }
    return (dst);
}

int cstring::strncmp(const char *s1, const char *s2, size_t n) {
    if (n == 0)
        return (0);
    do {
        if (*s1 != *s2++)
            return (*(unsigned char *) s1 - *(unsigned char *) --s2);
        if (*s1++ == 0)
            break;
    } while (--n != 0);
    return (0);
}

/*
* Copy src to dst, truncating or null-padding to always copy n bytes.
* Return dst.
*/
char* cstring::strncpy(char *dst, const char *src, size_t n) {
    if (n != 0) {
        char *d = dst;
        const char *s = src;

        do {
            if ((*d++ = *s++) == 0) {
                /* NUL pad the remaining n-1 bytes */
                while (--n != 0)
                    *d++ = 0;
                break;
            }
        } while (--n != 0);
    }
    return (dst);
}


size_t cstring::strnlen(const char *str, size_t maxlen) {
    char *p = (char *) memchr(str, 0, maxlen);

    if (p == NULL)
        return maxlen;
    else
        return (p - str);
}

/*
* Find the first occurrence in s1 of a character in s2 (excluding NUL).
*/
char* cstring::strpbrk(const char *s1, const char *s2) {
    const char *scanp;
    int c, sc;

    while ((c = *s1++) != 0) {
        for (scanp = s2; (sc = *scanp++) != 0;)
            if (sc == c)
                return ((char *) (s1 - 1));
    }
    return (NULL);
}

char* cstring::strrchr(const char *p, int ch) {
    char *save;

    for (save = NULL; ; ++p) {
        if (*p == ch)
            save = (char *) p;
        if (!*p)
            return (save);
    }
    /* NOTREACHED */
}

/*
* Get next token from string *stringp, where tokens are possibly-empty
* strings separated by characters from delim.
*
* Writes NULs into the string at *stringp to end tokens.
* delim need not remain constant from call to call.
* On return, *stringp points past the last NUL written (if there might
* be further tokens), or is NULL (if there are definitely no more tokens).
*
* If *stringp is NULL, strsep returns NULL.
*/
char* cstring::strsep(char **stringp, const char *delim) {
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;

    if ((s = *stringp) == NULL)
        return (NULL);
    for (tok = s; ;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}

/*
* Span the string s2 (skip characters that are in s2).
*/
size_t cstring::strspn(const char *s1, const char *s2) {
    const char *p = s1, *spanp;
    char c, sc;

    /*
     * Skip any characters in s2, excluding the terminating \0.
     */
    cont:
    c = *p++;
    for (spanp = s2; (sc = *spanp++) != 0;)
        if (sc == c)
            goto cont;
    return (p - 1 - s1);
}

/*
* Find the first occurrence of find in s.
*/
char* cstring::strstr(const char *s, const char *find) {
    char c, sc;
    size_t len;

    if ((c = *find++) != 0) {
        len = cstring::strlen(find);
        do {
            do {
                if ((sc = *s++) == 0)
                    return (NULL);
            } while (sc != c);
        } while (strncmp(s, find, len) != 0);
        s--;
    }
    return ((char *) s);
}

char* strtok_r(char *s, const char *delim, char **last) {
    char *spanp;
    int c, sc;
    char *tok;


    if (s == NULL && (s = *last) == NULL)
        return (NULL);

    /*
     * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
     */
    cont:
    c = *s++;
    for (spanp = (char *) delim; (sc = *spanp++) != 0;) {
        if (c == sc)
            goto cont;
    }

    if (c == 0) {        /* no non-delimiter characters */
        *last = NULL;
        return (NULL);
    }
    tok = s - 1;

    /*
     * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
     * Note that delim must have one NUL; we stop if we see that, too.
     */
    for (; ;) {
        c = *s++;
        spanp = (char *) delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *last = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}

char* cstring::strtok(char *s, const char *delim) {
    static char *last;

    return strtok_r(s, delim, &last);
}