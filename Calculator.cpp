#include "Calculator.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace {
const short kPrecision{5};
const double kPrecent{100};
const double kDegree{0.5};
}  // namespace

namespace {
std::string RoundResult(const long double& number) {
    std::ostringstream value;
    value << std::fixed << std::setprecision(kPrecision) << number;

    std::string result = value.str();

    if (result.find('.') != std::string::npos) {
        result.erase(result.find_last_not_of('0') + 1, std::string::npos);
        if (result[result.length() - 1] == '.') {
            result.pop_back();
        }
    }

    return result;
}
}  // namespace

Numbers::Numbers() : numberA_(0), numberB_(0) {
}

Numbers::Numbers(const std::string& expression, const char delimiter) {
    size_t delimiterPosition{expression.find(delimiter)};

    try {
        numberA_ = std::stold(expression.substr(0, delimiterPosition));
        numberB_ = std::stold(expression.substr(delimiterPosition + 1, expression.length() - delimiterPosition - 1));
    } catch (std::invalid_argument&) {
        numberA_ = std::stold(expression.substr(0, delimiterPosition));
        numberB_ = 0;
    }
}

Numbers::Numbers(const std::string& expression) {
    numberA_ = std::stold(expression.substr(0, expression.rfind('-')));
    numberB_ = std::stold(expression.substr(expression.rfind('-')));
}

Numbers::~Numbers() {
    numberA_ = 0;
    numberB_ = 0;
}

std::string Numbers::CalculateExpression(const std::string& expression) {
    if (expression.find('%') != std::string::npos) {
        return Numbers::CalculatePrecent(expression);
    }

    if (expression.find('/') != std::string::npos) {
        return Numbers::CalculateDivision(expression);
    }

    if (expression.find('x') != std::string::npos) {
        return Numbers::CalculateMultiplication(expression);
    }

    if (expression.find('+') != std::string::npos) {
        return Numbers::CalculateAddition(expression);
    }

    if (expression.find('-') != std::string::npos) {
        return Numbers::CalculateSubtraction(expression);
    }

    return expression;
}

std::string Numbers::CalculatePrecent(const std::string& expression) {
    Numbers numbers(expression, '%');
    return RoundResult(numbers.numberA_ / kPrecent * numbers.numberB_);
}

std::string Numbers::CalculateDegree(const std::string& value) {
    long double number{std::stold(value)};
    return RoundResult(number * number);
}

std::string Numbers::CalculateRoot(const std::string& value) {
    long double number{std::stold(value)};
    if (number < 0) {
        return "Invalid input";
    }

    return RoundResult(std::pow(number, kDegree));
}

std::string Numbers::CalculateDivision(const std::string& expression) {
    Numbers numbers(expression, '/');
    if (numbers.numberB_ == 0) {
        return "Invalid input";
    }

    return RoundResult(numbers.numberA_ / numbers.numberB_);
}

std::string Numbers::CalculateMultiplication(const std::string& expression) {
    Numbers numbers(expression, 'x');
    return RoundResult(numbers.numberA_ * numbers.numberB_);
}

std::string Numbers::CalculateSubtraction(const std::string& expression) {
    if (expression.rfind('-') == 0) {
        return expression;
    }

    if (std::count(expression.begin(), expression.end(), '-') == 2) {
        Numbers numbers(expression);
        return RoundResult(numbers.numberA_ + numbers.numberB_);
    }

    Numbers numbers(expression, '-');
    return RoundResult(numbers.numberA_ - numbers.numberB_);
}

std::string Numbers::CalculateAddition(const std::string& expression) {
    Numbers numbers(expression, '+');
    return RoundResult(numbers.numberA_ + numbers.numberB_);
}
