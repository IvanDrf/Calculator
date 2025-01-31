#include "Window.hpp"

#include "Calculator.hpp"

namespace {
const short kMoveX{5};
const short kMoveY{5};

const short kMoveMinusY{11};
}  // namespace

namespace {
bool CheckCorrectExpression(const std::string& string) {
    return string.find('/') == std::string::npos && string.find('x') == std::string::npos && string.find('+') == std::string::npos &&
           string.find('-') == std::string::npos && string.find('%') == std::string::npos;
}

bool CheckCorrectSign(const std::string& currentText, const std::string& buttonText) {
    std::string temp{currentText + buttonText};

    long minusCount{std::count(temp.begin(), temp.end(), '-')};
    long plusCount{std::count(temp.begin(), temp.end(), '+')};

    long precentCount{std::count(temp.begin(), temp.end(), '%')};

    long multiplyCount{std::count(temp.begin(), temp.end(), 'x')};
    long divisionCount{std::count(temp.begin(), temp.end(), '/')};

    long signCount{minusCount + plusCount + precentCount + multiplyCount + divisionCount};

    if (signCount > 3 || temp.find("-x") != std::string::npos || temp.find("+x") != std::string::npos || temp.find("-%") != std::string::npos ||
        temp.find("+%") != std::string::npos) {
        return false;
    }

    if (signCount == 3 && (temp.find("x-") != std::string::npos || temp.find("/-") != std::string::npos) && temp[0] == '-') {
        return true;
    }

    if (signCount == 2 && (minusCount == 2 || minusCount == 1 && plusCount == 1) && temp.find("-+") == std::string::npos && temp[0] != '0') {
        return true;
    }

    if (signCount == 2 && temp.find("-/") == std::string::npos && temp.find("+/") == std::string::npos && temp[0] != '0') {
        return true;
    }

    if (signCount == 1) {
        return true;
    }

    return false;
}
}  // namespace

namespace Window {
void SetIcon(sf::RenderWindow& window) {
    sf::Image icon;
    if (!icon.loadFromFile("CalculatorIcon.png")) {
        throw std::runtime_error("Icon could not be found");
    }

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}
}  // namespace Window

InputField::InputField() {
    shape_.setPosition(0, 0);
    shape_.setSize(sf::Vector2f(kInputFieldWidth, kInputFieldHeight));
    shape_.setFillColor(sf::Color(kInputFieldRed, kInputFieldGreen, kInputFieldBlue));

    if (!font_.loadFromFile("calibri.ttf")) {
        throw std::runtime_error("Font could not be found");
    }

    text_.setFont(font_);
    text_.setString("0");
    text_.setCharacterSize(3 * kCharacterSize);
    text_.setFillColor(sf::Color::White);

    sf::FloatRect textBound{text_.getGlobalBounds()};
    text_.setOrigin(textBound.width / 2, textBound.height / 2);

    text_.setPosition(kInputFieldWidth - 2 * kCharacterSize, kInputFieldHeight / 2 - textBound.height / 2);
}

void InputField::UpdateTextPosition() {
    std::string text{text_.getString()};
    sf::FloatRect textBound{text_.getGlobalBounds()};

    if (textBound.width > kInputFieldWidth) {
        text_.move(kTextMove, 0);
    } else {
        text_.setPosition(kInputFieldWidth - textBound.width - kCharacterSize, kInputFieldHeight / 2 - textBound.height / 2);
    }
}

void InputField::UpdateTextSize() {
    std::string expression{text_.getString()};
    if (expression.length() > kMaxTextLength) {
        text_.setCharacterSize(2 * kCharacterSize);

        return;
    }

    text_.setCharacterSize(3 * kCharacterSize);
}

void InputField::DrawInputField(sf::RenderWindow& window) const {
    window.draw(shape_);
    window.draw(text_);
}

