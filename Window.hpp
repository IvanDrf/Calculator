#pragma once

#include <SFML/Graphics.hpp>

class Button;
class InputField;

namespace {
const uint kDesktopWidth{sf::VideoMode::getDesktopMode().width};
const uint kDesktopHeight{sf::VideoMode::getDesktopMode().height};

const uint kWindowWidth{static_cast<uint>(kDesktopWidth * 0.35)};
const uint kWindowHeight{static_cast<uint>(kDesktopWidth * 0.4)};

const float kInputFieldWidth{static_cast<float>(kWindowWidth)};
const float kInputFieldHeight{static_cast<float>(kWindowHeight * 0.36)};

const float kButtonWidth{static_cast<float>(kWindowWidth * 0.25)};
const float kButtonHeight{static_cast<float>(kButtonWidth * 0.55)};
const float kButtonDistance{5};
const size_t kButtonAmount{20};

const short kDefaultRed{50};
const short kDefaultGreen{50};
const short kDefaultBlue{50};

const double kActiveTime{0.25};

const short kActiveRed{100};
const short kActiveGreen{100};
const short kActiveBlue{100};

const short kInputFieldRed{32};
const short kInputFieldGreen{32};
const short kInputFieldBlue{32};

const short kNumberRed{59};
const short kNumberGreen{59};
const short kNumberBlue{59};

const short kEqualRed{76};
const short kEqualGreen{100};
const short kEqualBlue{255};

const short kCharacterSize{40};
const short kTextMove{-5};
const short kMaxTextLength{7};
}  // namespace

namespace Window {
void SetIcon(sf::RenderWindow& window);
}  // namespace Window

class InputField {
 private:
    sf::Text text_;
    sf::Font font_;
    sf::RectangleShape shape_;

 public:
    InputField();

    void UpdateTextPosition();
    void UpdateTextSize();

    void DrawInputField(sf::RenderWindow& window) const;
    void ExecuteCalculation(sf::RenderWindow& window, Button& button, [[maybe_unused]] size_t buttonNumber);
};

enum class Buttons {
    Percent = 0,
    Degree = 1,
    Root = 2,

    Division = 3,
    Multiplication = 7,

    Subtraction = 11,
    Addition = 15,

    Clear = 16,
    Equal = 19
};

class Button {
 private:
    sf::Text text_;
    sf::Font font_;

    sf::RectangleShape shape_;
    sf::Color oldColor_;

    sf::Clock clickTimer_;
    bool clicking_;

 public:
    Button();
    Button(float x, float y, const std::string& value, const sf::Color& color = {kDefaultRed, kDefaultGreen, kDefaultBlue});

    void static CreateButtons(Button* buttons);

    void DrawButton(sf::RenderWindow& window) const;
    void static DrawButtons(sf::RenderWindow& window, const Button* buttons);

    bool PressButton(const sf::Vector2f& mousePosition) const;
    void ChangeColor();
    void ReturnColor();

    friend void InputField::ExecuteCalculation(sf::RenderWindow& window, Button& button, [[maybe_unused]] size_t buttonNumber);
};
