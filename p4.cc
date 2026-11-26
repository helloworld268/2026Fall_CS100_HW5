#ifndef SMART_POINTERS_HPP
#define SMART_POINTERS_HPP 1

#include <cstddef>
#include <utility>

// Problem 4 (Smart Pointers): implement UniquePtr<T> and SharedPtr<T>.
// See the handout for the full requirements:
//
// UniquePtr<T>: exclusive ownership.
//   - default ctor (owns nothing), ctor from T*, destructor deletes
//   - copy ctor/assignment = delete; move ctor/assignment noexcept,
//     self-assignment safe, source becomes "null"
//   - *p, *cp (const), p->, explicit operator bool, get()
//   - reset(T* = nullptr), release()
//
// SharedPtr<T>: shared ownership via reference counting.
//   - default ctor (owns nothing), ctor from T* (count starts at 1)
//   - copy ctor/assignment increment the counter; copy-assignment must be
//     self-assignment safe (increment before decrement!)
//   - move ctor/assignment steal the pointer (noexcept), source becomes empty
//   - destructor decrements; destroys the object exactly once at zero
//   - *p, p->, explicit operator bool, get(), use_count()
//
// Do NOT include <memory>.

template <typename T>
class UniquePtr
{
  // TODO: implement (see the handout).
private:
  T *ptr_;

public:
  UniquePtr() : ptr_(nullptr) {}
  explicit UniquePtr(T *p) : ptr_(p) {}
  ~UniquePtr()
  {
    delete ptr_;
  }
  UniquePtr(const UniquePtr &other) = delete;
  UniquePtr &operator=(const UniquePtr &other) = delete;
  UniquePtr(UniquePtr &&other) noexcept
      : ptr_(other.ptr_)
  {
    other.ptr_ = nullptr;
  }
  UniquePtr &operator=(UniquePtr &&other) noexcept
  {
    if (this == &other)
    {
      return *this;
    }

    delete ptr_;
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
    return *this;
  }
  T &operator*()
  {
    return *ptr_;
  }
  const T &operator*() const
  {
    return *ptr_;
  }
  T *operator->()
  {
    return ptr_;
  }
  const T *operator->() const
  {
    return ptr_;
  }
  T *get() const
  {
    return ptr_;
  }
  explicit operator bool() const
  {
    return ptr_ != nullptr;
  }
  T *release()
  {
    T *old = ptr_;
    ptr_ = nullptr;
    return old;
  }
  void reset(T *p = nullptr)
  {
    if (ptr_ == p)
    {
      return;
    }
    delete ptr_;
    ptr_ = p;
  }
};

template <typename T>
class SharedPtr
{
  // TODO: implement (see the handout).
  // Hint: a control block holding { T *ptr; std::size_t count; } works well.
private:
  struct ControlBlock
  {
    T *ptr;
    std::size_t count;
  };
  ControlBlock *control_;
  void release_current()
  {
    if (control_ == nullptr)
    {
      return;
    }

    --control_->count;

    if (control_->count == 0)
    {
      delete control_->ptr;
      delete control_;
    }

    control_ = nullptr;
  }

public:
  SharedPtr() : control_(nullptr) {}
  explicit SharedPtr(T *p) : control_(nullptr)
  {
    if (p != nullptr)
    {
      control_ = new ControlBlock{p, 1};
    }
  }
  std::size_t use_count() const
  {
    if (control_ == nullptr)
    {
      return 0;
    }
    return control_->count;
  }
  SharedPtr(const SharedPtr &other)
      : control_(other.control_)
  {
    if (control_ != nullptr)
    {
      ++control_->count;
    }
  }
  ~SharedPtr()
  {
    release_current();
  }
  SharedPtr &operator=(const SharedPtr &other)
  {
    if (this == &other)
    {
      return *this;
    }
    release_current();
    control_ = other.control_;
    if (control_ != nullptr)
    {
      ++control_->count;
    }
    return *this;
  }
  SharedPtr(SharedPtr &&other) noexcept : control_(other.control_)
  {
    other.control_ = nullptr;
  }
  SharedPtr &operator=(SharedPtr &&other) noexcept
  {
    if (this == &other)
    {
      return *this;
    }
    release_current();
    control_ = other.control_;
    other.control_ = nullptr;
    return *this;
  }
  T *get() const
  {
    if (control_ == nullptr)
    {
      return nullptr;
    }
    return control_->ptr;
  }
  T &operator*()
  {
    return *control_->ptr;
  }
  const T &operator*() const
  {
    return *control_->ptr;
  }
  T *operator->()
  {
    return get();
  }
  const T *operator->() const
  {
    return get();
  }
  explicit operator bool() const
  {
    return get() != nullptr;
  }
};

#endif // SMART_POINTERS_HPP
