//
// Created by 汤荣亮 on 2022/12/28.
//

#include "ut_ui.h"
#include "ui.h"
#include "calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

UiContext *ui_context;

// 输出字符串函数（后面赋值给CalcContext->display_text）
static void DisplayTextWithField(char *text){
  // 将后面字符串写入到前面字符串中
  sprintf(ui_context->text_displayer, text);
}

// 初始化控制台
static void InitUt(){
  // 开启一片空间存放输出文本，即存放计算结果
  char *const kOutputBuffer = malloc(MAX_CHARS + 1);
  // 申请内存空间
  ui_context = malloc(sizeof(UiContext));
  // 设置存放计算结果输出的地方
  ui_context->text_displayer = kOutputBuffer;
  // 创建计算上下文
  ui_context->context = CreateCalcContext();
  // 输出字符串函数赋值
  ui_context->context->display_text = DisplayTextWithField;
}

// 运行控制台
static void UtMain() {
  // 打开文件
  FILE *case_file = fopen("./unit_test/cases.txt", "r");
  // 判断打开是否成功
  if (case_file) {
    // 存放文件的输入
    char key;
    // 存放文件中的正确值
    char output_result[MAX_CHARS + 1];
    // 循环读取，从流 stream 读取格式化输入，直至文件尾
    while (fscanf(case_file, " %c,%s", &key, output_result) != EOF) {
      // 将文件输入 输入HandleInput函数 处理
      if (HandleInput(ui_context->context, key)) {
        // 对比 文件中的正确值 与 计算结果（存放字符串输出的地方）
        if (strcmp(output_result, ui_context->text_displayer) == 0) {
          printf("Input [%c]: Output: %s, OK\n", key, output_result);
        } else {
          printf("Input [%c]: Output: %s, Except: %s, ERROR\n", key,ui_context->text_displayer , output_result);
          // 出错，异常退出程序（结束进程）
          exit(-1);
        }
      }
    }
    // 处理完，记得关闭文件
    fclose(case_file);
  } else {
    // 把一个描述性错误消息输出到标准错误stderr
    perror("open case file");
  }

}

// 销毁控制台
static void DestoryUt(){
  DestoryCalcContext(&ui_context->context);
  free(ui_context->text_displayer);
  free(ui_context);
  ui_context = NULL;
}

// 运行控制台ui
int RunUtUi(int argc, char *argv[]){
  InitUt();
  UtMain();
  DestoryUt();
  return 0;
}
