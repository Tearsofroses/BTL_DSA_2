/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"

using namespace std;

template<typename DType, typename LType>
class DataLoader{
public:
    class Iterator; //forward declaration for class Iterator
    
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int nbatch;
    ulong_tensor item_indices;
    int m_seed;
    
public:
    DataLoader(Dataset<DType, LType>* ptr_dataset, 
            int batch_size, bool shuffle=true, 
            bool drop_last=false, int seed=-1)
                : ptr_dataset(ptr_dataset), 
                batch_size(batch_size), 
                shuffle(shuffle),
                m_seed(seed){
            nbatch = ptr_dataset->len()/batch_size;
            item_indices = xt::arange(0, ptr_dataset->len());
            if (shuffle){
                if (m_seed != -1){
                    std::random::seed(m_seed);
                }
                std::random_shuffle(item_indices.begin(), item_indices.end());
            }
    }
    virtual ~DataLoader(){}
    
    //New method: from V2: begin
    int get_batch_size(){ return batch_size; }
    int get_sample_count(){ return ptr_dataset->len(); }
    int get_total_batch(){return int(ptr_dataset->len()/batch_size); }
    
    //New method: from V2: end
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
public:
    Iterator begin() {
    return Iterator(this,0);
  }

  Iterator end() {
    return Iterator(this, batch_count);
  }

  class Iterator {
   private:
    int curr_index;
    DataLoader<DType, LType>* data_loader;
   public:
    Iterator(DataLoader* data_loader = 0, int idx = 0) 
      : data_loader(data_loader), curr_index(idx) {}

    Iterator& operator=(const Iterator& iterator) {
      data_loader = iterator.data_loader;
      curr_index = iterator.curr_index;
      return *this;
    }

    Iterator& operator++() {
      curr_index++;
      return *this;
    }

    Iterator operator++(int) {
      Iterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const Iterator &iterator) const {
      return curr_index == iterator.curr_index;
    }
   
    bool operator!=(const Iterator& other) const {
      return curr_index != other.curr_index;
    }

    Batch<DType, LType> operator*() const {
      int start = curr_index * (data_loader->batch_size);
      int end = start + (data_loader->batch_size);
      if (curr_index == data_loader->batch_count - 1 && !(data_loader->drop_last))
        end = data_loader->ptr_dataset->len();
      xt::svector<std::size_t> data_shape = data_loader->ptr_dataset->get_data_shape();
      xt::svector<std::size_t> label_shape = data_loader->ptr_dataset->get_label_shape();
      data_shape[0] = end - start;
      if (label_shape.size() == 0) {
        xt::xarray<DType> batch_data = xt::zeros<DType>(data_shape);
        for (int i = start; i < end; i++) {
          xt::view(batch_data, i - start) = data_loader->ptr_dataset->getitem(data_loader->index(i)).getData(); 
        }
        return Batch<DType, LType> (batch_data, data_loader->ptr_dataset->getitem(0).getLabel());
      }
      label_shape[0] = end - start;
      xt::xarray<DType> batch_data = xt::zeros<DType>(data_shape);
      xt::xarray<LType> batch_label = xt::empty<LType>(label_shape);
      for (int i = start; i < end; i++) {
        xt::view(batch_data, i - start) = data_loader->ptr_dataset->getitem(data_loader->index(i)).getData();
        xt::view(batch_label, i - start) = data_loader->ptr_dataset->getitem(data_loader->index(i)).getLabel();
      }
      return Batch<DType, LType> (batch_data, batch_label);
    }
  };
};

#endif /* DATALOADER_H */

