//
// Created by 汤荣亮 on 2022/12/27.
//

/*定义表示算式的结构体，及运算方法*/

#ifndef CALCULATOR__OPERATION_H_
#define CALCULATOR__OPERATION_H_

/**
 * 定义一个结构体类型别名
 * 可以用Operation来创建结构体变量，表示一个操作算式
 */
typedef struct {
  // 左边操作数
  double left;
  // 右边操作数
  double right;
  // 操作方法（函数指针）
  double (*operator)(double, double);
} Operation;

/**
 * 计算结果
 * @param operation 操作算式结构体
 * @return 计算结果
 */
double OperationApply(Operation *operation);

/**
 * 加法操作
 * @param left 左边操作数
 * @param right 右边操作数
 * @return 计算结果
 */
double Plus(double left, double right);

/**
 * 减法操作
 * @param left 左边操作数
 * @param right 右边操作数
 * @return 计算结果
 */
double Minus(double left, double right);

/**
 * 乘法操作
 * @param left 左边操作数
 * @param right 右边操作数
 * @return 计算结果
 */
double Multiply(double left, double right);

/**
 * 除法操作
 * @param left 左边操作数
 * @param right 右边操作数
 * @return 计算结果
 */
double Divide(double left, double right);

#endif //CALCULATOR__OPERATION_H_
