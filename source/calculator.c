//
// Created by 汤荣亮 on 2022/12/27.
//

#include "calculator.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 清除上下文Buffer，对应内存置0
static void ClearBuffer(CalcContext *context){
  // 数组利用编译器特性，方便置0
  context->input_buffer[0] = 0;
  context->input_buffer_position = 0;
}

// 清除上下文，内存全部置0
static void ClearAll(CalcContext *context){
  ClearBuffer(context);
  context->result = 0;
  // memset将某一块内存中的全部设置为指定的值
  // 结构体中内存是连续的
  // NULL实际为地址0，所以用memset把指针置0效果一样
  memset(&context->previous_operation, 0, sizeof(Operation));
  memset(&context->current_operation, 0, sizeof(Operation));
}

/**
 * 对输入的数字进行格式化（去除多余的0和小数点）
 * @param context
 */
static void FormatInput(CalcContext *context){
  // 如果没有输入，格式化为"0"
  if (context->input_buffer_position == 0) {
    context->input_buffer[0] = '0';
    context->input_buffer[1] = 0;
    context->input_buffer_position = 1;
  // 如果输入累计等于1
  } else if (context->input_buffer_position == 1) {
    // 有一个输入且为'.'，格式为"0."
    if (context->input_buffer[0] == '.') {
      context->input_buffer[0] = '0';
      context->input_buffer[1] = '.';
      context->input_buffer[2] = 0;
      context->input_buffer_position = 2;
    }
  // 如果输入累计大于1
  } else if (context->input_buffer_position > 1) {
    char *p = context->input_buffer;
    // 格式化"0123" -> "123"
    if (p[0] == '0' && p[1] != '.') {
      p[0] = p[1];
      p[1] = 0;
      context->input_buffer_position = 1;
    // 格式化"123..." -> "123."
    } else if (p[context->input_buffer_position - 1] == '.') {
      for (int i = 0; i < context->input_buffer_position - 1; ++i) {
        if (p[i] == '.') {
          context->input_buffer_position--;
          p[context->input_buffer_position] = 0;
          break;
        }
      }
    }
  }
}

/**
 * 输出输入的数字
 * @param context
 */
static void DisplayInput(CalcContext *context){
  // 格式化输入（去除多余的0和小数点）
  FormatInput(context);
  // 输出输入
  context->display_text(context->input_buffer);
}

/**
 * 对输出的数字进行格式化
 * @param context
 */
static void FormatOutput(CalcContext *context) {
  // double会出现-0.0的情况
  if (context->result == -0.0) {
    context->result = 0.0;
  }
  // 把result格式化后放入到input中（自动转为字符串）
  // %.6g表示6位有效数字，超过6位会采用科学计数法
  sprintf(context->input_buffer, "%.6g", context->result);
}

/**
 * 显示输出计算结果
 * @param context
 */
static void DisplayOutput(CalcContext *context) {
  // 对输出的数字进行格式化
  FormatOutput(context);
  // 显示input
  context->display_text(context->input_buffer);
  // 对buffer清0
  ClearBuffer(context);
}

/**
 * 获取输入，转换double
 * @param context
 * @param operand
 */
static void ApplyInput(CalcContext *context, double *operand) {
  if (context->input_buffer_position > 0) {
    *operand = atof(context->input_buffer);
    ClearBuffer(context);
  } else {
    *operand = context->result;
  }
}

/**
 * 获取输入，转换double，放到current_operation.left中
 * @param context
 */
static void ApplyInputToLeft(CalcContext *context) {
  ApplyInput(context, &context->current_operation.left);
}

/**
 * 获取输入，转换double，放到current_operation.right中
 * @param context
 */
static void ApplyInputToRight(CalcContext *context) {
  ApplyInput(context, &context->current_operation.right);
}

/**
 * 对当前算式进行计算
 * @param context
 */
