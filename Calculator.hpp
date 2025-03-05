#pragma once

#include <string>

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class Numbers {
 private:
    long double numberA_;
    long double numberB_;

 public:
    Numbers();
    Numbers(const std::string& expression, const char delimiter);
    explicit Numbers(const std::string& expression);

    ~Numbers();

    static std::string CalculateExpression(const std::string& expression);

    static std::string CalculatePrecent(const std::string& expression);

    static std::string CalculateDegree(const std::string& value);
    static std::string CalculateRoot(const std::string& value);

    static std::string CalculateDivision(const std::string& expression);
    static std::string CalculateMultiplication(const std::string& expression);

    static std::string CalculateSubtraction(const std::string& expression);
    static std::string CalculateAddition(const std::string& expression);
};
