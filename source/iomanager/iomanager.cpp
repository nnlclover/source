#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <functional>
#include <SDL2/SDL.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class InputSettings {
public:
    // Конструктор по умолчанию
    InputSettings() = default;

    // Загрузка пользовательских настроек из файла JSON
    bool loadSettingsFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        json settings;
        file >> settings;

        if (!settings.is_object()) {
            std::cerr << "Invalid JSON format in file: " << filename << std::endl;
            return false;
        }

        // Парсим пользовательские настройки клавиш и функций
        for (const auto& entry : settings.items()) {
            const std::string& keyName = entry.key();
            const std::string& functionName = entry.value();

            // Находим функцию по имени в таблице функций
            auto functionIt = functionMap.find(functionName);
            if (functionIt == functionMap.end()) {
                std::cerr << "Function not found: " << functionName << std::endl;
                continue;
            }

            // Преобразуем имя клавиши в SDL_Keycode
            SDL_Keycode keyCode = SDL_GetKeyFromName(keyName.c_str());
            if (keyCode == SDLK_UNKNOWN) {
                std::cerr << "Invalid key name: " << keyName << std::endl;
                continue;
            }

            // Добавляем клавишу и функцию в таблицу пользовательских настроек
            userSettings[keyCode] = functionIt->second;
        }

        return true;
    }

    // Получение функции для указанной клавиши
    std::function<void()> getFunction(SDL_Keycode key) const {
        auto userSettingIt = userSettings.find(key);
        if (userSettingIt != userSettings.end()) {
            return userSettingIt->second;
        }

        // Если функция не найдена для указанной клавиши, возвращаем пустую функцию
        return std::function<void()>();
    }

private:
    // Тип функции для обработки клавиши
    typedef std::function<void()> KeyFunction;

    // Таблица функций для всех возможных клавиш
    std::unordered_map<std::string, KeyFunction> functionMap = {
        {"MoveForward", []() { std::cout << "Moving forward" << std::endl; }},
        {"MoveBackward", []() { std::cout << "Moving backward" << std::endl; }},
        // Здесь можно добавить другие функции и их имена
    };

    // Таблица пользовательских настроек клавиш и функций
    std::unordered_map<SDL_Keycode, KeyFunction> userSettings;
};

int main() {
    // Создаем экземпляр InputSettings и загружаем пользовательские настройки из файла
    InputSettings inputSettings;
    if (!inputSettings.loadSettingsFromFile("user_settings.json")) {
        std::cerr << "Failed to load user settings!" << std::endl;
        return 1;
    }

    // Пример использования InputSettings
    // Получение функции для клавиши "W"
    std::function<void()> wFunction = inputSettings.getFunction(SDLK_w);
    if (wFunction) {
        wFunction(); // Вызов функции для клавиши "W" (например, движение вперед)
    } else {
        std::cerr << "No function assigned to key W!" << std::endl;
    }

    return 0;
}
