#include "jwstring.h"
#include "cstring.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
jwstring::jwstring(const char* p_str) {
    m_pdata = NULL;
    m_len =  0;
    m_size = 0;

    set(p_str);
}


jwstring::jwstring(const jwstring& str) {
    if (str.m_pdata == NULL) {
        set("");
        return;
    }

    size_t size = str.size();
    m_pdata = new char[ m_size = size];

    m_len  = size - sizeof(char);
    cstring::strncpy(m_pdata, str.c_str(), str.size());
}


jwstring::~jwstring() {
    release();
}

///////////////////////////////////////////////////////////////////////////
//                       Member     Function
///////////////////////////////////////////////////////////////////////////
void jwstring::set(const char* p_str) {
    if (p_str == NULL) {
        p_str = "";
    }

    size_t size = cstring::strlen(p_str) + sizeof(char);
    if (m_size < size) {
        /* realloc space */
        realloc_space(size);
    }

    cstring::strncpy(m_pdata, p_str, size);
    m_len = size - sizeof(char);
}


/*
 * 不带\0拷贝字符串
 */
void jwstring::set(const char* p_data, size_t len) {
    if (p_data == NULL) {
        return;
    }

    size_t size = len + 1;
    if (m_size < size) {
        /* realloc space */
        realloc_space(size);
    }

    cstring::memcpy(m_pdata, p_data, len);
    m_len = len;
    m_size = size;
    m_pdata[len] = '\0';
}


void jwstring::set(const jwstring& str) {
    set(str.c_str());
}


void jwstring::realloc_space(size_t size) {
    release();

    m_pdata = new char[m_size = size];
}


void jwstring::add(const char* p_str) {
    if (p_str == NULL) {
        return;
    }

    size_t size = cstring::strlen(p_str) + m_len + sizeof(char);

    /* realloc space */
    if (m_size < size) {
        char* p_tmp  = new char[m_size = size];
        cstring::strncpy(p_tmp, m_pdata, m_size);

        release();
        m_pdata = p_tmp;
    }

    cstring::strcat(m_pdata, p_str);
    m_len = size - sizeof(char);
    m_size = size;
}


void jwstring::add(const jwstring& str) {
    add(str.c_str());
}


void jwstring::release() {
    if (m_pdata != NULL) {
        delete[] m_pdata;

        m_pdata = NULL;
    }

    m_size = 0;
    m_len = 0;
}


int jwstring::find(const char* p_str) {
    char* p = cstring::strstr(m_pdata, p_str);
    if (p == NULL) {
        return -1;
    }

    return (int) (p - m_pdata);
}


int jwstring::find(const jwstring& str) {
    return this->find(str.c_str());
}


int jwstring::find(const char ch) {
    char* p = cstring::index(m_pdata, ch);
    if (p == NULL) {
        return -1;
    }

    return (int) (p - m_pdata);
}


int jwstring::compare(const jwstring &str) {
    return cstring::strncmp(m_pdata, str.c_str(), m_len);
}


int jwstring::compare(const char* p_str) {
    return cstring::strncmp(m_pdata, p_str, m_len);;
}


jwstring& jwstring::operator=(const jwstring& str) {
    if (this != &str) {
        set(str.c_str());
    }

    return *this;
}


jwstring& jwstring::operator=(const char* p_str) {
    set(p_str);

    return *this;
}


bool jwstring::operator==(const jwstring& str) {
    return this->compare(str) == 0;
}


bool jwstring::operator==(const char* p_str) {
    return this->compare(p_str) == 0;
}


bool jwstring::operator!=(const jwstring& str) {
    return this->compare(str) != 0;
}


bool jwstring::operator!=(const char* p_str) {
    return this->compare(p_str) != 0;
}


bool jwstring::operator>(const jwstring& str) {
    int ret = cstring::strncmp(m_pdata, str.c_str(), m_len);

    return ret > 0;
}

