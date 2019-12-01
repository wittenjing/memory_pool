// StackAlloc.hpp

#ifndef STACK_ALLOC_H
#define STACK_ALLOC_H

#include <memory>

template <typename T>
struct StackNode_
{
  T data;
  StackNode_* prev;
};

// T 为存储的对象类型, Alloc 为使用的分配器,
// 并默认使用 std::allocator 作为对象的分配器
template <class T, class Alloc = std::allocator<T> >
class StackAlloc
{
  public:
    // 使用 typedef 简化类型名
    typedef StackNode_<T> Node;
    typedef typename Alloc::template rebind<Node>::other allocator;

    // 默认构造
    StackAlloc() { head_ = 0; }
    // 默认析构
    ~StackAlloc() { clear(); }

    // 当栈中元素为空时返回 true
    bool empty() {return (head_ == 0);}

    // 释放栈中元素的所有内存
    void clear() {
      Node* curr = head_;
      while (curr != 0)
      {
        Node* tmp = curr->prev;
        allocator_.destroy(curr);
        allocator_.deallocate(curr, 1);
        curr = tmp;
      }
      head_ = 0;
    }

    // 入栈
    void push(T element) {
      // 为一个节点分配内存
      Node* newNode = allocator_.allocate(1);
      // 调用节点的构造函数
      allocator_.construct(newNode, Node());

      // 入栈操作
      newNode->data = element;
      newNode->prev = head_;
      head_ = newNode;
    }

    // 出栈
    T pop() {
      // 出栈操作 返回出栈结果
      T result = head_->data;
      Node* tmp = head_->prev;
      allocator_.destroy(head_);
      allocator_.deallocate(head_, 1);
      head_ = tmp;
      return result;
    }

    // 返回栈顶元素
    T top() { return (head_->data); }

  private:
    allocator allocator_;
    Node* head_;
};

#endif // STACK_ALLOC_H