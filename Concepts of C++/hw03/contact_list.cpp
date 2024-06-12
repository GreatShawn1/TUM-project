#include "contact_list.h"

#include <algorithm>
#include <numeric>
#include <sstream>

// TODO create implementation here!

namespace contact_list {

    bool add(storage& contacts, std::string_view name, number_t number) {
        if (name.empty()) return false;

        auto it = std::find(contacts.names.begin(), contacts.names.end(), name);
        if (it != contacts.names.end()) return false; // Disallow duplicate names

        contacts.names.push_back(std::string(name));
        contacts.numbers.push_back(number);
        return true;
    }

    size_t size(const storage& contacts) {
        return contacts.names.size();
    }

    number_t get_number_by_name(storage& contacts, std::string_view name) {
        auto it = std::find(contacts.names.begin(), contacts.names.end(), name);
        if (it != contacts.names.end()) {
            size_t index = std::distance(contacts.names.begin(), it);
            return contacts.numbers[index];
        }
        return -1; // Name not found
    }

    std::string to_string(const storage& contacts) {
        std::ostringstream oss;
        for (size_t i = 0; i < contacts.names.size(); ++i) {
            oss << contacts.names[i] << " - " << contacts.numbers[i] << "\n";
        }
        return oss.str();
    }

    bool remove(storage& contacts, std::string_view name) {
        auto it = std::find(contacts.names.begin(), contacts.names.end(), name);
        if (it != contacts.names.end()) {
            size_t index = std::distance(contacts.names.begin(), it);
            contacts.names.erase(contacts.names.begin() + index);
            contacts.numbers.erase(contacts.numbers.begin() + index);
            return true;
        }
        return false; // Name not found
    }

    void sort(storage& contacts) {
        std::vector<std::pair<std::string, number_t>> temp;
        for (size_t i = 0; i < contacts.names.size(); ++i) {
            temp.emplace_back(contacts.names[i], contacts.numbers[i]);
        }

        std::sort(temp.begin(), temp.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        for (size_t i = 0; i < temp.size(); ++i) {
            contacts.names[i] = temp[i].first;
            contacts.numbers[i] = temp[i].second;
        }
    }

    std::string get_name_by_number(storage& contacts, number_t number) {
        auto it = std::find(contacts.numbers.begin(), contacts.numbers.end(), number);
        if (it != contacts.numbers.end()) {
            size_t index = std::distance(contacts.numbers.begin(), it);
            return contacts.names[index];
        }
        return ""; // Number not found
    }
} // namespace contact_list
