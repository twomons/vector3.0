#pragma once
#include <iostream>
#include <cstring>
#include <assert.h>

template<class T>
class Iterator
{
    T* x_data;

public:

    Iterator(T* ptr) : x_data(ptr) {}
    T& operator*() const { return *x_data; }
    Iterator& operator++() { ++x_data; return *this; }
    bool operator!=(const Iterator& it) const { return x_data != it.x_data; }
    bool operator==(const Iterator& it) const { return x_data == it.x_data; }

    // 重载"+"运算符
    Iterator operator+(int n) const { return Iterator(x_data + n); }
};

template<typename T>
class Vector
{
    T* x_data;      // 数据
    size_t x_size;  // 个数
    size_t x_mem;   // 内存大小

    //递归赋值 自定义元素
    template<typename...ARGS>
    void Recfunc(T first, ARGS...args) {
        x_data[x_size++] = first;
        Recfunc(args...);
    }
    //结束递归
    void Recfunc() {}

public:
    using iterator = Iterator<T>;

    // 构造函数
    //这里改造失败会报错,没有使用 placement new 运算符来进行对象的初始化。可以使用 placement new 运算符来在已分配的内存上直接调用类型 T 的构造函数进行初始化。
    //**************************一处不足*************************
    /*
    这里我原本直接用的 ，并无报错异常
    X data(new T[source.X size]f) {什么也不写}
    后面有人说“构造函数初始化列表中仅仅使用 new，没有保证异常安全; 应该使用 operator new 和 placement new 进行内存分配和初始化，否则造成不必要的构造函数调用”*/
    Vector() : x_data(nullptr), x_size(0), x_mem(0) {}

    Vector(size_t size) : x_data(static_cast<T*>(operator new(size * sizeof(T)))), x_size(size), x_mem(size * sizeof(T))
    {
        for (size_t i = 0; i < x_size; ++i)
        {
            new (&x_data[i]) T();
        }
    }

    Vector(size_t count, T data) : x_data(static_cast<T*>(operator new(count * sizeof(T)))), x_size(count), x_mem(count * sizeof(T))
    {
        for (size_t i = 0; i < x_size; ++i)
        {
            new (&x_data[i]) T(data);
        }
    }

    Vector(const Vector& source) : x_data(static_cast<T*>(operator new(source.x_size * sizeof(T)))), x_size(source.x_size), x_mem(source.x_size * sizeof(T))
    {
        for (size_t i = 0; i < x_size; ++i)
        {
            new (&x_data[i]) T(source.x_data[i]);
        }
    }

    Vector(std::initializer_list<T> list) : x_data(static_cast<T*>(operator new(list.size() * sizeof(T)))), x_size(list.size()), x_mem(list.size() * sizeof(T))
    {
        size_t i = 0;
        for (const T& element : list)
        {
            new (&x_data[i++]) T(element);
        }
    }

    template<typename...ARGS>
    Vector(ARGS...args) : x_data(static_cast<T*>(operator new(sizeof...(args) * sizeof(T)))), x_size(sizeof...(args)), x_mem(sizeof...(args) * sizeof(T))
    {
        T arr[] = { args... };
        for (size_t i = 0; i < x_size; ++i)
        {
            new (&x_data[i]) T(arr[i]);
        }
    }

    // 析构函数    // 构造函数
    //**************************一处不足*************************
    /*
    这里我原本直接用的 ，并无报错异常
     X data(new T[source.X size]f) {什么也不写}
    后面有人说“构造函数初始化列表中仅仅使用 new，没有保证异常安全; 应该使用 operator new 和 placement new 进行内存分配和初始化，否则造成不必要的构造函数调用”*/
    virtual ~Vector()
    {
        for (size_t i = 0; i < x_size; ++i)
        {
            x_data[i].~T();
        }
        operator delete(x_data);
        x_data = nullptr;
    }

    // 赋值运算符重载函数
    Vector& operator=(const Vector& source)
    {
        if (this == &source)
            return *this;

        // 释放原有资源
        for (size_t i = 0; i < x_size; ++i)
        {
            x_data[i].~T();
        }
        operator delete(x_data);

        // 分配新的资源
        x_data = static_cast<T*>(operator new(source.x_size * sizeof(T)));
        x_size = source.x_size;
        x_mem = source.x_size * sizeof(T);

        for (size_t i = 0; i < x_size; ++i)
        {
            new (&x_data[i]) T(source.x_data[i]);
        }

        return *this;
    }

