#pragma once

#include <string>
#include <sstream>

#if defined(__clang__) || defined(__GNUG__)
#include <cxxabi.h>
#endif

/*
 * Code contents adapted from the first half of https://accu.org/journals/overload/18/95/golodetz_1608/
 * Notable changes include using variadic templates to register functions with any amount of arguments,
 * as well as getting the name of a type as a string dynamically.
 */

struct asSimpleTypeString {
    std::string name, prefix, suffix;
    explicit asSimpleTypeString(const std::string& name_) {
        this->name = name_;
    }
    std::string operator()() const {
        std::ostringstream os;
        if (!prefix.empty()) os << prefix << ' ';
        os << type();
        if (!suffix.empty()) os << ' ' << suffix;
        if (!name.empty()) os << ' ' << name;
        return os.str();
    }
    [[nodiscard]] virtual std::string type() const = 0;
    virtual asSimpleTypeString& as_param() {
        return *this;
    }
};

template<typename T>
struct asTypeString : asSimpleTypeString {
    explicit asTypeString(const std::string& name_ = "") : asSimpleTypeString(name_) {}
    [[nodiscard]] std::string type() const override {
        std::string type = typeid(T).name();
#if defined(__clang__) || defined(__GNUG__)
        int status;
        char* demangled_name = abi::__cxa_demangle(type.c_str(), nullptr, nullptr, &status);
        if (status == 0) {
            type = demangled_name;
            std::free(demangled_name);
        }
#endif
        return type;
    }
};

template<>
struct asTypeString<std::string> : asSimpleTypeString {
    explicit asTypeString(const std::string& name_ = "") : asSimpleTypeString(name_) {}
    [[nodiscard]] std::string type() const override {
        return "string";
    }
};

template<typename T>
struct asTypeString<const T> : asTypeString<T> {
    explicit asTypeString(const std::string& name_ = "") : asTypeString<T>(name_) {
        this->prefix = "const ";
    }
    asTypeString& as_param() {
        return *this;
    }
};

template<typename T>
struct asTypeString<T*> : asTypeString<T> {
    explicit asTypeString(const std::string& name_ = "") : asTypeString<T>(name_) {
        this->suffix = "@";
    }
};

template<typename T>
struct asTypeString<T&> : asTypeString<T> {
    explicit asTypeString(const std::string& name_ = "") : asTypeString<T>(name_) {
        this->suffix = "&";
    }
    asTypeString& as_param() {
        this->suffix = "&out";
        return *this;
    }
};

template<typename T>
struct asTypeString<const T&> : asTypeString<T> {
    explicit asTypeString(const std::string& name_ = "") : asTypeString<T>(name_) {
        this->prefix = "const";
        this->suffix = "&";
    }
    asTypeString& as_param() {
        this->suffix = "&in";
        return *this;
    }
};

template<typename R, typename... ArgTypes>
struct asTypeString<R(ArgTypes...)> {
    std::string name;

    explicit asTypeString(const std::string& name_) {
        this->name = name_;
    }

    std::string operator()() const {
        std::ostringstream os;
        os << asTypeString<R>()() << ' ' << name << '(';
        ((os << asTypeString<ArgTypes>().as_param()() << ", "), ...);
        os.seekp(-2, std::stringstream::cur);
        os << ")";
        return os.str();
    }
};

template<typename R, typename... ArgTypes>
struct asTypeString<R(*)(ArgTypes...)> : asTypeString<R(ArgTypes...)> {
    explicit asTypeString(const std::string &name_) : asTypeString<R(ArgTypes...)>(name_) {}
};
