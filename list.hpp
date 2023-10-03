#pragma once
#include "iostream"

template <typename T, typename Allocator = std::allocator<T>>
class List {
 private:
  struct TNode {
    TNode* prev;
    TNode* next;
    T value;
    explicit TNode(TNode* prev = nullptr, TNode* next = nullptr)
        : value(T()), prev(prev), next(next) {}
    explicit TNode(const T& value, TNode* prev = nullptr, TNode* next = nullptr)
        : value(value), prev(prev), next(next) {}
  };

 public:
  using traits = std::allocator_traits<Allocator>;
  using node_allocator = typename traits::template rebind_alloc<TNode>;
  using node_traits = std::allocator_traits<node_allocator>;
  using value_type = T;
  using allocator_type =
      typename std::allocator_traits<Allocator>::allocator_type;
  List(const Allocator& allocator = Allocator()) : allocator_(allocator) {
    size_ = 0;
    root_ = node_traits::allocate(allocator_, 1);
    root_->next = root_;
    root_->prev = root_;
  }
  List(size_t count, const T& value, const Allocator& allocator = Allocator())
      : allocator_(allocator) {
    root_ = node_traits::allocate(allocator_, 1);
    root_->next = root_;
    root_->prev = root_;
    try {
      for (size_t ind = 0; ind < count; ++ind) {
        push_back(value);
      }
    } catch (...) {
      TNode* current = root_->next;
      while (current != root_) {
        TNode* tmp = current->next;
        node_traits::destroy(allocator_, current);
        node_traits::deallocate(allocator_, current, 1);
        --size_;
        current = tmp;
      }
      size_ = 0;
      node_traits::deallocate(allocator_, root_, 1);
      throw;
    }
  }
  explicit List(size_t count, const Allocator& allocator = Allocator())
      : allocator_(allocator) {
    root_ = node_traits::allocate(allocator_, 1);
    root_->next = root_;
    root_->prev = root_;
    try {
      for (size_t ind = 0; ind < count; ++ind) {
        push_back();
      }
    } catch (...) {
      TNode* current = root_->next;
      while (current != root_) {
        TNode* tmp = current->next;
        node_traits::destroy(allocator_, current);
        node_traits::deallocate(allocator_, current, 1);
        --size_;
        current = tmp;
      }
      size_ = 0;
      node_traits::deallocate(allocator_, root_, 1);
      throw;
    }
  }
  List(const List& other)
      : allocator_(
            traits::select_on_container_copy_construction(other.allocator_)) {
    root_ = node_traits::allocate(allocator_, 1);
    root_->next = root_;
    root_->prev = root_;
    try {
      TNode* tmp = other.root_;
      for (size_t ind = 0; ind < other.size_; ++ind) {
        tmp = tmp->next;
        push_back(tmp->value);
      }
    } catch (...) {
      TNode* current = root_->next;
      while (current != root_) {
        TNode* tmp = current->next;
        node_traits::destroy(allocator_, current);
        node_traits::deallocate(allocator_, current, 1);
        --size_;
        current = tmp;
      }
      size_ = 0;
      node_traits::deallocate(allocator_, root_, 1);
      throw;
    }
  }
  List(std::initializer_list<T> initializer,
       const Allocator& allocator = Allocator())
      : allocator_(allocator) {
    root_ = node_traits::allocate(allocator_, 1);
    root_->next = root_;
    root_->prev = root_;
    try {
      TNode* tmp = root_->next;
      for (auto itr = initializer.begin(); itr != initializer.end(); ++itr) {
        push_back(*itr);
      }
    } catch (...) {
      TNode* current = root_->next;
      while (current != root_) {
        TNode* tmp = current->next;
        node_traits::destroy(allocator_, current);
        node_traits::deallocate(allocator_, current, 1);
        --size_;
        current = tmp;
      }
      size_ = 0;
      node_traits::deallocate(allocator_, root_, 1);
      throw;
    }
  }
  ~List() {
    TNode* current = root_->next;
    while (current != root_) {
      TNode* tmp = current->next;
      node_traits::destroy(allocator_, current);
      node_traits::deallocate(allocator_, current, 1);
      --size_;
      current = tmp;
    }
    size_ = 0;
    node_traits::deallocate(allocator_, root_, 1);
  }
  List& operator=(const List& other) {
    if (this == &other) {
      return *this;
    }
    List tmp(other);
    if (node_traits::propagate_on_container_copy_assignment::value and
        tmp.allocator_ != other.allocator_) {
      tmp.allocator_ = other.allocator_;
    }
    std::swap(root_, tmp.root_);
    allocator_ = tmp.allocator_;
    size_ = tmp.size_;
    return *this;
  }
  bool empty() const { return size_ == 0; }
  size_t size() const { return size_; }