void InputField::ExecuteCalculation(sf::RenderWindow& window, Button& button, size_t buttonNumber) {
    std::string buttonText{button.text_.getString()};
    std::string expression{text_.getString()};

    if (expression == "Invalid input") {
        expression = "0";
        text_.setString(expression);

        UpdateTextPosition();
        DrawInputField(window);
    }

    try {
        std::stold(buttonText);

        if (expression == "0") {
            text_.setString(buttonText);
        } else {
            text_.setString(expression + buttonText);
        }

    } catch (std::invalid_argument&) {
        if (buttonText == "," && expression[expression.length() - 1] >= '0' && expression[expression.length() - 1] <= '9') {
            text_.setString(expression + '.');
        }

        if (!CheckCorrectExpression(expression) && !CheckCorrectSign(expression, buttonText) && buttonText != "=" && buttonText != "cl") {
            return;
        }

        switch (static_cast<Buttons>(buttonNumber)) {
            case Buttons::Percent: {
                text_.setString(expression + '%');
                break;
            }

            case Buttons::Degree: {
                text_.setString(Numbers::CalculateDegree(expression));
                break;
            }
            case Buttons::Root: {
                text_.setString(Numbers::CalculateRoot(expression));
                break;
            }

            case Buttons::Division: {
                text_.setString(expression + "/");
                break;
            }
            case Buttons::Multiplication: {
                text_.setString(expression + "x");
                break;
            }

            case Buttons::Subtraction: {
                text_.setString(expression + "-");
                break;
            }
            case Buttons::Addition: {
                text_.setString(expression + "+");
                break;
            }

            case Buttons::Clear: {
                text_.setString("0");
                break;
            }

            case Buttons::Equal: {
                text_.setString(Numbers::CalculateExpression(expression));
                break;
            }

            default: {
                text_.setString("Invalid input");
                break;
            }
        }
    }

    UpdateTextSize();
    UpdateTextPosition();

    DrawInputField(window);
}

Button::Button() : clicking_(false) {
}

Button::Button(float x, float y, const std::string& value, const sf::Color& color) : clicking_(false), oldColor_(color) {
    shape_.setPosition(x, y);
    shape_.setSize(sf::Vector2f(kButtonWidth, kButtonHeight));
    shape_.setFillColor(sf::Color(color));

    if (!font_.loadFromFile("calibri.ttf")) {
        throw std::runtime_error("Font could not be found");
    }

    text_.setFont(font_);
    text_.setString(value);
    text_.setCharacterSize(kCharacterSize);
    text_.setFillColor(sf::Color::White);

    sf::FloatRect textBound{text_.getGlobalBounds()};
    text_.setOrigin(textBound.width / 2, textBound.height / 2 + kCharacterSize / 4 + 2);

    if (value == "+" || value == "=" || value == "x") {
        text_.setPosition(x + kButtonWidth / 2 - kMoveX, y + kButtonHeight / 2 - kMoveY);
        return;
    }

    if (value == "-") {
        text_.setPosition(x + kButtonWidth / 2 - kMoveX, y + kButtonHeight / 2 - kMoveMinusY);
        return;
    }

    text_.setPosition(x + kButtonWidth / 2, y + kButtonHeight / 2);
}

