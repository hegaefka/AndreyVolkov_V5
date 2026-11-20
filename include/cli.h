#ifndef CLI_H
#define CLI_H

/**
 * @brief Структура для хранения аргументов командной строки
 */
typedef struct {
    char *input_file;   /**< Путь к входному файлу */
    char *output_file;  /**< Путь к выходному файлу */
    int task_num;       /**< Номер задания (1 или 2) */
    char *action;       /**< Действие (add, sub, mul, fact, pow, conv) */
    char *val1;         /**< Первое значение (число или база) */
    char *val2;         /**< Второе значение (степень или база) */
    char *val3;         /**< Третье значение (целевая база для conv) */
    int packed;         /**< Использовать плотную упаковку структур (1 - да, 0 - нет) */
} CliArgs;

/**
 * @brief Парсит аргументы командной строки
 * @param argc Количество аргументов
 * @param argv Массив аргументов
 * @return Структура с аргументами
 */
CliArgs parse_cli(int argc, char **argv);

/**
 * @brief Выводит справку по использованию
 */
void print_help(void);

#endif // CLI_H
