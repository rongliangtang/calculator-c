//
// Created by 汤荣亮 on 2022/12/27.
//

/*这个文件定义了Ui上下文结构体，用来实现不同类型的运行方式*/

#ifndef CALCULATOR__UI_H_
#define CALCULATOR__UI_H_

#include "calculator.h"

/**
 * Ui上下文
 * 通过进行不同的赋值，来实现不同类型的运行方式
 */
typedef struct {
  // 存放输出文本（stdout、char *、gtk）
  void *text_displayer;
  // 存放计算上下文
  CalcContext *context;
} UiContext;

#endif //CALCULATOR__UI_H_
