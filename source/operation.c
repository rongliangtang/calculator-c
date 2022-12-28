//
// Created by 汤荣亮 on 2022/12/27.
//

#include "operation.h"

double OperationApply(Operation *operation) {
  // 如果算式结构体操作方法存在，则进行计算，否则返回0
  if (operation->operator) {
    return operation->operator(operation->left, operation->right);
  } else {
    return 0.0;
  }
}

double Plus(double left, double right) {

  return left + right;
}

double Minus(double left, double right) {

  return left - right;
}

double Multiply(double left, double right) {

  return left * right;
}

double Divide(double left, double right) {
  // 如果除数为0，返回0
  // 这里用0.0，省得再进行隐式转换
  if (right == 0.0) {
    return 0;
  } else {
    return left / right;
  }
}