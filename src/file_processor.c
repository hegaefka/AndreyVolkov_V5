#include "file_processor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

// Вспомогательная функция для записи бинарного файла
static void write_binary(const char* filename, Item* items, size_t count, int use_packed) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        perror("Ошибка открытия файла для записи");
        return;
    }

    // Пишем заголовок
    BinaryHeader header;
    strncpy(header.magic, "XMLBIN", 8);
    header.count = (uint32_t)count;
    fwrite(&header, sizeof(BinaryHeader), 1, f);

    if (use_packed) {
        // Плотная упаковка (через #pragma pack)
        for (size_t i = 0; i < count; i++) {
            ItemPacked p;
            p.id = items[i].id;
            p.price = items[i].price;
            strncpy(p.name, items[i].name, 32);
            fwrite(&p, sizeof(ItemPacked), 1, f);
        }
        printf("Экспортировано %zu записей (Packed). Размер одной записи: %zu байт\n", count, sizeof(ItemPacked));
    } else {
        // Естественное выравнивание
        fwrite(items, sizeof(Item), count, f);
        printf("Экспортировано %zu записей (Natural). Размер одной записи: %zu байт\n", count, sizeof(Item));
    }
    fclose(f);
}

// Функция обратного чтения и проверки
static void read_and_verify(const char* filename, int use_packed) {
    FILE* f = fopen(filename, "rb");
    if (!f) return;

    BinaryHeader header;
    if (fread(&header, sizeof(BinaryHeader), 1, f) != 1) {
        printf("Ошибка чтения заголовка.\n"); fclose(f); return;
    }

    if (strncmp(header.magic, "XMLBIN", 8) != 0) {
        printf("ОШИБКА ЦЕЛОСТНОСТИ: Неверное магическое слово!\n"); fclose(f); return;
    }

    printf("Чтение файла... Магия верна. Записей в файле: %u\n", header.count);

    // Просто читаем данные, чтобы проверить, что файл не поврежден
    size_t item_size = use_packed ? sizeof(ItemPacked) : sizeof(Item);
    void* buffer = malloc(item_size * header.count);
    
    if (buffer) {
        size_t read_count = fread(buffer, item_size, header.count, f);
        if (read_count == header.count) {
            printf("УСПЕХ: Бинарный файл прочитан и проверен корректно.\n");
        } else {
            printf("ОШИБКА: Файл короче, чем заявлено в заголовке.\n");
        }
        free(buffer);
    }
    fclose(f);
}

void process_xml_task(const char* input_file, const char* output_file, int use_packed) {
    // 1. Загрузка XML
    xmlDocPtr doc = xmlReadFile(input_file, NULL, 0);
    if (!doc) {
        fprintf(stderr, "Ошибка парсинга XML файла: %s\n", input_file);
        return;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    Item* items = NULL;
    size_t count = 0;

    // 2. Обход дерева (ищем все теги <item>)
    for (xmlNodePtr node = root->children; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, (const xmlChar*)"item") == 0) {
            
            // Выделяем память под новую структуру (динамический массив)
            items = realloc(items, sizeof(Item) * (count + 1));
            if (!items) { xmlFreeDoc(doc); return; }

            memset(&items[count], 0, sizeof(Item)); // Очистка

            // Парсим атрибуты id и price
            xmlChar* id_str = xmlGetProp(node, (const xmlChar*)"id");
            xmlChar* price_str = xmlGetProp(node, (const xmlChar*)"price");
            
            if (id_str) { items[count].id = (uint32_t)atoi((char*)id_str); xmlFree(id_str); }
            if (price_str) { items[count].price = atof((char*)price_str); xmlFree(price_str); }
            
            // Парсим текст (название)
            xmlChar* content = xmlNodeGetContent(node);
            if (content) {
                strncpy(items[count].name, (char*)content, 31); // 31, оставляем место под '\0'
                xmlFree(content);
            }
            count++;
        }
    }

    // 3. Экспорт и проверка
    if (count > 0) {
        write_binary(output_file, items, count, use_packed);
        read_and_verify(output_file, use_packed);
    } else {
        printf("В XML не найдено тегов <item>\n");
    }

    // 4. Очистка памяти
    free(items);
    xmlFreeDoc(doc);
}
