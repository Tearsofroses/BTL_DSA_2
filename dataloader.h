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
    ulong_tensor index;
    int m_seed;
    XArrayList<Batch<DType, LType>*> batches;
    TensorDataset<DType, LType>* tensor;

public:
    DataLoader(Dataset<DType, LType>* ptr_dataset, 
            int batch_size, bool shuffle=true, 
            bool drop_last=false, int seed=-1)
                : ptr_dataset(ptr_dataset), 
                batch_size(batch_size), 
                shuffle(shuffle),
                m_seed(seed),
                drop_last(drop_last) {
        nbatch = ptr_dataset->len()/batch_size;
        index = xt::arange(0, ptr_dataset->len());
        tensor = dynamic_cast<TensorDataset<DType, LType>*> (ptr_dataset);
        if (drop_last && nbatch == 0) return;
        if (nbatch == 0) nbatch = 1;
        if (shuffle) {
          xt::xarray<DType> tensor_data = tensor->getTensorData();
          xt::xarray<LType> tensor_label = tensor->getTensorLabel();
          xt::xarray<DType> shuffled_tensor_data = xt::empty<DType>(tensor_data.shape());
          xt::xarray<LType> shuffled_tensor_label = xt::empty<LType>(tensor_label.shape());
          if (m_seed >= 0) {
            xt::random::seed(m_seed);
          }
          xt::random::shuffle(index);
          int n = index.size();
          for (int i = 0; i < n; i++) {
            shuffled_tensor_data(i) = tensor_data(index[i]);
            shuffled_tensor_label(i) = tensor_label(index[i]);
          }
          tensor->setTensorData(shuffled_tensor_data);
          tensor->setTensorLabel(shuffled_tensor_label);
        }
        for (int i = 0; i < nbatch; i++) {
            xt::xarray<DType> batch_data;
            xt::xarray<LType> batch_label;
            int start = i*batch_size;
            int end = start + batch_size;
            if (i == nbatch - 1)
            if (!drop_last)
              end = ptr_dataset->len();
            if (tensor->getTensorData().dimension() == 0)
                batch_data = tensor->getTensorData();
            else
                batch_data = xt::view(tensor->getTensorData(), xt::range(start, end), xt::all());
            if (tensor->getTensorLabel().dimension() == 0)
                batch_label = tensor->getTensorLabel();
            else
                batch_label = xt::view(tensor->getTensorLabel(), xt::range(start, end), xt::all());
            batches.add(new Batch<DType, LType>(batch_data, batch_label));
        }
    }
    virtual ~DataLoader(){
        for (int i = 0; i < nbatch; i++) {
            delete batches.get(i);
        }
    }
    
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
    // TODO implement
    return Iterator(this,true);
  }

  Iterator end() {
    // TODO implement
    return Iterator(this,false);
  }
  
  //BEGIN of Iterator

    //YOUR CODE IS HERE: to define iterator
  class Iterator {
   private:
    int cursor;
    DataLoader* data_loader;
   public:
    // TODO implement contructor
    Iterator(DataLoader* data_loader = 0, bool begin = true) 
      : data_loader(data_loader)
    {
      if (begin)
        cursor = 0;
      else
        cursor = data_loader->nbatch;
    }

    Iterator& operator=(const Iterator& iterator) {
      // TODO implement
      data_loader = iterator.data_loader;
      cursor = iterator.cursor;
      return *this;
    }

    Iterator& operator++() {
      // TODO implement
      cursor++;
      return *this;
    }

    Iterator operator++(int) {
      // TODO implement
      Iterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const Iterator &iterator) const
    {
      return cursor == iterator.cursor;
    }
   
    bool operator!=(const Iterator& other) const {
      // TODO implement
      return cursor != other.cursor;
    }

    Batch<DType, LType> operator*() const {
      // TODO implement
      return *(data_loader->batches.get(cursor));
    }
  };

    //END of Iterator
    
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};


#endif /* DATALOADER_H */