  node_allocator get_allocator() { return allocator_; }

  T& front() { return root_->next->value; }
  const T& front() const { return root_->next->value; }
  T& back() { return root_->prev->value; }
  const T& back() const { return root_->prev->value; }

  void push_back() {
    TNode* tmp = node_traits::allocate(allocator_, 1);
    try {
      node_traits::construct(allocator_, tmp, root_->prev, root_);
    } catch (...) {
      node_traits::deallocate(allocator_, tmp, 1);
      throw;
    }
    root_->prev->next = tmp;
    tmp->next->prev = tmp;
    ++size_;
  }
  void push_back(const T& value) {
    TNode* tmp = node_traits::allocate(allocator_, 1);
    try {
      node_traits::construct(allocator_, tmp, std::move(value), root_->prev,
                             root_);
    } catch (...) {
      node_traits::deallocate(allocator_, tmp, 1);
      throw;
    }
    root_->prev->next = tmp;
    tmp->next->prev = tmp;
    ++size_;
  }
  void push_back(T&& value) {
    TNode* tmp = node_traits::allocate(allocator_, 1);
    node_traits::construct(allocator_, tmp, std::move(value), root_->prev,
                           root_);
    root_->prev->next = tmp;
    tmp->next->prev = tmp;
    ++size_;
  }
  void push_front(const T& value) {
    TNode* tmp = node_traits::allocate(allocator_, 1);
    try {
      node_traits::construct(allocator_, tmp, std::move(value), root_,
                             root_->next);
    } catch (...) {
      node_traits::deallocate(allocator_, tmp, 1);
      throw;
    }
    root_->next = tmp;
    tmp->next->prev = tmp;
    ++size_;
  }
  void push_front(T&& value) {
    TNode* tmp = node_traits::allocate(allocator_, 1);
    node_traits::construct(allocator_, tmp, std::move(value), root_,
                           root_->next);
    root_->next = tmp;
    tmp->next->prev = tmp;
    ++size_;
  }
  void pop_back() {
    if (root_->prev == root_) {
      return;
    }
    TNode* tmp = root_->prev;
    root_->prev = tmp->prev;
    root_->prev->next = root_;
    node_traits::destroy(allocator_, tmp);
    node_traits::deallocate(allocator_, tmp, 1);
    --size_;
  }
  void pop_front() {
    if (root_->next == root_) {
      return;
    }
    TNode* tmp = root_->next;
    root_->next = tmp->next;
    root_->next->prev = root_;
    node_traits::destroy(allocator_, tmp);
    node_traits::deallocate(allocator_, tmp, 1);
    --size_;
  }
  template <bool IsConst>
  class TListIterator {
   public:
    using value_type = typename std::conditional<IsConst, const T, T>::type;
    using difference_type = std::ptrdiff_t;
    using pointer = typename std::conditional<IsConst, const T*, T*>::type;
    using reference = typename std::conditional<IsConst, const T&, T&>::type;
    using iterator_category = std::bidirectional_iterator_tag;

    TNode* get_node() { return node_; }

    explicit operator TListIterator<true>() {
      return TListIterator<true>(node_);
    }
    explicit TListIterator(TNode* node) : node_(node) {}
    TListIterator& operator++() {
      node_ = node_->next;
      return *this;
    }
    TListIterator operator++(int) {
      TNode tmp = *this;
      ++(*this);
      return tmp;
    }
    TListIterator& operator--() {
      node_ = node_->prev;
      return *this;
    }
    TListIterator operator--(int) {
      TNode tmp = *this;
      --(*this);
      return tmp;
    }
    bool operator!=(const TListIterator& other) const {
      return node_ != other.node_;
    }
    bool operator==(const TListIterator& other) const {
      return !(*this != other);
    }
    pointer operator->() { return &(node_->value); }
    reference operator*() { return node_->value; }

   private:
    TNode* node_;
  };
  using iterator = TListIterator<false>;
  using const_iterator = TListIterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator begin() { return iterator(root_->next); }
  iterator end() { return iterator(root_); }
  const_iterator begin() const { return const_iterator(root_->next); }
  const_iterator end() const { return const_iterator(root_); }
  const_iterator cbegin() const { return const_iterator(root_->next); }
  const_iterator cend() const { return const_iterator(root_); }
  reverse_iterator rbegin() { return std::make_reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return std::make_reverse_iterator(end());
  }
  reverse_iterator rend() { return std::make_reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return std::make_reverse_iterator(begin());
  }
  const_reverse_iterator crbegin() const {
    return std::make_reverse_iterator(cend());
  }
  const_reverse_iterator crend() const {
    return std::make_reverse_iterator(cbegin());
  }

 private:
  TNode* root_;
  node_allocator allocator_;
  size_t size_ = 0;
};
