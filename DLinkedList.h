/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <iostream>
#include <sstream>
#include <type_traits>

#include "list/IList.h"
using namespace std;

template <class T>
class DLinkedList : public IList<T> {
 public:
  class Node;         // Forward declaration
  class Iterator;     // Forward declaration
  class BWDIterator;  // Forward declaration

 protected:
  Node *head;
  Node *tail;
  int count;
  bool (*itemEqual)(T &lhs, T &rhs);
  void (*deleteUserData)(DLinkedList<T> *);

 public:
  DLinkedList(void (*deleteUserData)(DLinkedList<T> *) = 0,
              bool (*itemEqual)(T &, T &) = 0);
  DLinkedList(const DLinkedList<T> &list);
  DLinkedList<T> &operator=(const DLinkedList<T> &list);
  ~DLinkedList();

  // Inherit from IList: BEGIN
  void add(T e);
  void add(int index, T e);
  T removeAt(int index);
  bool removeItem(T item, void (*removeItemData)(T) = 0);
  bool empty();
  int size();
  void clear();
  T &get(int index);
  int indexOf(T item);
  bool contains(T item);
  string toString(string (*item2str)(T &) = 0);
  // Inherit from IList: END

  void println(string (*item2str)(T &) = 0) {
    cout << toString(item2str) << endl;
  }
  void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0) {
    this->deleteUserData = deleteUserData;
  }

  bool contains(T array[], int size) {
    int idx = 0;
    for (DLinkedList<T>::Iterator it = begin(); it != end(); it++) {
      if (!equals(*it, array[idx++], this->itemEqual)) return false;
    }
    return true;
  }

  Iterator begin() { return Iterator(this, true); }
  Iterator end() { return Iterator(this, false); }

  BWDIterator bbegin() { return BWDIterator(this, true); }
  BWDIterator bend() { return BWDIterator(this, false); }

 protected:
  void copyFrom(const DLinkedList<T> &list);
  void removeInternalData();
  Node *getPreviousNodeOf(int index);

  //! FUNTION STATIC
 public:
  static void free(DLinkedList<T> *list) {
    typename DLinkedList<T>::Iterator it = list->begin();
    while (it != list->end()) {
      delete *it;
      it++;
    }
  }

 protected:
  static string defaultToString(T & t);
  static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &)) {
    if (itemEqual == 0)
      return lhs == rhs;
    else
      return itemEqual(lhs, rhs);
  }

 public:
  class Node {
   public:
    T data;
    Node *next;
    Node *prev;
    friend class DLinkedList<T>;

   public:
    Node(Node *next = 0, Node *prev = 0) {
      this->next = next;
      this->prev = prev;
    }
    Node(T data, Node *next = 0, Node *prev = 0) {
      this->data = data;
      this->next = next;
      this->prev = prev;
    }
  };

 public:
  class Iterator {
   private:
    DLinkedList<T> *pList;
    Node *pNode;

   public:
    Iterator(DLinkedList<T> *pList = 0, bool begin = true) {
      if (begin) {
        if (pList != 0)
          this->pNode = pList->head->next;
        else
          pNode = 0;
      } else {
        if (pList != 0)
          this->pNode = pList->tail;
        else
          pNode = 0;
      }
      this->pList = pList;
    }

    Iterator &operator=(const Iterator &iterator) {
      this->pNode = iterator.pNode;
      this->pList = iterator.pList;
      return *this;
    }
    void remove(void (*removeItemData)(T) = 0) {
      pNode->prev->next = pNode->next;
      pNode->next->prev = pNode->prev;
      Node *pNext = pNode->prev;  // MUST prev, so iterator++ will go to end
      if (removeItemData != 0) removeItemData(pNode->data);
      delete pNode;
      pNode = pNext;
      pList->count -= 1;
    }

    T &operator*() { return pNode->data; }
    bool operator!=(const Iterator &iterator) {
      return pNode != iterator.pNode;
    }
    // Prefix ++ overload
    Iterator &operator++() {
      pNode = pNode->next;
      return *this;
    }
    // Postfix ++ overload
    Iterator operator++(int) {
      Iterator iterator = *this;
      ++*this;
      return iterator;
    }
  };

  class BWDIterator {
    // TODO implement
    private:
      Node* currentNode;
      DLinkedList<T> *pList;
    public:
    /*
    BWDIterator bbegin() { return BWDIterator(this, true); }
    BWDIterator bend() { return BWDIterator(this, false); }
    */
      BWDIterator(DLinkedList<T> *pList, bool begin = true) {
        if (begin == 0) {
          if (pList != 0) 
            this->currentNode = pList->head;
          else
            this->currentNode = 0;
        }
        else {
          if (pList != 0) 
            this->currentNode = pList->tail->prev;
          else
            this->currentNode = 0;
        }
        this->pList = pList;
      }

      BWDIterator &operator=(const BWDIterator &iterator) {
        this->pList = iterator.plist;
        this->currentNode = iterator.currentNode;
        return *this;
      }

      T &operator*() {
        return currentNode->data;
      }

      bool operator!=(const BWDIterator &iterator) {
        return currentNode != iterator.currentNode;
      }

      BWDIterator &operator--() {
        if (currentNode == nullptr) {
          currentNode = pList->tail;
        }
        else {
          currentNode = currentNode->prev;
        }
        return *this;
      }

      BWDIterator operator--(int){
        BWDIterator tmp = *this;
        --(*this);
        return tmp;
      }

      void remove(void(*removeItemData)(T) = 0) {
        currentNode->prev->next = currentNode->next;
        currentNode->next->prev = currentNode->prev;
        Node *pPrev = currentNode->next;  // MUST prev, so iterator++ will go to end
        if (removeItemData != 0) removeItemData(currentNode->data);
        delete currentNode;
        currentNode = pPrev;
        pList->count -= 1;
      }
      
      /*void remove(void (*removeItemData)(T) = 0) {
      pNode->prev->next = pNode->next;
      pNode->next->prev = pNode->prev;
      Node *pNext = pNode->prev;  // MUST prev, so iterator++ will go to end
      if (removeItemData != 0) removeItemData(pNode->data);
      delete pNode;
      pNode = pNext;
      pList->count -= 1;
    }
    */
  };
};

