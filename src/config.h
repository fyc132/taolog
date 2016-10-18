#pragma once

#include <cassert>

#include <json11/json11.hpp>
#include <codecvt>

namespace taoetw {


class JsonWrapper
{
public:
    typedef json11::Json Json;

public:
    // 空对象
    JsonWrapper()
    {

    }

    // 从已有构造
    JsonWrapper(const Json& json)
        : _json(json)
    { }

    // 附加
    void attach(const Json& json)
    {
        _json = json;
    }

    // 直接返回原来的对象
    operator Json()
    {
        return _json;
    }

public:
    // 直接调用 Json 对象的方法
    const Json* operator->()
    {
        return &_json;
    }

    // 直接调用原来的 [int] 方法
    const Json& operator[](size_t i) const
    {
        return _json[i];
    }

    // 直接调用原来的 [const char*] 方法
    const Json& operator[](const char* k) const
    {
        return _json[k];
    }

public:
    // 直接设置数组值
    Json& operator[](size_t i)
    {
        return as_arr()[i];
    }

    // 直接设置对象成员值
    Json& operator[](const char* k)
    {
        return as_obj()[k];
    }

public:
    // 作为非 const 数组使用（需要手动确保为数组）
    Json::array& as_arr()
    {
        assert(_json.is_array());
        return const_cast<Json::array&>(_json.array_items());
    }

    // 作为非 const 对象使用（需要手动确保为对象）
    Json::object& as_obj()
    {
        assert(_json.is_object());
        return const_cast<Json::object&>(_json.object_items());
    }

public:
    // 获取 [k] 为数组（确保）
    JsonWrapper arr(const char* k)
    {
        assert(_json.is_object());

        if(!_json.has_shape({{k,Json::Type::ARRAY}}, _err))
            as_obj()[k] = Json::array{};

        return _json[k];
    }

    // 获取 [k]  为对象（确保）
    JsonWrapper obj(const char* k)
    {
        assert(_json.is_object());

        if(!_json.has_shape({{k,Json::Type::OBJECT}}, _err))
            as_obj()[k] = Json::object {};

        return _json[k];
    }

protected:
    Json _json;
    std::string  _err;
};

class Config
{
public:
    typedef std::wstring_convert<std::codecvt_utf8_utf16<unsigned short>, unsigned short> U8U16Cvt;

public:
    Config()
    {}

    bool load(const std::wstring& file);
    bool save();

    JsonWrapper* operator->() { return &_obj; }
    operator json11::Json() { return _obj; }
    JsonWrapper operator[](size_t i) { return _obj[i]; }
    JsonWrapper operator[](const char* k) { return _obj[k]; }

    static std::wstring ws(const std::string& s);
    static std::string us(const std::wstring& s);

protected:
    std::wstring _file;
    JsonWrapper  _obj;

};

extern Config& g_config;

}
