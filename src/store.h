#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <mutex>

class Store
{
public:
    // Insert or overwrite a key's value.
    void set(const std::string &key, const std::string &value);

    // Look up a key. Returns the value if found, empty optional otherwise.
    std::optional<std::string> get(const std::string &key) const;

    bool del(const std::string &key);

private:
    std::unordered_map<std::string, std::string> data_;
    mutable std::mutex mutex_;
};