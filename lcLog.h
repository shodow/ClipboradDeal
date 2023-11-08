#ifndef CLOG_H
#define CLOG_H

#include <QDebug>
#include <iostream>
#include <sstream>
using namespace std;

#define PLACEHOLDER_SIZE 2

enum LogLevel
{
    INFO,
    DEBUG,
    WARNNING,
    ERROR
};

// 函数1：递归终止函数
void write_log(int log_level, const char *code_file, int code_line, string &msg)
{
    std::string level_msg;
    switch (log_level)
    {
        case LogLevel::INFO:
        {
            level_msg = "INFO:";
            break;
        }
        case LogLevel::DEBUG:
        {
            level_msg = "DEBUG:";
            break;
        }
        case LogLevel::WARNNING:
        {
            level_msg = "WARNNING:";
            break;
        }
        case LogLevel::ERROR:
        {
            level_msg = "ERROR:";
            break;
        }
        default:
        {
            level_msg = "INFO:";
            break;
        }
    }
    qDebug() << level_msg.c_str() << msg.c_str();
}

// 函数2：展开模板函数
template<typename First, typename ...Rest>
void write_log(int log_level, const char *code_file, int code_line, string &fmt,
    const First &first, const Rest &...rest)
{
        auto idx = fmt.find("{}");
        if (idx == string::npos) { // 如果没有占位符，则忽略可变参数
            write_log(log_level, code_file, code_line, fmt);
        } else {
            ostringstream oss;
            oss << first;
            fmt.replace(idx, PLACEHOLDER_SIZE, oss.str()); // constepxr int PLACEHOLDER_SIZE=2
            write_log(log_level, code_file, code_line, fmt, rest...);
        }
}

// 函数3：可变参数模板函数
template<typename ...Args>
void write_log(int log_level, const char *code_file, int code_line, const string &fmt,
    const Args &...args)
{
    string tmp(fmt);
    write_log(log_level, code_file, code_line, tmp, args...);
}

#define LOG_INFO(fmt, ...) \
write_log(LogLevel::INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) \
write_log(LogLevel::DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_WARNNING(fmt, ...) \
write_log(LogLevel::WARNNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
write_log(LogLevel::ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)


#endif // CLOG_H
