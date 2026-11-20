#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "special_math.h"
#include "file_processor.h"

int main(int argc, char **argv) {
    CliArgs args = parse_cli(argc, argv);

    // ==========================================
    // ЗАДАНИЕ 1: Длинная арифметика
    // ==========================================
    if (args.task_num == 1) {
        if (!args.action) {
            fprintf(stderr, "Ошибка: укажите действие через -a (add, sub, mul, fact, pow, conv)\n");
            return EXIT_FAILURE;
        }

        char* res = NULL;

        if (strcmp(args.action, "add") == 0) {
            if (!args.val1 || !args.val2) return EXIT_FAILURE;
            res = big_add(args.val1, args.val2);
        } 
        else if (strcmp(args.action, "sub") == 0) {
            if (!args.val1 || !args.val2) return EXIT_FAILURE;
            res = big_sub(args.val1, args.val2);
        } 
        else if (strcmp(args.action, "mul") == 0) {
            if (!args.val1 || !args.val2) return EXIT_FAILURE;
            res = big_mul(args.val1, args.val2);
        } 
        else if (strcmp(args.action, "fact") == 0) {
            if (!args.val1) return EXIT_FAILURE;
            int n = atoi(args.val1);
            res = big_factorial(n);
        } 
        else if (strcmp(args.action, "pow") == 0) {
            if (!args.val1 || !args.val2) return EXIT_FAILURE;
            int exp = atoi(args.val2);
            res = big_power(args.val1, exp);
        } 
        else if (strcmp(args.action, "conv") == 0) {
            // Для перевода нужны: число (-x), из какой системы (-y), в какую (-z)
            if (!args.val1 || !args.val2 || !args.val3) {
                fprintf(stderr, "Ошибка: для conv нужны -x <число> -y <из_базы> -z <в_базу>\n");
                return EXIT_FAILURE;
            }
            int from_b = atoi(args.val2);
            int to_b = atoi(args.val3);
            res = convert_base(args.val1, from_b, to_b);
        } 
        else {
            printf("Неизвестное действие: %s\n", args.action);
        }

        // Вывод результата и очистка памяти
        if (res) {
            printf("%s\n", res);
            free(res);
        }
    } 
    // ==========================================
    // ЗАДАНИЕ 2: XML и бинарные файлы
    // ==========================================
    else if (args.task_num == 2) {
        if (!args.input_file || !args.output_file) {
            fprintf(stderr, "Ошибка: для задания 2 нужны -i <входной_xml> и -o <выходной_bin>\n");
            return EXIT_FAILURE;
        }
        process_xml_task(args.input_file, args.output_file, args.packed);
    } 
    else {
        printf("Укажите задание: -t 1 (математика) или -t 2 (XML).\n");
        printf("Для справки введите: -h\n");
    }

    return EXIT_SUCCESS;
}
