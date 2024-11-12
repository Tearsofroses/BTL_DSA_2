/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/*
 * File:   CrossEntropy.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"
#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction) : ILossLayer(reduction)
{
}

CrossEntropy::CrossEntropy(const CrossEntropy &orig) : ILossLayer(orig)
{
}

CrossEntropy::~CrossEntropy()
{
}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t)
{
  m_aCached_Ypred = X;
  m_aYtarget = t;
  if (this->m_eReduction != LossReduction::REDUCE_MEAN)
    return cross_entropy(X, t, false);
  else
    return cross_entropy(X, t, true);
}
xt::xarray<double> CrossEntropy::backward()
{
  xt::xarray<double> grad = m_aYtarget / (m_aCached_Ypred + 1e-7);
  if (this->m_eReduction == LossReduction::REDUCE_MEAN)
    grad = -grad / m_aCached_Ypred.shape()[0];
  else
  {
    grad = -grad;
  }
  return grad;
}