структура проекта:
text
QuizApp/
├── src/
│   ├── services/
│   │   └── api.js
│   ├── components/
│   │   └── GameMap.js
│   └── assets/
├── App.js
└── package.json


Одновременная работа
Запустите C++ бекенд в Visual Studio 2022 (F5)

Запустите React Native в VS Code терминале

Тестируйте на эмуляторе или реальном устройстве

⚡ Быстрый старт - упрощенная версия
Если хотите начать быстрее, создайте простой HTML файл для тестирования бекенда:

test.html (положите рядом с .exe):

html
<!DOCTYPE html>
<html>
<body>
    <h1>Тест бекенда</h1>
    <button onclick="testAPI()">Тест /level/1</button>
    <div id="result"></div>
    
    <script>
        async function testAPI() {
            try {
                const response = await fetch('http://localhost:8080/level/1');
                const data = await response.json();
                document.getElementById('result').innerHTML = 
                    JSON.stringify(data, null, 2);
            } catch (error) {
                document.getElementById('result').innerHTML = 'Error: ' + error;
            }
        }
    </script>
</body>
</html>
