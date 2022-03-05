#pragma once

#include <string>
#include <string_view>
#include <memory>

class CowString {
public:
    class Buffer {
    public:
        class Proxy {
        public:
            Proxy() = delete;
            Proxy(CowString& cow_str, size_t index);

            Proxy operator=(char c);

            operator const char() const;

        private:
            CowString& cow_str_;
            size_t index_;
        };

        class Iterator {
        public:
            Iterator() = delete;
            Iterator(const CowString& cow_str, size_t index);

            Iterator operator++();
            bool operator!=(const CowString::Buffer::Iterator& other) const;
            Proxy operator*();

        private:
            CowString& cow_str_;
            size_t index_;
        };

        Buffer(const std::string_view string);
        Buffer(const Buffer& other);
//        ~Buffer();

        void IncrementCounter();
        void DecrementCounter();
        void AddString(std::string_view string);
        char* GetData();
        size_t Size() const;
        size_t UseCount() const;
        char Get(size_t index);

    private:
        void Set(size_t index, char c);

        std::string str_;
        size_t counter_ = 0;
    };


    CowString(const std::string_view string);
    CowString(const CowString& cow_string);
    CowString(CowString&& cow_string);

    ~CowString();

    CowString& operator=(const CowString& other);
    CowString& operator=(CowString&& other);
    CowString& operator+=(const std::string_view other);
    CowString& operator+=(const CowString& other);

    char* GetData() const;

    Buffer::Iterator begin() const;
    Buffer::Iterator end() const;

    char At(size_t index) const;

    Buffer::Proxy operator[](size_t index);

    bool operator==(const std::string_view str) const;
    bool operator!=(const std::string_view str) const;

    std::string operator+(const std::string& other) const;
    std::string operator+(const CowString& other) const;

    operator const std::string_view() const;

private:
    void ChangeBuffer();

    Buffer* buf_ptr_;
};

bool operator==(const std::string_view str, const CowString& cow_str);
bool operator!=(const std::string_view str, const CowString& cow_str);
std::string operator+(const std::string& str, const CowString& cow_str);