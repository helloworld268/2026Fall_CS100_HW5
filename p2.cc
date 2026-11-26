#ifndef DYNARRAY_HPP
#define DYNARRAY_HPP 1

#include <cstddef>
#include <stdexcept>

class Dynarray
{
  // TODO: Implement this class as required by the handout (HW5 Problem 2).
  // See the handout for the full list of requirements:
  //   - five constructors (default, (n), (n, x), (begin, end), copy, move)
  //   - copy/move assignment operators, destructor
  //   - size(), empty(), at(n) (with bounds checking)
private:
  std::size_t size_;
  int *data_;

public:
  Dynarray() : size_(0), data_(nullptr) {}
  std::size_t size() const
  {
    return size_;
  }
  bool empty() const
  {
    return size_ == 0;
  }
  explicit Dynarray(std::size_t n) : size_(n), data_(n == 0 ? nullptr : new int[n]{}) {}
  ~Dynarray()
  {
    delete[] data_;
  }
  Dynarray(std::size_t n, int x) : size_(n), data_(n == 0 ? nullptr : new int[n])
  {
    for (std::size_t i = 0; i < n; ++i)
    {
      data_[i] = x;
    }
  }
  int &at(std::size_t index)
  {
    if (index >= size_)
    {
      throw std::out_of_range{"Dynarray index out of range!"};
    }
    return data_[index];
  }
  const int &at(std::size_t index) const
  {
    if (index >= size_)
    {
      throw std::out_of_range{"Dynarray index out of range!"};
    }
    return data_[index];
  }
  Dynarray(const int *begin, const int *end)
      : size_(begin == end ? 0 : static_cast<std::size_t>(end - begin)),
        data_(size_ == 0 ? nullptr : new int[size_]{})
  {
    for (std::size_t i = 0; i < size_; ++i)
    {
      data_[i] = begin[i];
    }
  }
  Dynarray(const Dynarray &other) : size_(other.size_), data_(size_ == 0 ? nullptr : new int[size_]{})
  {
    for (std::size_t i = 0; i < size_; ++i)
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
    int *new_data = (other.size_ == 0 ? nullptr : new int[other.size_]);
    for (std::size_t i = 0; i < other.size_; ++i)
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
};

#endif // DYNARRAY_HPP
