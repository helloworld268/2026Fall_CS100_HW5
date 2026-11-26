#ifndef DYNARRAY_HPP
#define DYNARRAY_HPP 1

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <ostream>
// Problem 3 (Dynamic Array 2.0) builds on your Problem 2 Dynarray.
// Start from your finished p2.cc, then add everything required by the
// handout:
//   - public type alias members (size_type, value_type, pointer, reference,
//     const_pointer, const_reference)
//   - subscript operator a[i] (like at(i), but no bounds checking)
//   - the six relational operators (lexicographical comparison,
//     no loops/recursions allowed)
//   - operator<< printing "[e1, e2, ...]"
class Dynarray
{
  // TODO: your Problem 2 implementation + the new members above.
public:
  using size_type = std::size_t;
  using value_type = int;
  using pointer = int *;
  using reference = int &;
  using const_pointer = const int *;
  using const_reference = const int &;

private:
  size_type size_;
  pointer data_;

public:
  Dynarray() : size_(0), data_(nullptr) {}
  size_type size() const
  {
    return size_;
  }
  bool empty() const
  {
    return size_ == 0;
  }
  explicit Dynarray(size_type n) : size_(n), data_(n == 0 ? nullptr : new value_type[n]{}) {}
  ~Dynarray()
  {
    delete[] data_;
  }
  Dynarray(size_type n, value_type x) : size_(n), data_(n == 0 ? nullptr : new value_type[n])
  {
    for (size_type i = 0; i < n; ++i)
    {
      data_[i] = x;
    }
  }
  reference at(size_type index)
  {
    if (index >= size_)
    {
      throw std::out_of_range{"Dynarray index out of range!"};
    }
    return data_[index];
  }
  const_reference at(size_type index) const
  {
    if (index >= size_)
    {
      throw std::out_of_range{"Dynarray index out of range!"};
    }
    return data_[index];
  }
  Dynarray(const_pointer begin, const_pointer end)
      : size_(begin == end ? 0 : static_cast<size_type>(end - begin)),
        data_(size_ == 0 ? nullptr : new value_type[size_]{})
  {
    for (size_type i = 0; i < size_; ++i)
    {
      data_[i] = begin[i];
    }
  }
  Dynarray(const Dynarray &other) : size_(other.size_), data_(size_ == 0 ? nullptr : new value_type[size_]{})
  {
    for (size_type i = 0; i < size_; ++i)
    {
      data_[i] = other.data_[i];
    }
  }
  Dynarray &operator=(const Dynarray &other)
  {
    if (this == &other)
    {
      return *this;
    }
    pointer new_data = (other.size_ == 0 ? nullptr : new value_type[other.size_]);
    for (size_type i = 0; i < other.size_; ++i)
    {
      new_data[i] = other.data_[i];
    }
    delete[] data_;
    data_ = new_data;
    size_ = other.size_;
    return *this;
  }
  Dynarray(Dynarray &&other) noexcept
      : size_(other.size_), data_(other.data_)
  {
    other.size_ = 0;
    other.data_ = nullptr;
  }
  Dynarray &operator=(Dynarray &&other) noexcept
  {
    if (this == &other)
    {
      return *this;
    }
    delete[] data_;
    size_ = other.size_;
    data_ = other.data_;
    other.size_ = 0;
    other.data_ = nullptr;
    return *this;
  }
  reference operator[](size_type index)
  {
    return data_[index];
  }
  const_reference operator[](size_type index) const
  {
    return data_[index];
  }
  bool operator==(const Dynarray &other) const
  {
    if (size_ != other.size_)
    {
      return false;
    }

    if (size_ == 0)
    {
      return true;
    }

    return std::equal(data_, data_ + size_, other.data_);
  }
  bool operator<(const Dynarray &other) const
  {
    if (size_ == 0)
    {
      return other.size_ > 0;
    }
    if (other.size_ == 0)
    {
      return false;
    }
    return std::lexicographical_compare(data_, data_ + size_, other.data_, other.data_ + other.size_);
  }
  bool operator!=(const Dynarray &other) const
  {
    return !(*this == other);
  }
  bool operator>(const Dynarray &other) const
  {
    return (other < *this);
  }
  bool operator<=(const Dynarray &other) const
  {
    return !(*this > other);
  }
  bool operator>=(const Dynarray &other) const
  {
    return !(*this < other);
  }
};
inline std::ostream &operator<<(std::ostream &os, const Dynarray &a)
{
  os << '[';
  for (Dynarray::size_type i = 0; i < a.size(); ++i)
  {
    if (i != 0)
    {
      os << ", ";
    }
    os << a[i];
  }
  os << ']';
  return os;
}
#endif // DYNARRAY_HPP
