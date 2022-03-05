#include "cow_string.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>

CowString::Buffer::Proxy::Proxy(CowString& cow_str, size_t index)
    : cow_str_(cow_str), index_(index) {}

CowString::Buffer::Proxy CowString::Buffer::Proxy::operator=(char c) {
    if (cow_str_.buf_ptr_->Get(index_) != c && cow_str_.buf_ptr_->counter_ > 1) {
        cow_str_.ChangeBuffer();
    }
    cow_str_.buf_ptr_->Set(index_, c);
    return *this;
}

CowString::Buffer::Proxy::operator const char() const {
    return cow_str_.buf_ptr_->Get(index_);
}

CowString::Buffer::Iterator::Iterator(const CowString& cow_str, size_t index)
    : cow_str_(const_cast<CowString&>(cow_str)), index_(index) {}

CowString::Buffer::Iterator CowString::Buffer::Iterator::operator++() {
    index_++;
    return *this;
}

bool CowString::Buffer::Iterator::operator!=(const CowString::Buffer::Iterator& other) const {
    return std::tuple(&(this->cow_str_.buf_ptr_), this->index_) != std::tuple(&(other.cow_str_.buf_ptr_), other.index_);
}

CowString::Buffer::Proxy CowString::Buffer::Iterator::operator*() {
    return Proxy(cow_str_, index_);
}

CowString::Buffer::Buffer(const std::string_view string)
    : str_(string), counter_(1) {}

CowString::Buffer::Buffer(const CowString::Buffer& other)
    : str_(other.str_), counter_(1) {}

//CowString::Buffer::~Buffer() {
//
//}

void CowString::Buffer::IncrementCounter() {
    counter_++;
}

char* CowString::Buffer::GetData() {
    return &str_[0];
}

size_t CowString::Buffer::Size() const {
    return str_.size();
}

size_t CowString::Buffer::UseCount() const {
    return counter_;
}

void CowString::Buffer::Set(size_t index, char c) {
    str_[index] = c;
}

char CowString::Buffer::Get(size_t index) {
    return str_[index];
}

void CowString::Buffer::DecrementCounter() {
    if (--counter_ == 0) {
//        std::cout << "deleting buffer\n";
        delete this;
    }
}

void CowString::Buffer::AddString(std::string_view string) {
    str_ += string;
}

CowString::CowString(const std::string_view string)
    : buf_ptr_(new Buffer(string)) {}

CowString::CowString(const CowString& cow_string) : buf_ptr_(cow_string.buf_ptr_) {
    buf_ptr_->IncrementCounter();
}

CowString::CowString(CowString&& cow_string) {
    this->buf_ptr_ = cow_string.buf_ptr_;
    cow_string.buf_ptr_ = nullptr;
}

CowString::~CowString() {
    buf_ptr_->DecrementCounter();
}

CowString& CowString::operator=(const CowString& other) {
    buf_ptr_->DecrementCounter();
    buf_ptr_ = other.buf_ptr_;
    buf_ptr_->IncrementCounter();
    return *this;
}

CowString& CowString::operator=(CowString&& other) {
    buf_ptr_ = other.buf_ptr_;
    other.buf_ptr_ = nullptr;
    return *this;
}

CowString& CowString::operator+=(const std::string_view other) {
    if (!other.empty() && buf_ptr_->UseCount() > 1) {
        ChangeBuffer();
    }
    buf_ptr_->AddString(other);
    return *this;
}

CowString& CowString::operator+=(const CowString& other) {
    if (other.GetData() != nullptr && buf_ptr_->UseCount() > 1) {
        ChangeBuffer();
    }
    buf_ptr_->AddString(other.GetData());
    return *this;
}

char* CowString::GetData() const {
    return buf_ptr_->GetData();
}

CowString::Buffer::Iterator CowString::begin() const {
    return CowString::Buffer::Iterator(*this, 0);
}

CowString::Buffer::Iterator CowString::end() const {
    return CowString::Buffer::Iterator(*this, buf_ptr_->Size());
}

char CowString::At(size_t index) const {
    return buf_ptr_->Get(index);
}

CowString::Buffer::Proxy CowString::operator[](size_t index) {
    return CowString::Buffer::Proxy(*this, index);
}

void CowString::ChangeBuffer() {
    Buffer* tmp = buf_ptr_;
    buf_ptr_ = new Buffer(*buf_ptr_);
    tmp->DecrementCounter();
}

bool CowString::operator==(const std::string_view str) const {
    return str.compare(GetData()) == 0;
}

bool CowString::operator!=(const std::string_view str) const {
    return str.compare(GetData()) != 0;
}

std::string CowString::operator+(const std::string& other) const {
    return GetData() + other;
}

CowString::operator const std::string_view() const {
    return GetData();
}

std::string CowString::operator+(const CowString& other) const {
    std::string result = GetData();
    return result + other.GetData();
}

bool operator==(const std::string_view str, const CowString& cow_str) {
    return str.compare(cow_str.GetData()) == 0;
}

bool operator!=(const std::string_view str, const CowString& cow_str) {
    return str.compare(cow_str.GetData()) != 0;
}

std::string operator+(const std::string& str, const CowString& cow_str) {
    return str + cow_str.GetData();
}