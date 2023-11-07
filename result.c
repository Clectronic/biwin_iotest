
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "include/result.h"
#include "include/tool.h"
extern Arguments opt_args;
void drawHorizontalLine(int width, int cellWidth) {
    printf("+");
    for (int i = 0; i < width; i++) {
        if (i % cellWidth == 0) {
            printf("+");
        } else {
            printf("-");
        }
    }
    printf("+\n");
}

void drawCell(const char* text, int width, int isHeader) {
    int textLen = strlen(text);

    if (isHeader) {
        printf("| \033[1;37m%-*s\033[0m |", width - 2, text);
    } else {
        printf("| %-*s |", width - 2, text);
    }
}

int print_test_results(thread_info *t){
    // Table header
    printf("\033[1;34m"); // 设置标题为蓝色
    drawHorizontalLine(94, 16);
    printf("| \033[1;37mOperation\033[0m    || \033[1;37mRate\033[0m         || \033[1;37mTime\033[0m         || \033[1;37mCPU Usage\033[0m    || \033[1;37mAvg Delay\033[0m    || \033[1;37mMax Delay\033[0m    |");
    printf("\n");
    printf("\033[1;34m"); // 设置标题为蓝色
    drawHorizontalLine(94, 16);
    //计算结果
    
    // Write data
    drawCell("Write", 14, 1);
    drawCell("123.45 MB/s", 14, 0);
    drawCell("00:10:15", 14, 0);
    drawCell("30.5%", 14, 0);
    drawCell("0.005 sec", 14, 0);
    drawCell("0.01 sec", 14, 0);
    printf("\n");
    drawHorizontalLine(94, 16);

    // Read data
    drawCell("Read", 14, 1);
    drawCell("67.89 MB/s", 14, 0);
    drawCell("00:08:20", 14, 0);
    drawCell("20.2%", 14, 0);
    drawCell("0.003 sec", 14, 0);
    drawCell("0.008 sec", 14, 0);
    printf("\n");
    drawHorizontalLine(94, 16);

    // Random Write data
    drawCell("Random Write", 14, 1);
    drawCell("45.67 MB/s", 14, 0);
    drawCell("00:05:50", 14, 0);
    drawCell("40.2%", 14, 0);
    drawCell("0.006 sec", 14, 0);
    drawCell("0.02 sec", 14, 0);
    printf("\n");
    drawHorizontalLine(94,16);

    // Random Read data
    drawCell("Random Read", 14, 1);
    drawCell("55.78 MB/s", 14, 0);
    drawCell("00:05:40", 14, 0);
    drawCell("45.8%", 14, 0);
    drawCell("0.004 sec", 14, 0);
    drawCell("0.03 sec", 14, 0);
    printf("\n");
    drawHorizontalLine(94, 16);

    printf("\033[0m"); // 重置颜色
}