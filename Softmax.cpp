/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   Softmax.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:46 PM
 */

#include "layer/Softmax.h"

#include <filesystem>  //require C++17

#include "ann/functions.h"
#include "sformat/fmt_lib.h"
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name) : m_nAxis(axis) {
  if (trim(name).size() != 0)
    m_sName = name;
  else
    m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax& orig) {}

Softmax::~Softmax() {}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
  // Todo CODE YOUR
  m_aCached_Y = xt::xarray<double>::from_shape(X.shape());
  if (X.dimension() < 2) 
    this->m_aCached_Y = softmax(X);
  else {
    for (int i = 0; i < X.shape()[0]; i++) {
      xt::view(m_aCached_Y, i, xt::all()) = softmax(xt::view(X, i, xt::all()));
    }
  }
  return m_aCached_Y;
}

xt::xarray<double> Softmax::backward(xt::xarray<double> DY) {
  // Todo CODE YOUR
  xt::xarray<double> Z(m_aCached_Y.shape());
  if (DY.dimension() < 2) {
    xt::xarray<double> jacobian = xt::diag(m_aCached_Y) - xt::linalg::outer(m_aCached_Y, m_aCached_Y);
    Z = xt::linalg::dot(DY, jacobian);
  }
  else {
    for (int i = 0; i < m_aCached_Y.shape()[0]; i++) {
      xt::xarray<double> row = xt::view(m_aCached_Y, i, xt::all());
      xt::xarray<double> jacobian = xt::diag(row) - xt::linalg::outer(row, row);
      xt::view(Z, i, xt::all()) = xt::linalg::dot(jacobian, xt::view(DY, i, xt::all()));
    }
  }
  return Z;
}

string Softmax::get_desc() {
  string desc = fmt::format("{:<10s}, {:<15s}: {:4d}", "Softmax",
                            this->getname(), m_nAxis);
  return desc;
}
