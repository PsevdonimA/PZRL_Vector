#include <iostream>
#include <string.h>
#include "Vector.h"

using ValueType = double;

Vector::Vector(const ValueType* rawArray, const size_t size, float coef)
{
    if (rawArray != nullptr)
    {
        _data = new ValueType[size];
        memcpy(_data, rawArray, size * sizeof(ValueType));
        _size = size;
        _capacity = size;
    }
    else
    {
        _data = nullptr;
        _size = 0;
        _capacity = 0;
    }
    _multiplicativeCoef = coef;
}

Vector::Vector(const Vector& other)
{
    _size = other._size;
    _capacity = _size;
    _multiplicativeCoef = other._multiplicativeCoef;
    _data = new ValueType[_capacity];
    memcpy(_data, other._data, _size * sizeof(ValueType));
}

Vector& Vector::operator=(const Vector& other)
{
    if (this == &other)
    {
        return *this;
    }
    delete[] _data;
    _size = other._size;
    _capacity = _size;
    _multiplicativeCoef = other._multiplicativeCoef;
    _data = new ValueType[_capacity];
    memcpy(_data, other._data, _size * sizeof(ValueType));
    return *this;
}

Vector::Vector(Vector&& other) noexcept
{
    _data = other._data;
    _size = other._size;
    _capacity = other._capacity;
    _multiplicativeCoef = other._multiplicativeCoef;
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}

Vector& Vector::operator=(Vector&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }
    delete[] _data;
    _data = other._data;
    _size = other._size;
    _capacity = other._capacity;
    _multiplicativeCoef = other._multiplicativeCoef;
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
    return *this;
}

Vector::~Vector()
{
    delete[] _data;
}

void Vector::pushBack(const ValueType& value)
{
    resize(_size + 1);
    insert(value, _size);
}

void Vector::pushFront(const ValueType& value)
{
    resize(_size + 1);
    insert(value, 0);
}

void Vector::insert(const ValueType& value, size_t pos)
{
    if (pos > _size)
    {
        throw std::invalid_argument("Cannot insert value: position must be lesser than size.");
    }
    else if (pos < 0)
    {
        throw std::invalid_argument("Cannot insert value: position cannot be negative.");
    }
    resize(_size + 1);
    for (size_t i = _size; i > pos; i--)
    {
        _data[i] = _data[i-1];
    }
    _data[pos] = value;
    _size++;
}

void Vector::insert(const ValueType* values, size_t size, size_t pos)
{
    if (size <= 0)
    {
        throw std::invalid_argument("Cannot insert values: length of array must be greater than 0.");
    }
    else if (pos > _size)
    {
        throw std::invalid_argument("Cannot insert values: position must be lesser than size.");
    }
    else if (pos < 0)
    {
        throw std::invalid_argument("Cannot insert values: position cannot be negative.");
    }
    else if (values == nullptr)
    {
        throw std::invalid_argument("Cannot insert values: pointer must not be null.");
    }
    resize(_size + size);
    for (int i = size - 1; i >= 0; i--)
    {
        insert(values[i], pos);
    }
}

void Vector::insert(const Vector& vector, size_t pos)
{
    if (pos > _size)
    {
        throw std::invalid_argument("Cannot insert vector: position must be lesser than size.");
    }
    else if (pos < 0)
    {
        throw std::invalid_argument("Cannot insert vector: position cannot be negative.");
    }
    insert(vector._data, vector._size, pos);
}

void Vector::popBack()
{
    erase(_size - 1);
}

void Vector::popFront()
{
    erase(0);
}

void Vector::erase(size_t pos, size_t count)
{
    if (pos >= _size)
    {
        throw std::invalid_argument("Cannot erase element: position must be lesser than size.");
    }
    else if (pos < 0)
    {
        throw std::invalid_argument("Cannot erase element: position cannot be negative.");
    }
    else if (count < 0)
    {
        throw std::invalid_argument("Cannot erase elements: count must be positive.");
    }
    size_t delta = count;
    if (pos + delta > _size)
    {
        delta = _size - pos;
    }
    for (size_t i = pos; i < _size; i++)
    {
        if (i + delta >= _size)
        {
            break;
        }
        else
        {
            _data[i] = _data[i + delta];
        }
    }
    _size -= delta;
}

