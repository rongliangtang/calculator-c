//
// Created by 汤荣亮 on 2022/12/27.
//

#include "console_ui.h"
#include "ui.h"
#include "calculator.h"
#include <stdio.h>
#include <stdlib.h>

UiContext *ui_context;

// 输出字符串函数
static void DisplayTextWithField(char *text){
  // 把字符串写入到指定的流 stream 中，但不包括空字符。
  // 系统不会自动添加换行符
  fputs(text, ui_context->text_displayer);
  // 把字符写入到指定的流 stream 中，但不包括空字符。
  fputc('\n', ui_context->text_displayer);
  // 刷新流的输出缓冲区，因为流有个缓冲区，不满不会输出，这里强制刷新输出
  fflush(ui_context->text_displayer);
}

// 初始化控制台
static void InitConsole(){
  // 申请内存空间
  ui_context = malloc(sizeof(UiContext));
  // 设置显示文本输出流
  ui_context->text_displayer = stdout;
  // 创建计算上下文
  ui_context->context = CreateCalcContext();
  // 输出字符串函数赋值
  ui_context->context->display_text = DisplayTextWithField;
}

// 运行控制台
static void ConsoleMain() {
  int next_input = getchar();

  while (next_input != EOF && next_input != 'q' && next_input != 'Q'){
    HandleInput(ui_context->context, next_input);
    next_input = getchar();
  }
  // 系统会自动添加换行符
  puts("Thank you!");
}

// 销毁控制台
static void DestoryConsole(){
  DestoryCalcContext(&ui_context->context);
  free(ui_context);
  ui_context = NULL;
}

// 运行控制台ui
int RunConsoleUi(int argc, char *argv[]){
  InitConsole();
  ConsoleMain();
  DestoryConsole();
  return 0;
}