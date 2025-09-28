#include <iostream>
#include <vector>
#include <string>
#include "httplib.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace httplib;

struct Question {
    std::string text;
    std::vector<std::string> options;
    int correct;
    std::string explanation;
};

// 20 вопросов для 20 уровней
std::vector<Question> levels = {
    {"Сколько будет 2+2?", {"3", "4", "5"}, 1, "Правильный ответ: 4"},
    {"Столица Франции?", {"Берлин", "Париж", "Рим"}, 1, "Париж - столица Франции"},
    {"Цвет неба?", {"Синий", "Зелёный", "Красный"}, 0, "Небо обычно синего цвета"},
    {"Самая большая планета Солнечной системы?", {"Земля", "Юпитер", "Марс"}, 1, "Юпитер - самая большая планета"},
    {"Сколько континентов на Земле?", {"5", "6", "7"}, 2, "На Земле 7 континентов"},
    {"Автор 'Войны и мира'?", {"Достоевский", "Толстой", "Чехов"}, 1, "Лев Толстой написал 'Войну и мир'"},
    {"Химическая формула воды?", {"CO2", "H2O", "O2"}, 1, "H2O - формула воды"},
    {"Сколько дней в високосном году?", {"365", "366", "367"}, 1, "В високосном году 366 дней"},
    {"Самое глубокое озеро в мире?", {"Байкал", "Верхнее", "Виктория"}, 0, "Озеро Байкал - самое глубокое"},
    {"Столица Японии?", {"Пекин", "Сеул", "Токио"}, 2, "Токио - столица Японии"},
    {"Сколько цветов у радуги?", {"5", "6", "7"}, 2, "В радуге 7 цветов"},
    {"Единица измерения силы тока?", {"Вольт", "Ампер", "Ватт"}, 1, "Ампер - единица силы тока"},
    {"Автор теории относительности?", {"Ньютон", "Эйнштейн", "Тесла"}, 1, "Альберт Эйнштейн"},
    {"Сколько костей в теле взрослого человека?", {"206", "300", "150"}, 0, "206 костей у взрослого человека"},
    {"Самая длинная река в мире?", {"Амазонка", "Нил", "Янцзы"}, 0, "Амазонка - самая длинная река"},
    {"Столица Австралии?", {"Сидней", "Мельбурн", "Канберра"}, 2, "Канберра - столица Австралии"},
    {"Сколько часов в сутках?", {"12", "24", "36"}, 1, "В сутках 24 часа"},
    {"Холоднее: -5°C или -10°C?", {"-5°C", "-10°C", "Одинаково"}, 1, "-10°C холоднее чем -5°C"},
    {"Сколько сторон у шестиугольника?", {"5", "6", "7"}, 1, "У шестиугольника 6 сторон"},
    {"Самый большой океан?", {"Атлантический", "Индийский", "Тихий"}, 2, "Тихий океан - самый большой"}
};

// Добавляем CORS headers для React Native
void set_cors_headers(Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

int main() {
    Server svr;

    // Обработка CORS preflight
    svr.Options(R"(/level/(\d+))", [](const Request& req, Response& res) {
        set_cors_headers(res);
    });

    svr.Options("/answer", [](const Request& req, Response& res) {
        set_cors_headers(res);
    });

    // Получить вопрос по уровню (для React Native)
    svr.Get(R"(/level/(\d+))", [](const Request& req, Response& res) {
        set_cors_headers(res);
        
        int lvl = std::stoi(req.matches[1]);
        if (lvl < 1 || lvl > levels.size()) {
            res.status = 404;
            json error = {{"error", "Level not found"}};
            res.set_content(error.dump(), "application/json");
            return;
        }

        auto q = levels[lvl - 1];
        json j;
        j["level"] = lvl;
        j["question"] = q.text;
        j["options"] = q.options;
        j["explanation"] = q.explanation;
        res.set_content(j.dump(), "application/json");
    });

    // Проверить ответ (для React Native)
    svr.Post("/answer", [](const Request& req, Response& res) {
        set_cors_headers(res);
        
        try {
            auto body = json::parse(req.body);
            int lvl = body["level"];
            int answer = body["answer"];

            if (lvl < 1 || lvl > levels.size()) {
                res.status = 404;
                json error = {{"error", "Level not found"}};
                res.set_content(error.dump(), "application/json");
                return;
            }

            auto& question = levels[lvl - 1];
            bool correct = (question.correct == answer);
            
            json j;
            j["level"] = lvl;
            j["correct"] = correct;
            j["correctIndex"] = question.correct;
            j["explanation"] = question.explanation;
            res.set_content(j.dump(), "application/json");
            
        } catch (const std::exception& e) {
            res.status = 400;
            json error = {{"error", "Invalid request format"}};
            res.set_content(error.dump(), "application/json");
        }
    });

    // Получить информацию о всех уровнях (для карты)
    svr.Get("/levels", [](const Request& req, Response& res) {
        set_cors_headers(res);
        
        json j;
        j["totalLevels"] = levels.size();
        j["levels"] = json::array();
        
        for (int i = 0; i < levels.size(); i++) {
            json levelInfo;
            levelInfo["level"] = i + 1;
            levelInfo["question"] = levels[i].text;
            j["levels"].push_back(levelInfo);
        }
        
        res.set_content(j.dump(), "application/json");
    });

    // Health check
    svr.Get("/health", [](const Request& req, Response& res) {
        set_cors_headers(res);
        json j = {{"status", "ok"}, {"totalLevels", levels.size()}};
        res.set_content(j.dump(), "application/json");
    });

    std::cout << "🚀 C++ Backend Server started at http://localhost:8080\n";
    std::cout << "📱 Ready for React Native app\n";
    std::cout << "📊 Total levels: " << levels.size() << "\n";
    std::cout << "🔗 Endpoints:\n";
    std::cout << "   GET  /level/{id}     - Get question for level\n";
    std::cout << "   POST /answer         - Check answer\n";
    std::cout << "   GET  /levels         - Get all levels info\n";
    std::cout << "   GET  /health         - Health check\n";
    
    svr.listen("0.0.0.0", 8080);
}