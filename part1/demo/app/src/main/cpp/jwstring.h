//
// Created by bingghost on 2017/4/6.
//

#ifndef SECURITYLIBRARY_CMYSTRING_H
#define SECURITYLIBRARY_CMYSTRING_H

#include <stdlib.h>
#include <vector>

using namespace std;


class jwstring
{
private:
    char*    m_pdata;            // 字符串首地址
    size_t   m_len;              // 字符串长度 (不算最后的字符串结束符)
    size_t   m_size;             // 占用空间长度 (算最后的结束符)
public:
    jwstring(const char* = NULL);
    jwstring(const jwstring&);
    virtual ~jwstring();
public:
    /* 字符串比较 */
    int compare(const jwstring&);
    int compare(const char* p_data);

    /* 字符串释放和申请 */
    void release();
    void realloc_space(size_t size);

    /* 字符串设置 */
    void set(const char* p_str);
    void set(const jwstring& str);
    void set(const char* p_data, size_t len);

    /* 字符串拼接 */
    void add(const char* p_str);
    void add(const jwstring&);

    /* 替换 */

    /* 查找是否存在某个字符串 */
    int find(const char* p_str);
    int find(const jwstring& str);

    /* 字符查找 */
    int find(const char ch);
    int lfind(const char ch);
    int lfind(int index, const char ch);

    int rfind(const char ch);
    int rfind(int index, const char ch);

    /* 字符串搜索(暴力搜索) */
    vector<int> search(const char* p_str);
    vector<int> search(const jwstring& str);
    
    /* 模糊搜索 */


    /* 提取子串 */
    jwstring substr(size_t start_index, size_t end_index);

    /* 大小写 */
    void upper();
    void lower();

    /* 空格 */
    void trim(char ch);
    void ltrim(char ch);
    void rtrim(char ch);

    /* 字符串分割 */
    vector<jwstring> split(char delim);

    /* 进制转换 */
    long to_binary();
    long to_oct();
    unsigned int to_hex();
    double to_float();
    long to_long();
    int to_int();

public:
    /* 格式化字符串 */
    static jwstring format(const char* p_fmt,...);
    
public:
    const jwstring operator+( const jwstring& );
    const jwstring operator+( const char* );

    jwstring& operator+=(const jwstring& );
    jwstring& operator+=(const char*);
    jwstring& operator+=(int num);

    jwstring& operator=( const jwstring& );
    jwstring& operator=( const char* );

    bool operator==(const jwstring&);
    bool operator==(const char*);

    bool operator!=(const jwstring&);
    bool operator!=(const char*);

    bool operator>(const jwstring&);
    bool operator>(const char* p_str);

    bool operator>=(const jwstring& str);
    bool operator>=(const char* p_str);

    bool operator<(const jwstring&);
    bool operator<(const char* p_str);
    char operator[](int index);

    operator char*() const {
        return m_pdata;
    }

    char* c_str() const {
        return m_pdata;
    }

    size_t size() const {
        return m_size;
    }

    size_t length() const {
        return m_len;
    }
};


#endif //SECURITYLIBRARY_CMYSTRING_H
