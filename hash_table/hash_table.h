#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <vector>

template <typename K, typename V, typename Hash = std::hash<K>, typename KeyEqual = std::equal_to<K>>
class HashTable {
public:
    using KVP = std::pair<K, V>;
    using Bucket = std::list<KVP>;

    class Iterator {
    public:
        friend class HashTable;
        explicit Iterator(HashTable& parent, std::vector<size_t>::iterator filled_bucket_it,
                          typename Bucket::iterator item_in_bucket_it)
            : parent_(parent), filled_bucket_it_(filled_bucket_it), item_in_bucket_it_(item_in_bucket_it) {
        }

        Iterator& operator++() {
            if (item_in_bucket_it_ == parent_.buckets_[*filled_bucket_it_].end() ||
                ++item_in_bucket_it_ == parent_.buckets_[*filled_bucket_it_].end()) {

                ++filled_bucket_it_;
                if (filled_bucket_it_ != parent_.filled_buckets_.end()) {
                    item_in_bucket_it_ = parent_.buckets_[*filled_bucket_it_].begin();
                }
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            if (this->filled_bucket_it_ == other.filled_bucket_it_ &&
                this->filled_bucket_it_ == parent_.filled_buckets_.end()) {
                return false;
            }

            return &this->parent_ != &other.parent_ || this->filled_bucket_it_ != other.filled_bucket_it_ ||
                   this->item_in_bucket_it_ != other.item_in_bucket_it_;
        }

        KVP& operator*() {
            return *item_in_bucket_it_;
        }

        KVP* operator->() {
            return &*item_in_bucket_it_;
        }

    private:
        HashTable& parent_;
        std::vector<size_t>::iterator filled_bucket_it_;
        typename Bucket::iterator item_in_bucket_it_;
    };
    
    class ConstIterator {
    public:
        friend class HashTable;
        explicit ConstIterator(const HashTable& parent, std::vector<size_t>::const_iterator filled_bucket_it,
                          typename Bucket::const_iterator item_in_bucket_it)
            : parent_(parent), filled_bucket_it_(filled_bucket_it), item_in_bucket_it_(item_in_bucket_it) {
        }

        ConstIterator& operator++() {
            if (item_in_bucket_it_ == parent_.buckets_[*filled_bucket_it_].end() ||
                ++item_in_bucket_it_ == parent_.buckets_[*filled_bucket_it_].end()) {

                ++filled_bucket_it_;
                if (filled_bucket_it_ != parent_.filled_buckets_.end()) {
                    item_in_bucket_it_ = parent_.buckets_[*filled_bucket_it_].begin();
                }
            }
            return *this;
        }

        bool operator!=(const ConstIterator& other) const {
            if (this->filled_bucket_it_ == other.filled_bucket_it_ &&
                this->filled_bucket_it_ == parent_.filled_buckets_.end()) {
                return false;
            }

            return &this->parent_ != &other.parent_ || this->filled_bucket_it_ != other.filled_bucket_it_ ||
                   this->item_in_bucket_it_ != other.item_in_bucket_it_;
        }

        const KVP& operator*() {
            return *item_in_bucket_it_;
        }

        const KVP* operator->() {
            return &*item_in_bucket_it_;
        }

    private:
        const HashTable& parent_;
        std::vector<size_t>::const_iterator filled_bucket_it_;
        typename Bucket::const_iterator item_in_bucket_it_;
    };

    HashTable() = default;

    HashTable(const HashTable& other)
        : buckets_count_(other.buckets_count_),
          elements_count_(other.elements_count_),
          filled_buckets_(other.filled_buckets_) {
        delete[] buckets_;
        delete[] filled_buckets_index_;
        this->buckets_ = new Bucket[other.buckets_count_];
        this->filled_buckets_index_ = new size_t[other.filled_buckets_.size()];
        std::copy(other.buckets_, other.buckets_ + other.buckets_count_, this->buckets_);
        std::copy(other.filled_buckets_index_, other.filled_buckets_index_ + other.filled_buckets_.size(), this->filled_buckets_index_);
    }

    HashTable(const HashTable&& other)
        : buckets_count_(other.buckets_count_),
          elements_count_(other.elements_count_),
          filled_buckets_(std::move(other.filled_buckets_)) {
        delete[] buckets_;
        delete[] filled_buckets_index_;
        this->buckets_ = other.buckets_;
        this->filled_buckets_index_ = other.filled_buckets_index_;
        other.buckets_ = nullptr;
        other.filled_buckets_index_ = nullptr;
    }

    template <typename C, typename T = decltype(*std::begin(std::declval<C>())),
              typename Check = typename std::enable_if<std::is_convertible<KVP, T>::value>::type>
    HashTable(const C& container) {
        for (const auto& item : container) {
            insert(item);
        }
    }

    ~HashTable() {
        delete[] buckets_;
        delete[] filled_buckets_index_;
    }

