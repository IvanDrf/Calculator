#include "Window.hpp"

int main(int, char**) {
    sf::RenderWindow window(sf::VideoMode(kWindowWidth, kWindowHeight), "Calculator");
    Window::SetIcon(window);

    Button buttons[kButtonAmount];
    Button::CreateButtons(buttons);

    InputField inputField;

    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.clear();
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePosition{sf::Mouse::getPosition(window)};

                    for (size_t i = 0; i < kButtonAmount; ++i) {
                        if (buttons[i].PressButton(mousePosition)) {
                            inputField.ExecuteCalculation(window, buttons[i], i);

                            buttons[i].ChangeColor();
                        }
                    }
                }
            }
        }

        for (auto& button : buttons) {
            button.ReturnColor();
        }

        window.clear();

        inputField.DrawInputField(window);
        Button::DrawButtons(window, buttons);

        window.display();
    }

    return 0;
}
