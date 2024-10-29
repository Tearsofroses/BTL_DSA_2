/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   CrossEntropy.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"

#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction) : ILossLayer(reduction) {}

CrossEntropy::CrossEntropy(const CrossEntropy& orig) : ILossLayer(orig) {}

CrossEntropy::~CrossEntropy() {}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t) {
  // Todo CODE YOUR
  m_aCached_Ypred = X;
  m_aYtarget = t;
  const double epsilon = 1e-7;
  xt::xarray<double> log_x = xt::log(X + epsilon);
  xt::xarray<double> CE_loss_array = -t * log_x;
  double CE_loss = xt::sum(CE_loss_array)() / X.size();
  return CE_loss; 
}
xt::xarray<double> CrossEntropy::backward() {
  // Todo CODE YOUR
  xt::xarray<double> grad_y = - m_aYtarget / (m_aCached_Ypred + 1e-7);
  grad_y /= m_aCached_Ypred.size();
  return grad_y;
}