    HashTable& operator=(const HashTable& other) {
        buckets_count_ = other.buckets_count_;
        elements_count_ = other.elements_count_;
        filled_buckets_ = other.filled_buckets_;
        delete[] this->buckets_;
        delete[] this->filled_buckets_index_;
        this->buckets_ = new Bucket[other.buckets_count_];
        this->filled_buckets_index_ = new size_t[other.filled_buckets_.size()];
        std::copy(other.buckets_, other.buckets_ + other.buckets_count_, this->buckets_);
        std::copy(other.filled_buckets_index_, other.filled_buckets_index_ + other.filled_buckets_.size(), this->filled_buckets_index_);
    }

    HashTable& operator=(const HashTable&& other) {
        buckets_count_ = other.buckets_count_;
        elements_count_ = other.elements_count_;
        filled_buckets_ = std::move(other.filled_buckets_);
        delete[] this->buckets_;
        delete[] this->filled_buckets_index_;
        this->buckets_ = other.buckets_;
        this->filled_bucket_index_ = other.filled_buckets_index_;
        other.buckets_ = nullptr;
        other.filled_buckets_index_ = nullptr;
    }

    Iterator begin() {
        if (!elements_count_) {
            return end();
        }
        return Iterator(*this, filled_buckets_.begin(), buckets_[filled_buckets_.front()].begin());
    }

    Iterator end() {
        return Iterator(*this, filled_buckets_.end(), buckets_[0].begin());
    }

    ConstIterator begin() const {
        if (!elements_count_) {
            return end();
        }
        return ConstIterator(*this, filled_buckets_.begin(), buckets_[filled_buckets_.front()].begin());
    }

    ConstIterator end() const {
        return ConstIterator(*this, filled_buckets_.end(), buckets_[0].begin());
    }

    V& operator[](const K& key) {
        Iterator it = find(key);
        if (it != end()) {
            return it->second;
        }
        InsertIntoBucket({key, V{}});
        return find(key)->second;
    };

    V& at(const K& key) {
        Iterator it = find(key);
        if (it != end()) {
            return it->second;
        }
        throw "No such item";
    };

    Iterator find(const K& key) {
        size_t index = GetIndex(key);
        for (auto it = buckets_[index].begin(); it != buckets_[index].end(); ++it) {
            if (KeyEqual()(key, it->first)) {
                return Iterator(*this, filled_buckets_.begin() + filled_buckets_index_[index], it);
            }
        }
        return end();
    }

    bool empty() const {
        return elements_count_ == 0;
    }

    size_t size() const {
        return elements_count_;
    }

    std::pair<Iterator, bool> insert(const KVP& item) {
        auto it1 = find(item.first);
        if (it1 != end()) {
            return {it1, false};
        }
        auto it2 = InsertIntoBucket(item);
        return {it2, true};
    }

    std::pair<Iterator, bool> emplace(const K& key, const V& value) {
        auto it1 = find(key);
        if (it1 != end()) {
            return {it1, false};
        }
        auto it2 = InsertIntoBucket({key, value});
        return {it2, true};
    }

    void clear() {
        buckets_count_ = 1;
        elements_count_ = 0;
        delete[] buckets_;
        delete[] filled_buckets_index_;
        buckets_ = new Bucket[1];
        filled_buckets_index_ = new size_t[1];
        filled_buckets_.clear();
    }

    double Density() const {
        return static_cast<double>(elements_count_) / buckets_count_;
    }

private:
    inline size_t GetIndex(const K& key) const {
        return hasher_(key) % buckets_count_;
    }

    Iterator InsertIntoBucket(const KVP& item) {
        elements_count_++;
        if (Density() > 0.75) {
            Reallocate();
            elements_count_++;
        }
        size_t index = GetIndex(item.first);
        if (buckets_[index].empty()) {
            filled_buckets_index_[index] = filled_buckets_.size();
            filled_buckets_.push_back(index);
        }
        buckets_[index].push_back(item);
        return Iterator(*this, --filled_buckets_.end(), --buckets_[index].end());
    }

    void Reallocate() {
        size_t old_buckets_count = buckets_count_;
        buckets_count_ <<= 1;
        elements_count_ = 0;
        filled_buckets_.clear();
        delete[] filled_buckets_index_;
        filled_buckets_index_ = new size_t[buckets_count_];
        Bucket* new_buckets = new Bucket[buckets_count_];
        for (Bucket* i = buckets_; i < buckets_ + old_buckets_count; ++i) {
            for (const auto& el : *i) {
                elements_count_++;
                size_t index = GetIndex(el.first);
                if (new_buckets[index].empty()) {
                    filled_buckets_index_[index] = filled_buckets_.size();
                    filled_buckets_.push_back(index);
                }
                new_buckets[index].push_back(el);
            }
        }
        delete[] buckets_;
        buckets_ = new_buckets;
    }

    Hash hasher_;
    size_t buckets_count_ = 1;
    size_t elements_count_ = 0;
    Bucket* buckets_ = new Bucket[1];
    size_t* filled_buckets_index_ = new size_t[1];
    std::vector<size_t> filled_buckets_;
};