void Button::CreateButtons(Button* buttons) {
    const sf::Color numberColor{kNumberRed, kNumberGreen, kNumberBlue};

    static Button precentButton(0, kInputFieldHeight, "%");
    static Button degreeButton(kButtonWidth + kButtonDistance, kInputFieldHeight, "x^2");
    static Button rootButton(2 * (kButtonWidth + kButtonDistance), kInputFieldHeight, "sqrt(x)");
    static Button divisionButton(3 * (kButtonWidth + kButtonDistance), kInputFieldHeight, "/");

    size_t i{};
    buttons[i++] = precentButton;
    buttons[i++] = degreeButton;
    buttons[i++] = rootButton;
    buttons[i++] = divisionButton;

    static Button number7Button(0, kInputFieldHeight + kButtonHeight + kButtonDistance, "7", numberColor);
    static Button number8Button(kButtonWidth + kButtonDistance, kInputFieldHeight + kButtonHeight + kButtonDistance, "8", numberColor);
    static Button number9Button(2 * (kButtonWidth + kButtonDistance), kInputFieldHeight + kButtonHeight + kButtonDistance, "9", numberColor);
    static Button multiplicationButton(3 * (kButtonWidth + kButtonDistance), kInputFieldHeight + kButtonHeight + kButtonDistance, "x");

    buttons[i++] = number7Button;
    buttons[i++] = number8Button;
    buttons[i++] = number9Button;
    buttons[i++] = multiplicationButton;

    static Button number4Button(0, kInputFieldHeight + 2 * (kButtonHeight + kButtonDistance), "4", numberColor);
    static Button number5Button(kButtonWidth + kButtonDistance, kInputFieldHeight + 2 * (kButtonHeight + kButtonDistance), "5", numberColor);
    static Button number6Button(2 * (kButtonWidth + kButtonDistance), kInputFieldHeight + 2 * (kButtonHeight + kButtonDistance), "6", numberColor);
    static Button subtractionButton(3 * (kButtonWidth + kButtonDistance), kInputFieldHeight + 2 * (kButtonHeight + kButtonDistance), "-");

    buttons[i++] = number4Button;
    buttons[i++] = number5Button;
    buttons[i++] = number6Button;
    buttons[i++] = subtractionButton;

    static Button number1Button(0, kInputFieldHeight + 3 * (kButtonHeight + kButtonDistance), "1", numberColor);
    static Button number2Button(kButtonWidth + kButtonDistance, kInputFieldHeight + 3 * (kButtonHeight + kButtonDistance), "2", numberColor);
    static Button number3Button(2 * (kButtonWidth + kButtonDistance), kInputFieldHeight + 3 * (kButtonHeight + kButtonDistance), "3", numberColor);
    static Button additionButton(3 * (kButtonWidth + kButtonDistance), kInputFieldHeight + 3 * (kButtonHeight + kButtonDistance), "+");

    buttons[i++] = number1Button;
    buttons[i++] = number2Button;
    buttons[i++] = number3Button;
    buttons[i++] = additionButton;

    static Button clearButton(0, kInputFieldHeight + 4 * (kButtonHeight + kButtonDistance), "cl", numberColor);
    static Button number0Button(kButtonWidth + kButtonDistance, kInputFieldHeight + 4 * (kButtonHeight + kButtonDistance), "0", numberColor);
    static Button comaButton(2 * (kButtonWidth + kButtonDistance), kInputFieldHeight + 4 * (kButtonHeight + kButtonDistance), ",", numberColor);
    static Button equalButton(3 * (kButtonWidth + kButtonDistance), kInputFieldHeight + 4 * (kButtonHeight + kButtonDistance), "=",
                              {kEqualRed, kEqualGreen, kEqualBlue});

    buttons[i++] = clearButton;
    buttons[i++] = number0Button;
    buttons[i++] = comaButton;
    buttons[i++] = equalButton;
}

void Button::DrawButton(sf::RenderWindow& window) const {
    window.draw(shape_);
    window.draw(text_);
}

void Button::DrawButtons(sf::RenderWindow& window, const Button* buttons) {
    for (size_t i = 0; i < kButtonAmount; ++i) {
        buttons[i].DrawButton(window);
    }
}

bool Button::PressButton(const sf::Vector2f& mousePosition) const {
    return shape_.getGlobalBounds().contains(mousePosition);
}

void Button::ChangeColor() {
    if (clicking_) {
        return;
    }

    clicking_ = true;
    clickTimer_.restart();

    sf::Color oldColor{shape_.getFillColor()};
    shape_.setFillColor(sf::Color(kActiveRed, kActiveGreen, kActiveBlue));
}

void Button::ReturnColor() {
    if (clicking_ && clickTimer_.getElapsedTime().asSeconds() > kActiveTime) {
        shape_.setFillColor(oldColor_);
        clicking_ = false;
    }
}
