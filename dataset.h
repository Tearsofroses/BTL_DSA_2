/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataset.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 3:59 PM
 */

#ifndef DATASET_H
#define DATASET_H
#include "tensor/xtensor_lib.h"
using namespace std;

template <typename DType, typename LType>
class DataLabel {
 private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;

 public:
  DataLabel(xt::xarray<DType> data, xt::xarray<LType> label)
      : data(data), label(label) {}
  xt::xarray<DType> getData() const { return data; }
  xt::xarray<LType> getLabel() const { return label; }
};

template <typename DType, typename LType>
class Batch {
 private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;

 public:
  Batch() = default;
  Batch(xt::xarray<DType> data, xt::xarray<LType> label)
      : data(data), label(label) {}
  virtual ~Batch() {}
  xt::xarray<DType>& getData() { return data; }
  xt::xarray<LType>& getLabel() { return label; }
};

template <typename DType, typename LType>
class Dataset {
 private:
 public:
  Dataset() {};
  virtual ~Dataset() {};

  virtual int len() = 0;
  virtual DataLabel<DType, LType> getitem(int index) = 0;
  virtual xt::svector<std::size_t> get_data_shape() = 0;
  virtual xt::svector<std::size_t> get_label_shape() = 0;
};

//////////////////////////////////////////////////////////////////////
template <typename DType, typename LType>
class TensorDataset : public Dataset<DType, LType> {
 private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;
  xt::svector<std::size_t> data_shape, label_shape;

 public:
  TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label)
    : data(data), label(label)
  {
    data_shape = data.shape();
    label_shape = label.shape();
  }

  int len() {
    return data_shape[0];
  }

  DataLabel<DType, LType> getitem(int index) {
    if (index < 0 || index >= len()) 
      throw out_of_range("Index is out of range!");
    int label_dimension = label.dimension();
    xt::xarray<DType> temp_data = xt::view(data, index);
    xt::xarray<LType> temp_label;
    if (label_dimension == 0)
      temp_label = label;
    else
      temp_label = xt::view(label, index);
    return DataLabel<DType, LType> (temp_data, temp_label);
  }

  xt::svector<std::size_t> get_data_shape() {
    return data_shape;
  }

  xt::svector<std::size_t> get_label_shape() {
    return label_shape;
  }

  virtual ~TensorDataset() {};
};

#endif /* DATASET_H */
