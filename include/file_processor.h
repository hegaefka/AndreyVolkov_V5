#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <stdint.h>

/**
 * @brief Обычная структура с естественным выравниванием
 */
typedef struct {
    uint32_t id;          /**< Уникальный идентификатор */
    double price;         /**< Цена товара */
    char name[32];        /**< Название товара */
} Item;

/**
 * @brief Плотная структура (без выравнивания)
 */
#pragma pack(push, 1)
typedef struct {
    uint32_t id;          /**< Уникальный идентификатор */
    double price;         /**< Цена товара */
    char name[32];        /**< Название товара */
} ItemPacked;
#pragma pack(pop)

/**
 * @brief Заголовок бинарного файла для проверки целостности
 */
typedef struct {
    char magic[8];        /**< Магическое слово "XMLBIN" */
    uint32_t count;       /**< Количество записей в файле */
} BinaryHeader;

/**
 * @brief Выполняет обработку XML и бинарный экспорт
 * @param input_file Путь к входному XML файлу
 * @param output_file Путь к выходному бинарному файлу
 * @param use_packed Флаг использования плотной упаковки (1 - да, 0 - нет)
 */
void process_xml_task(const char* input_file, const char* output_file, int use_packed);

#endif // FILE_PROCESSOR_H