    // []运算符重载函数（用于取出vector实例化对象的第index个元素）
    T& operator[](size_t index)
    {
        assert(index < x_size);
        return x_data[index];
    }

    const T& operator[](size_t index) const
    {
        assert(index < x_size);
        return x_data[index];
    }

    // ==运算符重载函数（用于判断两个vector对象是否相等）
    bool operator==(const Vector& other) const
    {
        if (x_size != other.x_size)
            return false;

        for (size_t i = 0; i < x_size; ++i)
        {
            if (x_data[i] != other.x_data[i])
                return false;
        }

        return true;
    }

    // 向vector示例化对象增添元素
    void push_back(const T& element)
    {
        if (x_size == x_mem)
        {
            // 扩展内存
            size_t new_mem = (x_mem == 0) ? 1 : x_mem * 2;
            T* new_data = static_cast<T*>(operator new(new_mem * sizeof(T)));

            for (size_t i = 0; i < x_size; ++i)
            {
                new (&new_data[i]) T(x_data[i]);
                x_data[i].~T();
            }

            operator delete(x_data);

            x_data = new_data;
            x_mem = new_mem;
        }

        new (&x_data[x_size++]) T(element);
    }

    // 删除vector实例化对象最后一个元素
    void pop_back()
    {
        assert(x_size > 0);
        x_data[--x_size].~T();
    }

    // 向vector实例化对象的it位置插入一个元素
    iterator insert(iterator it, const T& element)
    {
        size_t index = it - begin();
        assert(index <= x_size);

        if (x_size == x_mem)
        {
            // 扩展内存
            size_t new_mem = (x_mem == 0) ? 1 : x_mem * 2;
            T* new_data = static_cast<T*>(operator new(new_mem * sizeof(T)));

            for (size_t i = 0; i < x_size; ++i)
            {
                if (i < index)
                {
                    new (&new_data[i]) T(x_data[i]);
                    x_data[i].~T();
                }
                else if (i >= index)
                {
                    new (&new_data[i + 1]) T(x_data[i]);
                    x_data[i].~T();
                }
            }

            operator delete(x_data);

            x_data = new_data;
            x_mem = new_mem;
        }
        else
        {
            for (size_t i = x_size; i > index; --i)
            {
                new (&x_data[i]) T(x_data[i - 1]);
                x_data[i - 1].~T();
            }
        }

        new (&x_data[index]) T(element);
        ++x_size;

        return iterator(&x_data[index]);
    }

    // 删除vector实例化对象it位置的元素
    iterator erase(iterator it)
    {
        size_t index = it - begin();
        assert(index < x_size);

        for (size_t i = index; i < x_size - 1; ++i)
        {
            x_data[i].~T();
            new (&x_data[i]) T(x_data[i + 1]);
        }

        x_data[--x_size].~T();

        return iterator(&x_data[index]);
    }

    // 取出vector对象中的第一个元素
    T& front()
    {
        assert(x_size > 0);
        return x_data[0];
    }

    const T& front() const
    {
        assert(x_size > 0);
        return x_data[0];
    }

    // 取出vector对象中的最后一个元素
    T& back()
    {
        assert(x_size > 0);
        return x_data[x_size - 1];
    }

    const T& back() const
    {
        assert(x_size > 0);
        return x_data[x_size - 1];
    }

    // 获取vector实例化对象的元素首地址
    T* data()
    {
        return x_data;
    }

    const T* data() const
    {
        return x_data;
    }

    // 获取vector实例化对象的最后一个元素的下一个地址
    iterator end()
    {
        return iterator(x_data + x_size);
    }

    // 获取vector实例化对象的元素个数
    size_t size() const
    {
        return x_size;
    }

    // 获取vector实例化对象的容量
    size_t capacity() const
    {
        return x_mem / sizeof(T);
    }

    // 判断vector实例化对象是否为空
    bool empty() const
    {
        return x_size == 0;
    }

    // 清空vector实例化对象
    void clear()
    {
        for (size_t i = 0; i < x_size; ++i)
        {
            x_data[i].~T();
        }

        x_size = 0;
    }

    // 交换两个vector实例化对象
    void swap(Vector& other)
    {
        std::swap(x_data, other.x_data);
        std::swap(x_size, other.x_size);
        std::swap(x_mem, other.x_mem);
    }

    // 获取vector实例化对象的起始迭代器
    iterator begin()
    {
        return iterator(x_data);
    }
};