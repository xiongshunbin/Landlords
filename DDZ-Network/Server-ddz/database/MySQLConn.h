#ifndef SERVER_DDZ_MYSQLCONN_H
#define SERVER_DDZ_MYSQLCONN_H

#include <string>
#include <mysql.h>

class MySQLConn
{
public:
    // 初始化数据库连接
    MySQLConn();

    // 释放数据库连接资源
    ~MySQLConn();

    // 连接数据库
    bool connect(std::string host, std::string user, std::string passwd, std::string db, unsigned short port = 3306);

    // 执行SQL语句
    bool executeSQL(std::string sql);

    // 查询操作(select)
    bool query(std::string sql);

    // 向下遍历结果集中的数据
    bool next();

    // 得到指定索引对应的字段值
    std::string value(int index);

    // 事务 transaction
    bool transaction();

    // 提交事务
    bool commit();

    // 事务回滚
    bool rollback();

private:
    void freeResult();

private:
    MYSQL* m_conn = NULL;
    MYSQL_RES* m_result = NULL;
    MYSQL_ROW m_row = NULL;
};

#endif //SERVER_DDZ_MYSQLCONN_H
