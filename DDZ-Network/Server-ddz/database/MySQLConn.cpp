#include "MySQLConn.h"
#include "spdlog/spdlog.h"

MySQLConn::MySQLConn()
{
    m_mysql = mysql_init(NULL);
    mysql_set_character_set(m_mysql, "utf8");
}

MySQLConn::~MySQLConn()
{
    freeResult();
    if (m_mysql != NULL)
        mysql_close(m_mysql);
}

bool MySQLConn::connect(std::string host, std::string user,
    std::string passwd, std::string db, unsigned short port)
{
    MYSQL* ptr = mysql_real_connect(m_mysql, host.c_str(), user.c_str(),
        passwd.c_str(), db.c_str(), port, NULL, 0);

    bool flag = (ptr != NULL);
    if (!flag)
    {
        spdlog::get("basic_logger")->info("MSQL数据库连接失败, 原因: {}", mysql_error(m_mysql));
    }
    return flag;
}

bool MySQLConn::executeSQL(std::string sql)
{
    int ret = mysql_query(m_mysql, sql.c_str());
    bool flag = (ret == 0);
    if (!flag)
    {
        spdlog::get("basic_logger")->info("MySQL语句执行失败, 内容: {}, 原因: {}", sql, mysql_error(m_mysql));
    }
    return flag;
}

bool MySQLConn::query(std::string sql)
{
    freeResult();
    if (!executeSQL(sql))
        return false;

    m_result = mysql_store_result(m_mysql);
    bool flag = (m_result != NULL);
    if (!flag)
    {
        spdlog::get("basic_logger")->info("MySQL结果集取出失败, 原因: {}", sql, mysql_error(m_mysql));
    }
    return flag;
}

bool MySQLConn::next()
{
    if (m_result != NULL)
    {
        m_row = mysql_fetch_row(m_result);
        if (m_row != NULL)
        {
            return true;
        }
        freeResult();
    }
    return false;
}

std::string MySQLConn::value(int index)
{
    int num_fields = mysql_num_fields(m_result);
    if (index >= num_fields || index < 0)
    {
        return std::string();
    }
    char* val = m_row[index];
    unsigned long length = mysql_fetch_lengths(m_result)[index];
    return std::string(val, length);
}

bool MySQLConn::transaction()
{
    return mysql_autocommit(m_mysql, false);
}

bool MySQLConn::commit()
{
    bool flag1 = mysql_commit(m_mysql);
    bool flag2 = mysql_autocommit(m_mysql, true);
    return flag1 && flag2;
}

bool MySQLConn::rollback()
{
    bool flag1 = mysql_rollback(m_mysql);
    bool flag2 = mysql_autocommit(m_mysql, true);
    return flag1 && flag2;
}

void MySQLConn::freeResult()
{
    if (m_result != NULL)
    {
        mysql_free_result(m_result);
        m_result = NULL;
    }
}
