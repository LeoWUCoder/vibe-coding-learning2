#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

int max_index(const int *arr, int n);
int parse_int10(const char *s, int *out_value);
void *memdup(const void *src, size_t size);
int normalize_spaces(char *s);
int write_all(int fd, const void *buf, size_t len);
size_t strlcpy_safe(char *dst, const char *src, size_t dst_size);
int is_palindrome_ascii(const char *s);

int max_index(const int *arr, int n) {
    if (!arr || n <= 0) return -1;
    int idx = 0;
    int maxv = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > maxv) {
            maxv = arr[i];
            idx = i;
        }
    }
    return idx;
}

int parse_int10(const char *s, int *out_value) {
    if (!s || !out_value) return -1;

    while (isspace((unsigned char)*s)) s++;

    int sign = 1;
    if (*s == '+' || *s == '-') {
        if (*s == '-') sign = -1;
        s++;
    }

    if (!isdigit((unsigned char)*s)) return -1;

    long long val = 0;
    while (isdigit((unsigned char)*s)) {
        val = val * 10 + (*s - '0');
        long long signed_val = val * sign;
        if (signed_val > INT_MAX || signed_val < INT_MIN) {
            return -1;
        }
        s++;
    }

    while (isspace((unsigned char)*s)) s++;
    if (*s != '\0') return -1;

    *out_value = (int)(val * sign);
    return 0;
}

void *memdup(const void *src, size_t size) {
    if (!src || size == 0) return NULL;
    void *p = malloc(size);
    if (!p) return NULL;
    memcpy(p, src, size);
    return p;
}

int normalize_spaces(char *s) {
    if (!s) return 0;

    int r = 0;
    int w = 0;

    while (s[r] == ' ') r++;

    int in_space = 0;
    for (; s[r] != '\0'; r++) {
        char c = s[r];
        if (c == ' ') {
            if (!in_space) {
                s[w++] = ' ';
                in_space = 1;
            }
        } else {
            s[w++] = c;
            in_space = 0;
        }
    }

    if (w > 0 && s[w - 1] == ' ') {
        w--;
    }
    s[w] = '\0';
    return w;
}

int write_all(int fd, const void *buf, size_t len) {
    const char *p = (const char *)buf;
    size_t left = len;
    while (left > 0) {
        ssize_t n = write(fd, p, left);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (n == 0) {
            errno = EIO;
            return -1;
        }
        p += n;
        left -= (size_t)n;
    }
    return 0;
}

size_t strlcpy_safe(char *dst, const char *src, size_t dst_size) {
    if (!dst || !src || dst_size == 0) {
        return src ? strlen(src) : 0;
    }
    size_t src_len = strlen(src);
    size_t copy_len = (src_len >= dst_size) ? (dst_size - 1) : src_len;
    if (copy_len > 0) {
        memcpy(dst, src, copy_len);
    }
    dst[copy_len] = '\0';
    return src_len;
}

int is_palindrome_ascii(const char *s) {
    if (!s) return 0;
    int i = 0;
    int j = (int)strlen(s) - 1;
    while (i < j) {
        while (i < j && !isalnum((unsigned char)s[i])) i++;
        while (i < j && !isalnum((unsigned char)s[j])) j--;
        if (i < j) {
            char a = (char)tolower((unsigned char)s[i]);
            char b = (char)tolower((unsigned char)s[j]);
            if (a != b) return 0;
            i++;
            j--;
        }
    }
    return 1;
}

int main(void) {
    int a[] = {3, 7, 2, 9, 5};
    printf("max_index(a) = %d\n", max_index(a, 5));

    int x;
    if (parse_int10("   -42", &x) == 0) {
        printf("parse_int10(\"   -42\") = %d\n", x);
    } else {
        printf("parse_int10 failed\n");
    }

    const char *hello = "hello";
    char *dup = (char *)memdup(hello, strlen(hello) + 1);
    if (dup) {
        printf("memdup: %s\n", dup);
        free(dup);
    }

    char buf[128];
    strlcpy_safe(buf, "   the   quick  brown   fox   ", sizeof(buf));
    int newlen = normalize_spaces(buf);
    printf("normalize_spaces: '%s' (len=%d)\n", buf, newlen);

    const char *p1 = "A man, a plan, a canal: Panama";
    const char *p2 = "ab";
    printf("is_palindrome('%s') = %d\n", p1, is_palindrome_ascii(p1));
    printf("is_palindrome('%s') = %d\n", p2, is_palindrome_ascii(p2));

    const char *out = "write_all -> Hello, stdout!\n";
    if (write_all(STDOUT_FILENO, out, strlen(out)) != 0) {
        perror("write_all");
    }

    return 0;
}