template <class T>
using List = DLinkedList<T>;

//! ////////////////////////////////////////////////////////////////////
//! //////////////////////     METHOD DEFNITION      ///////////////////
//! ////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(void (*deleteUserData)(DLinkedList<T> *),
                            bool (*itemEqual)(T &, T &)) {
  // TODO implement
  this->deleteUserData = deleteUserData;
  this->itemEqual = itemEqual;
  this->head = new Node();
  this->tail = new Node();
  this->head->next = this->tail;
  this->tail->prev = this->head;
  this->count = 0;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list) {
  this->deleteUserData = list.deleteUserData;
  this->itemEqual = list.itemEqual;
  this->count = 0;
  copyFrom(list);
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list) {
  // TODO implement
  if (this != &list) {
    clear();
    Node* temp;
    Node* curr = list.head;
    while (curr->next != list.tail) {
      temp->next = new Node(curr->data, nullptr, temp);
      temp = temp->next;
      curr = curr->next;
    }
    tail = temp->next;
  }
  return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList() {
  clear();
  delete head;
  delete tail;
}

template <class T>
void DLinkedList<T>::add(T e) {
  // TODO implement
  Node* newNode = new Node(e, tail, tail->prev);
  (tail->prev)->next = newNode;
  tail->prev = newNode;
  count++;
}

template <class T>
void DLinkedList<T>::add(int index, T e) {
  // TODO implement
  if (index < 0 || index > count) throw std::out_of_range("Index is out of range!");
  if (index == count) {
    add(e);
    return;
  }
  else if (index == 0) {
    Node* newNode = new Node(e, head->next, head);
    head->next->prev = newNode;
    head->next = newNode;
  }
  else {
    Node* curr = head->next;
    for (int i = 0; i < index - 1; i++) {
      curr = curr->next;
    }
    Node* newNode = new Node(e, curr->next, curr);
    (curr->next)->prev = newNode;
    curr->next = newNode;
  }
  count++;
}

template <class T>
T DLinkedList<T>::removeAt(int index) {
  // TODO implement
  if (index < 0 || index >= count) throw std::out_of_range("Index is out of range!");
  if (index == 0) {
    Node* removed = head->next;
    head->next = removed->next;
    removed->next->prev = head;
    T value = removed->data;
    count--;
    delete removed;
    return value;
  }
  else {
    Node* curr = head->next;
    for (int i = 0; i < index - 1; i++) {
      curr = curr->next;
    }
    Node* removed = curr->next;
    T value = removed->data;
    curr->next = removed->next;
    removed->next->prev = curr;
    count--;
    delete removed;
    return value;
  }
}

template <class T>
bool DLinkedList<T>::empty() {
  // TODO implement
  return count==0;
}

template <class T>
int DLinkedList<T>::size() {
  // TODO implement
  return count;
}

template <class T>
void DLinkedList<T>::clear() {
  if(deleteUserData){
    deleteUserData(this);
  }
  Node* current = head->next;
  while (current != tail) {
    Node* temp = current;
    current = current->next;
    delete temp;
  }
  head->next = tail;
  tail->prev = head;
  count = 0;
}

template <class T>
T &DLinkedList<T>::get(int index) {
  // TODO implement
  if (index < 0 || index >= count) throw std::out_of_range("Index is out of range!");
  Node* curr = head->next;
  for (int i = 0; i < index; i++) {
    curr = curr->next;
  }
  return curr->data;
} 

template <class T>
int DLinkedList<T>::indexOf(T item) {
  // TODO implement
  Node* curr = head->next;
  for (int i = 0; i < count; i++) {
    if (equals(curr->data,item, itemEqual)) {
      return i;
    }
    curr = curr->next;
  }
  return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T)) {
  Node* current = head->next;
  while (current != tail) {
    if (equals(current->data, item, this->itemEqual)) {
      current->prev->next = current->next;
      current->next->prev = current->prev;

      if (removeItemData != nullptr) {
        removeItemData(current->data);
      }

      delete current;
      count--;  // Decrease the count of nodes
      return true;  // Item found and removed
    }
    current = current->next;
  }
  return false;  // Item not found
}

