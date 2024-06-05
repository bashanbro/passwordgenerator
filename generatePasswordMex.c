
#define _CRT_RAND_S

#include "mex.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // 确保包含此头文件以使用strncpy_s

// 检字符串中是否存在三个或三个以上连续的数字、大写或小写字母
bool hasConsecutiveChars(const char* str);

// 将字符串转换为大写
static void toUpperCase(char* str);

// 检查字符串是否包含特定的连续字符序列
bool containsConsecutiveSequence(const char* input, const char* sequence);

// 生成密码
char* genPasswordChar(int len, bool use_digits, bool use_upper, bool use_lower, bool use_symbols, const char* symbols);

// 定义要检查的特定序列数组
const char* sequences[] = {"qwertyuiop", "poiuytrewq", "asdfghjkl", "lkjhgfdsa", "zxcvbnm", "mnbvcxz"};

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    if (nrhs != 8) {
        mexErrMsgIdAndTxt("genPW:input", "Eight input arguments required: pw_length, use_digits, use_lower, use_upper, use_symbols, use_checka, use_checkb, symbols.");
    }
    if (nlhs > 1) {
        mexErrMsgIdAndTxt("genPW:output", "There can be at most one output argument.");
    }
    /* pw_length must be a numeric */
    if (!mxIsNumeric(prhs[0])) {
        mexErrMsgIdAndTxt("genPW:input:inputNotnumeric","The first argument pw_length must be numeric");
    }
    /* use_digits must be a logical */
    if (!mxIsLogical(prhs[1])) {
        mexErrMsgIdAndTxt("genPW:input:inputNotlogical","The second argument must use_digits be logical");
    }
    /* use_lower must be a logical */
    if (!mxIsLogical(prhs[2])) {
        mexErrMsgIdAndTxt("genPW:input:inputNotlogical","The third argument use_lower must  be logical");
    }
    /* use_upper must be a logical */
    if (!mxIsLogical(prhs[3])) {
        mexErrMsgIdAndTxt("genPW:input:inputNotlogical","The fourth argument use_upper must be logical");
    }
    /* use_symbols must be a logical */
    if (!mxIsLogical(prhs[4])) {
        mexErrMsgIdAndTxt("genPW:input:inputNotlogical","The fifth argument use_symbols must be logical"); 
    }
    /* use_checka must be a logical */
    if (!mxIsLogical(prhs[5])) {
        mexErrMsgIdAndTxt("genPW:input:inputNotlogical","The sixth argument use_checka must be logical");
    }
    
    /* use_checkb must be a logical */
    if (!mxIsLogical(prhs[6])) {
        mexErrMsgIdAndTxt("genPW:input:inputNotlogical","The seventh argument use_checkb must be logical");
    }

    /* symbols must be a string */
    if (!mxIsChar(prhs[7])) {
        mexErrMsgIdAndTxt("genPW:input:inputNotString","The eighth argument symbols must be symbols");
    }

    // 获取输入参数
    double lenInput  = mxGetScalar(prhs[0]);
    bool use_digits  = mxGetScalar(prhs[1]);
    bool use_lower   = mxGetScalar(prhs[2]);
    bool use_upper   = mxGetScalar(prhs[3]);
    bool use_symbols = mxGetScalar(prhs[4]);
    bool use_checka  = mxGetScalar(prhs[5]);
    bool use_checkb  = mxGetScalar(prhs[6]);
    const char* symbols = mxArrayToString(prhs[7]);

    /* input must be a string */
    if (lenInput < 8) {
        mexErrMsgIdAndTxt("genPW:input:length","The length of password must be greater than or equal to 8.");
    } else if (lenInput>256) {
        mexErrMsgIdAndTxt("genPW:input:length","The length of password must be less than or equal to 256.");
    }

    // 确保长度至少为8
    int len = (int)lenInput;
    char* password = "";

    if(use_checka) {

        bool overallResult = true;

        while(overallResult) {

            password = genPasswordChar(len, use_digits, use_upper, use_lower, use_symbols, symbols);

            bool foundChars = hasConsecutiveChars(password);

            if (use_checkb) {
                bool foundSeq = false;

                for (int i = 0; i < 6; ++i) {
                    if (containsConsecutiveSequence(password, sequences[i])) {
                        foundSeq = true;
                        break;
                    }
                }
                overallResult = foundChars || foundSeq;
            } else {
                overallResult = foundChars;
            }
        }
    } else {
        // 生成密码
        password = genPasswordChar(len, use_digits, use_upper, use_lower, use_symbols, symbols);
    }

    // 处理输出
    if (password) {
        plhs[0] = mxCreateString(password);
        free(password); // 由于在C中分配，需要在这里释放
    } else {
        plhs[0] = mxCreateString("");
    }
    mxFree(symbols); // 释放symbols字符串
}