static void ApplyCurrentOperation(CalcContext *context) {
  // 对当前算式进行运算
  context->result = OperationApply(&context->current_operation);
  // 把当前算式结构体复制到先前算式结构体中
  memcpy(&context->previous_operation, &context->current_operation, sizeof(Operation));
  // 对当前算式结构体进行清0
  memset(&context->current_operation, 0, sizeof(Operation));
}

/**
 * 对先前算式进行计算
 * @param context
 */
static void ApplyPreviousOperation(CalcContext *context) {
  context->result = OperationApply(&context->previous_operation);
}

// 处理输入，进行操作
int HandleInput(CalcContext *context, char input_value){
  // 易犯错误：switch没有遇到break，会从跳转的分支往下执行，直至遇到break
  switch (input_value) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.': {
      // 将输入放到input_value中
      if (context->input_buffer_position < MAX_CHARS) {
        context->input_buffer[context->input_buffer_position++] = input_value;
        context->input_buffer[context->input_buffer_position] = '\0';
      }
      // 对放入这次输入字符的input_value进行显示
      DisplayInput(context);
      break;
    }

    case '+':
      // 对current_operation.operator函数进行赋值，确定进行什么操作
      context->current_operation.operator = Plus;
      // 取出input，放到current_operation.left中
      ApplyInputToLeft(context);
      break;

    case '-':
      // 对current_operation.operator函数进行赋值，确定进行什么操作
      context->current_operation.operator = Minus;
      // 取出input，放到current_operation.left中
      ApplyInputToLeft(context);
      break;

    case '*':
      // 对current_operation.operator函数进行赋值，确定进行什么操作
      context->current_operation.operator = Multiply;
      // 取出input，放到current_operation.left中
      ApplyInputToLeft(context);
      break;

    case '/':
      // 对current_operation.operator函数进行赋值，确定进行什么操作
      context->current_operation.operator = Divide;
      // 取出input，放到current_operation.left中
      ApplyInputToLeft(context);
      break;

    case '=':
      // 如果当前输入了运算操作
      if (context->current_operation.operator) {
        // 取出input，放到current_operation.right中
        ApplyInputToRight(context);
        // 对当前算式进行计算
        ApplyCurrentOperation(context);
        // 输出显示运算结果
        DisplayOutput(context);
      // 如果没有输入当前运算操作，且上次操作存在
      // 实现按=，再次进行上次运算
      } else if (context->previous_operation.operator) {
        // 设置先前算式left=当前result
        context->previous_operation.left = context->result;
        // 计算先前算式
        ApplyPreviousOperation(context);
        // 输出显示运算结果
        DisplayOutput(context);
      // 如果没有输入当前运算操作，且上次操作不存在
      } else {
        printf("No operation.\n");
      }
      break;

    // 逻辑：将当前输入*0.01
    case '%':
      ApplyInputToLeft(context);
      context->current_operation.operator = Multiply;
      context->current_operation.right = 0.01;
      ApplyCurrentOperation(context);
      DisplayOutput(context);
      break;

    // 逻辑：将当前输入*-1
    case 'S':
      ApplyInputToLeft(context);
      context->current_operation.operator = Multiply;
      context->current_operation.right = -1;
      ApplyCurrentOperation(context);
      DisplayOutput(context);
      break;

    // 逻辑：全部清0，重新开始计算
    case 'C':
      ClearAll(context);
      DisplayInput(context);
      break;
  }
  return 1;
}

CalcContext *CreateCalcContext(){
  // 申请内存，返回指针
  CalcContext *calc_context = malloc(sizeof(CalcContext));
  // 对申请的内存进行清0
  ClearAll(calc_context);
  // 指针指向空
  calc_context->display_text = NULL;
  return calc_context;
}

// 因为需要改变指针的值，所以函数传参要为指针的指针
void DestroyCalcContext(CalcContext **p_context){
  free(*p_context);
  *p_context = NULL;
}