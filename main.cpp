#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

class Domain {
public:

    Domain(const std::string& str)
        : domain_(str) {
    }

    bool operator==(const Domain& other) const {
        return std::lexicographical_compare(domain_.begin(),
                                            domain_.end(),
                                            other.domain_.begin(),
                                            other.domain_.end());
    }
    bool IsSubdomain(const Domain& other) const {
        if (!(other.domain_.size() > domain_.size())) {
            return (std::equal(other.domain_.begin(), other.domain_.end(), domain_.begin()));
        }
        return false;
    }
    std::string GetDomain() const { 
        return domain_; 
    }

private:
    std::string domain_;
};

class DomainChecker {
public:

    template<typename Iterator>
    DomainChecker(const Iterator begin, const Iterator end)
        : forbidden_(begin, end)
    {
        UniqueSort();
    }

    bool IsForbidden(const Domain& domain) {
        if (forbidden_.size()) {
            std::vector<Domain>::iterator domain_pos = std::upper_bound(forbidden_.begin(),
                                                                            forbidden_.end(),
                                                                            domain,
                                                                            [](const Domain& lhs, const Domain& rhs) {
                                                                                return lhs.GetDomain() < rhs.GetDomain();
                                                                            });
            if (forbidden_.begin() == domain_pos) {
                return domain.IsSubdomain(forbidden_[domain_pos - forbidden_.begin()]);

            }
            return domain.IsSubdomain(forbidden_[domain_pos - forbidden_.begin() - 1]);
        }
        return false;
    }

private:
    void UniqueSort() {
        std::sort(forbidden_.begin(), forbidden_.end(), [](const Domain& lhs, const Domain& rhs) {
            return lhs.GetDomain() < rhs.GetDomain();
            });
        auto last_unique = unique(forbidden_.begin(), forbidden_.end(), [](const Domain& lhs, const Domain& rhs) {
            return rhs.IsSubdomain(lhs);
            });
        forbidden_.erase(last_unique, forbidden_.end());
    }

    std::vector<Domain> forbidden_;
};

const std::vector<Domain> ReadDomains(std::istream& input, size_t number) {
    std::vector<Domain> result;
    for (size_t i = 0; i < number; ++i) {
        std::string line;
        getline(input, line);
        line.insert(0, 1, '.');
        result.emplace_back(move(std::string({line.rbegin(), line.rend()})));
    }
    return result;
}

template <typename Number>
Number ReadNumberOnLine(std::istream& input) {
    std::string line;
    getline(input, line);
    Number number;
    std::istringstream(line) >> number;
    return number;
}


int main() {
    using namespace std::literals;
    const std::vector<Domain> forbidden_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
    for (const Domain& domain : domains) {
        std::cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << std::endl;
    }
}