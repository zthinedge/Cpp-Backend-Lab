#pragma once

#include <cstddef>
#include <string>

class MySqlConnection {
public:
    explicit MySqlConnection(std::size_t id)
        : id_(id)
    {
    }

    ~MySqlConnection()
    {
        close();
    }

    MySqlConnection(const MySqlConnection&) = delete;
    MySqlConnection& operator=(const MySqlConnection&) = delete;

    bool connect()
    {
        connected_ = true;
        return connected_;
    }

    void close()
    {
        connected_ = false;
    }

    bool isConnected() const
    {
        return connected_;
    }

    std::size_t id() const
    {
        return id_;
    }

    bool execute(const std::string& sql)
    {
        return connected_ && !sql.empty();
    }

private:
    std::size_t id_;
    bool connected_ = false;
};
