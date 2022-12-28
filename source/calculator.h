//
// Created by 汤荣亮 on 2022/12/27.
//

/**/

#ifndef CALCULATOR__CALCULATOR_H_
#define CALCULATOR__CALCULATOR_H_

#include "operation.h"

// 因为double能保证的有效位数为6，表示最大范围时如-1.56805e+015
#define MAX_CHARS 13

/**
 * 表示计算器的上下文
 */
typedef struct {
  // 表示输入的数字
  char input_buffer[MAX_CHARS + 1];
  // 输入的数字位数position，执行\0这个位置
  int input_buffer_position;
  // 计算结果
  double result;
  // 上次算式
  Operation previous_operation;
  // 当前算式
  Operation current_operation;
  // 输出字符串（函数指针）
  void (*display_text)(char *);
} CalcContext;

/**
 * 处理输入，进行操作
 * @param context
 * @param input_value
 * @return
 */
int HandleInput(CalcContext *context, char input_value);

/**
 * 创建上下文
 * @return
 */
CalcContext *CreateCalcContext();

/**
 * 销毁上下文
 * @param p_context
 * @return
 */
void DestroyCalcContext(CalcContext **p_context);

#endif //CALCULATOR__CALCULATOR_H_