// 生成密码
char* genPasswordChar(int len, bool use_digits, bool use_upper, bool use_lower, bool use_symbols, const char* symbols) {
    
    if (len < 8) {
        len = 8; // 确保密码长度至少为8
    }

    srand(time(NULL)); // 初始化随机数种子

    // 计算所需总缓冲区大小
    size_t all_chars_len = 0;
    if (use_digits) all_chars_len += 10; // 数字
    if (use_upper) all_chars_len += 26; // 大写字母
    if (use_lower) all_chars_len += 26; // 小写字母
    if (use_symbols) all_chars_len += strlen(symbols); // 特殊符号

    // 初始化并分配足够的空间以容纳所有可能的字符类型
    char* all_chars = (char*)calloc(1, all_chars_len + 1); // +1是为了结束符'\0'
    if (!all_chars) return NULL;

    if (use_digits) strcat_s(all_chars, all_chars_len + 1, "0123456789");
    if (use_upper) strcat_s(all_chars, all_chars_len + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    if (use_lower) strcat_s(all_chars, all_chars_len + 1, "abcdefghijklmnopqrstuvwxyz");
    if (use_symbols) strcat_s(all_chars, all_chars_len + 1, symbols);

    // 动态分配密码字符串空间
    char* pwstr = (char*)malloc((len + 1) * sizeof(char));
    if (!pwstr) {
        free(all_chars);
        return NULL;
    }

    // 生成密码
    for (int i = 0; i < len; ++i) {
        unsigned int r;
        rand_s(&r); // 生成安全随机数
        int idx = r % all_chars_len; // 确保索引在新字符串长度内
        pwstr[i] = all_chars[idx];
    }
    pwstr[len] = '\0'; // 结尾字符
    bool found    = false;

    // 检查密码是否满足条件，如果不满足则重新生成

    while (
        (use_digits && !strpbrk(pwstr, "0123456789")) ||
        (use_upper && !strpbrk(pwstr, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")) ||
        (use_lower && !strpbrk(pwstr, "abcdefghijklmnopqrstuvwxyz")) ||
        (use_symbols && !strpbrk(pwstr, symbols))
    ) {
        // 重新生成
        for (int i = 0; i < len; ++i) {
            unsigned int r;
            rand_s(&r); // 生成安全随机数
            int idx = r % all_chars_len;
            pwstr[i] = all_chars[idx];
        }

    }

    free(all_chars); // 释放all_chars的内存
    return pwstr;
}

// 检测字符串中是否存在三个或三个以上连续的数字、大写或小写字母
bool hasConsecutiveChars(const char* str) {
    if (str == NULL || *str == '\0') return false; // 空字符串处理

    int  count = 1; // 初始化计数器，用于记录连续字符的数量
    char prevChar = str[0]; // 前一个字符

    // 遍历字符串
    for (const char* p = str + 1; *p != '\0'; ++p) {
        // 判断当前字符与前一个字符的关系
        if ((isalpha(prevChar) && isalpha(*p) && (prevChar == *p - 1 || prevChar == *p + 1)) ||
            (isdigit(prevChar) && isdigit(*p) && (prevChar == *p - 1 || prevChar == *p + 1))) {
            count++; // 连续字符数加一
            if (count >= 3) { // 已经找到三个连续字符
                return true;
            }
        }
        else {
            count = 1; // 重置计数器，因为当前字符不连续
        }
        prevChar = *p; // 更新前一个字符
    }

    return false; // 遍历结束，未找到连续三个字符
}

// 将字符串转换为大写
static void toUpperCase(char* str) {
    for (size_t i = 0; i < strlen(str); ++i) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

// 检查字符串是否包含特定的连续字符序列
bool containsConsecutiveSequence(const char* input, const char* sequence) {
    size_t seqLen = 3;
    char substring[4]; // 用于存放三个字符加上结束符'\0'
    for (size_t i = 0; i <= strlen(input) - seqLen; ++i) {
        strncpy_s(substring, sizeof(substring), &input[i], 3);
        substring[3] = '\0'; // 添加字符串结束符
        char subseq[4]; // 用于存放三个字符加上结束符'\0'
        char subseqC[4]; // 用于存放三个字符加上结束符'\0'
        //printf("子串: '%s'\n", substring);
        for (size_t j = 0; j <= strlen(sequence) - seqLen; ++j) {
            strncpy_s(subseq, sizeof(subseq), &sequence[j], 3);
            strncpy_s(subseqC, sizeof(subseqC), &sequence[j], 3);
            toUpperCase(subseqC);
            //printf("子串: '%s'\n", subseq);
            if (strncmp(substring, subseq, seqLen) == 0 || strncmp(substring, subseqC, seqLen) == 0) {
                //printf("找到匹配序列: '%s' 在位置 %zu\n", sequence, i);
                return true;
            }
        }
    }
    return false;
}
