assets.csv – исходная таблица (неатомарная)

main.cpp – код для преобразования таблицы из assets.csv в запросы для вставки данных в БД.
Запросы записываються в файл InsertStudents.txt (после преобразуется в InsertStudents.sql).

Students.sql – хранит запросы создания таблицы students и селекции атомарной таблицы.

result.csv(.xlsx) – получившаяся атомарная таблица.


=================================

Функциональность программы на C++

Ввод и вывод

Входной файл: Программа считывает данные из CSV-файла с именем assets.csv.
Выходной файл: SQL-запросы на вставку записываются в файл InsertStudents.txt.

    std::ifstream inputFile("../assets.csv");
    std::ofstream outputFile("../InsertStudents.txt");

Разбор кода

Работа с файлами:
Программа пытается открыть входной и выходной файлы. Если не удается открыть хотя бы один из них, выводится сообщение об ошибке, и программа завершается.

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Failed to open the input files." << std::endl;
        return 1;
    }

Чтение заголовка:
Первая строка CSV-файла считывается для извлечения названий предметов, используется разделитель ;.
Программа игнорирует первые два столбца: "Группа" и "ФИО", сохраняя остальные названия предметов в векторе.

    std::string line;
    std::vector<std::string> subjects;

    if (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string subject;
        while (std::getline(ss, subject, ';')) {
            if (subject != "Группа" && subject != "ФИО") {
                subjects.push_back(subject);
            }
        }
    }

Обработка данных студентов:
Каждая последующая строка считывается, извлекая группу и имя.
Для каждого предмета собирается соответствующая оценка, которая форматируется в SQL-запрос на вставку и записывается в выходной файл.
Запрос использует одинарные кавычки для строковых значений в соответствии с SQL-стандартами.

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string group, name;

        std::getline(ss, group, ';');
        std::getline(ss, name, ';');

        // Process grades
        for (size_t i = 0; i < subjects.size(); ++i) {
            std::string grade;
            if (std::getline(ss, grade, ';')) {
                if (!grade.empty()) { // Skip empty cells
                    outputFile << "INSERT INTO students VALUES ("
                               << group << ", '" << name << "', '"
                               << subjects[i] << "', '" << grade << "');" << std::endl;
                }
            }
        }
    }

Закрытие файлов:
После обработки всех строк входные и выходные файлы закрываются.
На консоли выводится сообщение об успешном завершении.

    inputFile.close();
    outputFile.close();

Пример вывода:
Пример вывода для студента может выглядеть следующим образом:

    INSERT INTO students VALUES (775505, 'Студ 1', 'ТПРИ', '0');


=================================
SQL-запросы

Создание таблицы:

```sql
CREATE TABLE students (
	"Группа" INTEGER,
	"ФИО" TEXT,
	"Дисциплина" TEXT,
	"Отметка" TEXT
);
```
Эта команда SQL создает таблицу с именем students, состоящую из четырех столбцов: Группа, ФИО, Дисциплина и Отметка. Типы данных указаны как INTEGER для группы и TEXT для остальных.

Выборка данных:

```sql
SELECT "Группа", "ФИО", "Дисциплина", "Отметка" FROM (
	SELECT 
    	"Группа", "ФИО", "Дисциплина",
    	unnest(string_to_array("Отметка", ' '))::TEXT as "Отметка"
		FROM students
	)
WHERE "Отметка" != '';
```
Этот запрос извлекает данные из таблицы students, разлагая оценки из строки, разделенной пробелами, в столбце "Отметка". Он фильтрует пустые оценки.