bool jwstring::operator>(const char* p_str) {
    int ret = cstring::strncmp(m_pdata, p_str, m_len);

    return ret > 0;
}


bool jwstring::operator>=(const jwstring& str) {
    int ret = cstring::strncmp(m_pdata, str.c_str(), m_len);

    return ret >= 0;
}


bool jwstring::operator>=(const char* p_str) {
    int ret = cstring::strncmp(m_pdata, p_str, m_len);

    return ret >= 0;
}


bool jwstring::operator<(const jwstring& str) {
    int ret = cstring::strncmp(m_pdata, str.c_str(), m_len);

    return ret < 0;
}

bool jwstring::operator<(const char* p_str) {
    int ret = cstring::strncmp(m_pdata, p_str, m_len);

    return ret < 0;
}


char jwstring::operator[](int index) {
    return this->m_pdata[index];
}


jwstring& jwstring::operator+=(const jwstring& str) {
    add(str);

    return *this;
}

jwstring& jwstring::operator+=(const char* p_str) {
    add(p_str);

    return *this;
}

jwstring& jwstring::operator+=(int num) {
    jwstring str = jwstring::format("%d", num);
    add(str);

    return *this;    
}

const jwstring jwstring::operator+(const jwstring& str) {
    jwstring tmp(*this);
    tmp += str;
    return tmp;
}


const jwstring jwstring::operator+(const char* p_str) {
    jwstring tmp(*this);
    tmp += p_str;
    return tmp;
}


jwstring jwstring::substr(size_t start_index, size_t end_index) {
    /* check arguments */
    if ((start_index < 0) || (end_index < 0) || (start_index > m_len)
        || (end_index > m_len) || (start_index >= end_index)) {
        return "";
    }

    char* p_start = m_pdata + start_index;
    size_t len = end_index - start_index + 1;

    jwstring str;
    str.set(p_start, len);

    return str;
}


/**
 * 功  能: 最左边开始, 从左往右, 查找第一个出现ch的索引
 * 返回值:
 *    找到返回, 返回索引
 *    未找到, 返回-1
 */
int jwstring::lfind(const char ch) {
    for (int i = 0; i < m_len; ++i) {
        if (m_pdata[i] == ch) {
            return i;
        }
    }

    return -1;
}


/**
 * 功  能: 从指定索引开始, 从左往右, 查找第一个出现ch的索引
 * 返回值:
 *    找到返回, 返回索引
 *    未找到, 返回-1
 */
int jwstring::lfind(int index, const char ch) {

    if ((index < 0) || (index > m_len)) {
        return -1;
    }

    for (int i = index; i < m_len; ++i) {
        if (m_pdata[i] == ch) {
            return i;
        }
    }

    return -1;
}


/**
 * 功  能: 最右边开始, 从右往左, 查找第一个出现ch的索引
 * 返回值:
 *    找到返回, 返回索引
 *    未找到, 返回-1
 */
int jwstring::rfind(const char ch) {
    for (int i = m_len - 1; i >= 0 ; i--) {
        if (m_pdata[i] == ch) {
            return i;
        }
    }

    return -1;
}


/**
 * 功  能: 从指定索引开始, 从左往右, 查找第一个出现ch的索引
 * 返回值:
 *    找到返回, 返回索引
 *    未找到, 返回-1
 */
int jwstring::rfind(int index, const char ch) {

    if ((index < 0) || (index > m_len)) {
        return -1;
    }

    for (int i = index; i >= 0 ; i--) {
        if (m_pdata[i] == ch) {
            return i;
        }
    }

    return -1;
}


/**
 * 字符串分割
 */