void Vector::eraseBetween(size_t beginPos, size_t endPos)
{
    if (beginPos < 0 || beginPos >= endPos)
    {
        throw std::invalid_argument("Cannot erase elements: positions incorrect.");
    }
    erase(beginPos, endPos - beginPos);
}

size_t Vector::size() const
{
    return _size;
}

size_t Vector::capacity() const
{
    return _capacity;
}

double Vector::loadFactor() const
{
    return (double)_multiplicativeCoef;
}

ValueType& Vector::operator[](size_t idx)
{
    if (_size == 0)
    {
        throw std::invalid_argument("Cannot get element: size of vector is 0.");
    }
    size_t _idx = idx;
    _idx = _idx % _size;
    return _data[_idx];
}

const ValueType& Vector::operator[](size_t idx) const
{
    if (_size == 0)
    {
        throw std::invalid_argument("Cannot get element: size of vector is 0.");
    }
    size_t _idx = idx;
    _idx = _idx % _size;
    return _data[_idx];
}

long long Vector::find(const ValueType& value) const
{
    long long idx = -1;
    for (size_t i = 0; i < _size; i++)
    {
        if (_data[i] == value)
        {
            idx = (long long)i;
            break;
        }
    }
    return idx;
}

void Vector::reserve(size_t capacity)
{
    if (capacity > _capacity)
    {
        _capacity = capacity;
        ValueType* data = new ValueType[_capacity];
        memcpy(data, _data, _size * sizeof(ValueType));
        delete[] _data;
        _data = data;
    }
}

void Vector::shrinkToFit()
{
    if (_size < _capacity)
    {
        _capacity = _size;
        ValueType* data = new ValueType[_capacity];
        memcpy(data, _data, _size * sizeof(ValueType));
        delete[] _data;
        _data = data;
    }
}

Vector::Iterator::Iterator(ValueType* ptr)
{
    _ptr = ptr;
}

ValueType& Vector::Iterator::operator*()
{
    if (_ptr == nullptr)
    {
        throw std::invalid_argument("Iterator is nullptr.");
    }
    return *_ptr;
}

const ValueType& Vector::Iterator::operator*() const
{
    if (_ptr == nullptr)
    {
        throw std::invalid_argument("Iterator is nullptr.");
    }
    return *_ptr;
}

ValueType* Vector::Iterator::operator->()
{
    return _ptr;
}

const ValueType* Vector::Iterator::operator->() const
{
    return _ptr;
}

Vector::Iterator Vector::Iterator::operator++()
{
    _ptr++;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int)
{
    _ptr++;
    return *this;
}

bool Vector::Iterator::operator==(const Iterator& other) const
{
    return (_ptr == other._ptr);
}

bool Vector::Iterator::operator!=(const Iterator& other) const
{
    return (_ptr != other._ptr);
}

Vector::Iterator Vector::begin()
{
    Iterator it(_data);
    return it;
}

Vector::Iterator Vector::end()
{
    Iterator it(_data);
    for (int i = 0; i < _size; i++)
    {
        it++;
    }
    return it;
}

void Vector::resize(size_t capacity)
{
    if (capacity > _capacity)
    {
        if (_capacity == 0)
        {
            _capacity = (size_t)_multiplicativeCoef;
        }
        size_t delta_cap = 0;
        while (_capacity < capacity)
        {
            delta_cap = (size_t)((float)_capacity * _multiplicativeCoef);
            if (delta_cap > _capacity)
            {
                _capacity = delta_cap;
            }
            else
            {
                _capacity++;
            }
        }
        ValueType* data = new ValueType[_capacity];
        memcpy(data, _data, _size * sizeof(ValueType));
        delete[] _data;
        _data = data;
    }
}