template <class T>
bool DLinkedList<T>::contains(T item) {
  // TODO implement
  return indexOf(item) != -1;
}


// // template <class T>
// string DLinkedList<T>::defaultToString(T & t) {
//   return t;
// }

template <class T>
//&Point::point2str_X
string DLinkedList<T>::toString(string (*item2str)(T &)) {
  // TODO implement
  stringstream ss;
  ss << "[";
  if (item2str == nullptr) {
    Node* curr = head->next;
    for (int i = 0; i < count - 1; i++) {
      ss << curr->data << ", ";
      curr = curr->next;
    }
    if (count > 0) ss << curr->data << "]";
    else ss << "]";
  }
  else {
    Node* curr = head->next;
    for (int i = 0; i < count - 1; i++) {
      ss << item2str(curr->data) << ", ";
      curr = curr->next;
    }
    if (count > 0) ss << item2str(curr->data) << "]";
    else ss << "]";
  }
  return ss.str();
}

//! ////////////////////////////////////////////////////////////////////
//! ////////////////////// (private) METHOD DEFNITION //////////////////
//! ////////////////////////////////////////////////////////////////////
template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list) {
  /**
   * Copies the contents of another doubly linked list into this list.
   * Initializes the current list to an empty state and then duplicates all data
   * and pointers from the source list. Iterates through the source list and
   * adds each element, preserving the order of the nodes.
   */
  if(this != &list){
    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;
    for (Node* current = list.head->next; current != list.tail; current = current->next) { //while (current != nullptr)
      this->add(current->data);
    }
  }
}

template <class T>
void DLinkedList<T>::removeInternalData() {
  /**
   * Clears the internal data of the list by deleting all nodes and user-defined
   * data. If a custom deletion function is provided, it is used to free the
   * user's data stored in the nodes. Traverses and deletes each node between
   * the head and tail to release memory.
   */
  // TODO implement
  clear();
  head = nullptr;
  tail = nullptr;
  count = 0;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index) {
  if (index < 0 || index >= count) throw std::out_of_range("Index is out of range!");
  Node* curr;
  if (index <= count/2) {
    curr = head->next;
    for (int i = 0; i < index-1; i++) {
      curr = curr->next;
    }
  }
  else {
    curr = tail->prev;
    for (int i = 0; i < count-index+1; i++) {
      curr = curr->prev;
    }
  }
  return curr;
}


#endif /* DLINKEDLIST_H */