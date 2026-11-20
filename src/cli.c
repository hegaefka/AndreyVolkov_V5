#include "cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

void print_help(void) {
    printf("Использование: app [ОПЦИИ]\n");
    printf("  -h, --help            Показать эту справку\n");
    printf("  -i, --input <file>    Входной файл\n");
    printf("  -o, --output <file>   Выходной файл\n");
    printf("  -t, --task <num>      Номер задания (1 или 2)\n");
    printf("  -a, --action <act>    Действие (add, sub, mult)\n");
    printf("  -x, --val1 <num>      Первое число\n");
    printf("  -y, --val2 <num>      Второе число\n");
    printf("  -p, --packed          Использовать плотную упаковку структур\n");
}

CliArgs parse_cli(int argc, char **argv) {
    CliArgs args = {0};
    static struct option long_options[] = {
        {"help",   no_argument,       0, 'h'},
        {"input",  required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {"task",   required_argument, 0, 't'},
        {"action", required_argument, 0, 'a'},
        {"val1",   required_argument, 0, 'x'},
        {"val3",   required_argument, 0, 'z'},
        {"val2",   required_argument, 0, 'y'},
        {"packed", no_argument,       0, 'p'},
        {0, 0, 0, 0}
    };
    
    int opt;
    while ((opt = getopt_long(argc, argv, "hi:o:t:a:x:z:y:p", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h': print_help(); exit(EXIT_SUCCESS);
            case 'i': args.input_file = optarg; break;
            case 'o': args.output_file = optarg; break;
            case 't': args.task_num = atoi(optarg); break;
            case 'a': args.action = optarg; break;
            case 'x': args.val1 = optarg; break;
            case 'z': args.val3 = optarg; break;
            case 'y': args.val2 = optarg; break;
            case '?': 
            case 'p': args.packed = 1; break;
                fprintf(stderr, "Неверный аргумент.\n");
                exit(EXIT_FAILURE);
        }
    }
    return args;
}
