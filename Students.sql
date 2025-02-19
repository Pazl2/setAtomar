CREATE TABLE students (
	"Группа" INTEGER,
	"ФИО" TEXT,
	"Дисциплина" TEXT,
	"Отметка" TEXT
);

SELECT "Группа", "ФИО", "Дисциплина", "Отметка" FROM (
	SELECT 
    	"Группа", "ФИО", "Дисциплина",
    	unnest(string_to_array("Отметка", ' '))::TEXT as "Отметка"
		FROM students
	)
WHERE "Отметка" != '';