vector<jwstring> jwstring::split(char delim) {
    /* 如果返回一个局部变量vector的引用是非常危险的 */
    vector<jwstring> split_list;

    int first_index = 0;
    int next_index = lfind(0, delim);
    if (next_index == -1) {
        next_index = m_len;
    }

    do {
        jwstring str = substr((size_t)first_index, (size_t)next_index - 1);
        if (str != "") {
            split_list.push_back(str);
        }

        if (next_index == m_len) {
            break;
        }

        first_index = next_index + 1;
        next_index = lfind(first_index + 1, delim);

        if (next_index == -1) {
            next_index = m_len;
        }
    } while (true);


    return split_list;
}


void jwstring::upper() {
    for (int i = 0; i < m_len; ++i) {
        m_pdata[i] = (char)toupper(m_pdata[i]);
    }
}


void jwstring::lower() {
    for (int i = 0; i < m_len; ++i) {
        m_pdata[i] = (char)tolower(m_pdata[i]);
    }
}


unsigned int jwstring::to_hex() {
    unsigned int num[8] = {0};
    unsigned int result = 0;
    unsigned int scale = 1;
    int len = m_len - 1;
    char* p_data = m_pdata;
    if ((m_pdata[0] == '0') && ((m_pdata[1] == 'x') || (m_pdata[1] == 'X'))) {
        p_data += 2;
        len -= 2;
    }

    for (int i = len; i >= 0; i--) {
        if((p_data[i] >= '0') && (p_data[i] <= '9')) {
            num[i] = (unsigned int)(p_data[i] - '0');
        } else if ((p_data[i] >= 'a') && (p_data[i] <= 'f')) {
            num[i] = (unsigned int)(p_data[i] - 'a' + 10);
        } else if ((p_data[i] >= 'A') && (p_data[i] <= 'F')) {
            num[i] = (unsigned int)(p_data[i] - 'A' + 10);
        } else {
            /* data format error */
            result = 0;
            break;
        }

        result = result + num[i] * scale;
        scale = scale * 16;
    }

    return result;
}

long jwstring::to_binary() {
    return strtol(m_pdata, NULL, 2);
}

long jwstring::to_oct() {
    return strtol(m_pdata, NULL, 8);
}


double jwstring::to_float() {
    return atof(m_pdata);
}


long jwstring::to_long() {
    return atol(m_pdata);
}


int jwstring::to_int() {
    return atoi(m_pdata);
}

void jwstring::trim(char ch) {
    int i = 0;
    int j = m_len - 1;

    while(m_pdata[i] == ch)
        ++i;

    while(m_pdata[j] == ch)
        --j;

    if ((i == 0) && (j == m_len - 1)) {
        return;
    }

    /* check files */
    m_len = (size_t)(j - i + 1);
    strncpy(m_pdata, (m_pdata + i), m_len);
    m_pdata[m_len] = '\0';
}

void jwstring::ltrim(char ch) {
    int i = 0;
    while(m_pdata[i] == ch)
        ++i;

    if (i == 0) {
        return;
    }

    m_len = m_len - i;
    strncpy(m_pdata, (m_pdata + i), m_len);
    m_pdata[m_len] = '\0';
}

void jwstring::rtrim(char ch) {
    int i = m_len - 1;
    while(m_pdata[i] == ch)
        i--;

    if (i == m_len - 1) {
        return;
    }

    m_len = (size_t)i + 1;
    m_pdata[m_len] = '\0';
}

#include <stdarg.h>
#define FORMAT_LEN 1025

/**
 * 功  能: 格式化字符串
 * 参  数: 
 * 返回值: 
 *      返回格式化后的串, 失败返回空串
 *    PS:
 *      buff固定为1024, wvsprintf() 内部buff为1024大小
 */
jwstring jwstring::format(const char *p_fmt, ...) {
    if (p_fmt == NULL) {
        return "";
    }

    va_list argList;
    char buff[FORMAT_LEN] = {0};
    
    va_start(argList, p_fmt);
    
    vsnprintf(buff, FORMAT_LEN, p_fmt, argList);

    va_end(argList);

    return buff;
}

