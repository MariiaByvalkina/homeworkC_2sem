#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void splitCSV(char* buffer, char* tokens[]) // Функция разделения строки из массива по запятым
{
    int count = 0;
    char* token = strtok(buffer, ",");

    while (token != NULL) {
        tokens[count] = strdup(token);
        count++;
        token = strtok(NULL, ",");
    }
}

int parseCSV(char* filename, char*** data, int* countCols) // Чтение всех строк и запись в буффер
{
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Ошибка чтения файла");
        return -1;
    }

    char buffer[1024];
    int linesRead = 0;

    for (int i = 0; i < 100; i++) {
        data[i] = malloc(sizeof(char*) * 100);
        for (int j = 0; j < 100; j++) {
            data[i][j] = NULL;
        }
    }
    while (fgets(buffer, sizeof(buffer), file) != NULL) {

        buffer[strcspn(buffer, "\n")] = '\0';

        char* currentBuffer = strdup(buffer);

        splitCSV(currentBuffer, data[linesRead]);
        free(currentBuffer);

        if (linesRead == 0) {
            while (*countCols < 100 && data[0][*countCols] != NULL) { // Определили количество столбцов по первой строке
                (*countCols)++;
            }
        }
        linesRead++;
    }

    fclose(file);

    return linesRead;
}

bool isNumber(char* s) // Функция, которая проверяет является ли содержимое ячейки числом
{
    int countDot = 0;

    if (s[0] == '-' || s[0] == '+') {
        s++;
    }

    if (*s == '\0') {
        return false;
    }

    while (*s != '\0') {
        if (isdigit((unsigned char)*s)) {
            return true;
        } else if (*s == '.' && countDot == 0) {
            countDot = 1;
        } else {
            return false;
        }

        s++;
    }

    return true;
}

void calcWidth(char*** data, int rows, int cols, int* width) // Функция, которая считает длину самой длиной ячейки строки в столбце
{
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            int len = (int)strlen(data[i][j]);
            if (len > width[j]) {
                width[j] = len;
            }
        }
    }
}

void printSeparator(FILE* file, const int* widths, int col, char symbol)
{
    for (int j = 0; j < col; j++) {
        fputs("+", file);
        for (int k = 0; k < widths[j] + 2; k++) {
            fputc(symbol, file);
        }
    }

    fprintf(file, "+\n");
}

void printTable(char*** data, int rows, int cols, int* widths, char* output) // Функция печати таблицы
{
    FILE* file = fopen(output, "w");

    printSeparator(file, widths, cols, '=');

    fprintf(file, "|");

    for (int j = 0; j < cols; j++) {
        fprintf(file, " %-*s |", widths[j], data[0][j]);
    }

    fprintf(file, "\n");

    printSeparator(file, widths, cols, '=');

    for (int i = 1; i < rows; i++) {
        fprintf(file, "|");
        for (int j = 0; j < cols; j++) {
            char* cell = data[i][j];

            if (isNumber(cell)) {
                fprintf(file, " %*s |", widths[j], cell);
            } else {
                fprintf(file, " %-*s |", widths[j], cell);
            }
        }
        fprintf(file, "\n");

        printSeparator(file, widths, cols, '-');
    }
    fclose(file);
}

void freeTable(char*** data, int rows, int cols)
{ // Функция очистки памяти для таблицы

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (data[i][j] != NULL) {
                free(data[i][j]);
            }
        }
        free(data[i]);
    }
    free(data);
}

//--------tests--------

void testIsNumber(void)
{
    assert(isNumber("123") == true);
    assert(isNumber("-45") == true);
    assert(isNumber("+67") == true);
    assert(isNumber("3.14") == true);
    assert(isNumber("-2.5") == true);
    assert(isNumber("0") == true);
    assert(isNumber("0.0") == true);

    assert(isNumber("abc") == false);
    assert(isNumber("-") == false);
    assert(isNumber("+") == false);
}

void testCalcWidth(void)
{
    char*** data = malloc(sizeof(char**) * 10);
    for (int i = 0; i < 10; i++) {
        data[i] = malloc(sizeof(char*) * 10);
    }

    data[0][0] = "Name";
    data[0][1] = "Age";
    data[1][0] = "Alexander";
    data[1][1] = "25";

    int widths[10] = { 0 };
    calcWidth(data, 2, 2, widths);

    assert(widths[0] == 9);
    assert(widths[1] == 3);

    for (int i = 0; i < 10; i++) {
        free(data[i]);
    }
    free(data);
}

void testPrintTable(void)
{
    char*** data = malloc(sizeof(char**) * 100);
    int cols = 0;
    int rows = parseCSV("src/CSVprettyPrinter/testTable.csv", data, &cols);

    int widths[10] = { 0 };

    calcWidth(data, rows, cols, widths);

    printTable(data, rows, cols, widths, "src/CSVprettyPrinter/testTableOutput.txt");

    FILE* file = fopen("src/CSVprettyPrinter/testTableOutput.txt", "r");
    assert(file != NULL);

    char buffer[256];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        count++;
    }

    assert(count == 5);

    fclose(file);

    freeTable(data, rows, cols);
}

void testAlignment(void)
{
    char*** data = malloc(sizeof(char**) * 100);
    int cols = 0;
    int rows = parseCSV("src/CSVprettyPrinter/testAlignment.csv", data, &cols);

    int widths[10] = { 0 };
    calcWidth(data, rows, cols, widths);

    printTable(data, rows, cols, widths, "src/CSVprettyPrinter/testAlignmentOutput.txt");

    FILE* file = fopen("src/CSVprettyPrinter/testAlignmentOutput.txt", "r");
    char buffer[256];
    int lineCount = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        lineCount++;

        if (lineCount == 4) {
            char* pos = strstr(buffer, "1.5");
            assert(pos != NULL);
            assert(*(pos - 1) == ' ');
            assert(*(pos - 2) == ' ');
            assert(*(pos - 3) == ' ');
        }

        if (lineCount == 6) {
            char* pos = strstr(buffer, "120.50");
            assert(pos != NULL);
            assert(*(pos - 1) == ' ');
        }
    }

    fclose(file);
    freeTable(data, rows, cols);
}

void runTests(void)
{
    testIsNumber();
    testCalcWidth();
    testPrintTable();
    testAlignment();
}

int main(int argc, char* argv[])
{
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        runTests();
        return 0;
    }

    char* input = "src/CSVprettyPrinter/input.csv";
    char* output = "src/CSVprettyPrinter/output.txt";

    if (argc >= 2) {
        input = argv[1];
    }

    if (argc >= 3) {
        output = argv[2];
    }

    char*** data = malloc(sizeof(char**) * 100);
    int widths[100] = { 0 };
    int countCols = 0;
    int countRows = parseCSV(input, data, &countCols);

    if (countRows > 0) {
        calcWidth(data, countRows, countCols, widths);
        printTable(data, countRows, countCols, widths, output);
    }

    freeTable(data, countRows, countCols);
    return 0;
}
