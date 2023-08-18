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

    // ����"+"�����
    Iterator operator+(int n) const { return Iterator(x_data + n); }
};

template<typename T>
class Vector
{
    T* x_data;      // ����
    size_t x_size;  // ����
    size_t x_mem;   // �ڴ��С

    //�ݹ鸳ֵ �Զ���Ԫ��
    template<typename...ARGS>
    void Recfunc(T first, ARGS...args) {
        x_data[x_size++] = first;
        Recfunc(args...);
    }
    //�����ݹ�
    void Recfunc() {}

public:
    using iterator = Iterator<T>;

    // ���캯��
    //�������ʧ�ܻᱨ��,û��ʹ�� placement new ����������ж���ĳ�ʼ��������ʹ�� placement new ����������ѷ�����ڴ���ֱ�ӵ������� T �Ĺ��캯�����г�ʼ����
    //**************************һ������*************************
    /*
    ������ԭ��ֱ���õ� �����ޱ����쳣
    X data(new T[source.X size]f) {ʲôҲ��д}
    ��������˵�����캯����ʼ���б��н���ʹ�� new��û�б�֤�쳣��ȫ; Ӧ��ʹ�� operator new �� placement new �����ڴ����ͳ�ʼ����������ɲ���Ҫ�Ĺ��캯�����á�*/
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

    // ��������    // ���캯��
    //**************************һ������*************************
    /*
    ������ԭ��ֱ���õ� �����ޱ����쳣
     X data(new T[source.X size]f) {ʲôҲ��д}
    ��������˵�����캯����ʼ���б��н���ʹ�� new��û�б�֤�쳣��ȫ; Ӧ��ʹ�� operator new �� placement new �����ڴ����ͳ�ʼ����������ɲ���Ҫ�Ĺ��캯�����á�*/
    virtual ~Vector()
    {
        for (size_t i = 0; i < x_size; ++i)
        {
            x_data[i].~T();
        }
        operator delete(x_data);
        x_data = nullptr;
    }

    // ��ֵ��������غ���
    Vector& operator=(const Vector& source)
    {
        if (this == &source)
            return *this;

        // �ͷ�ԭ����Դ
        for (size_t i = 0; i < x_size; ++i)
        {
            x_data[i].~T();
        }
        operator delete(x_data);

        // �����µ���Դ
        x_data = static_cast<T*>(operator new(source.x_size * sizeof(T)));
        x_size = source.x_size;
        x_mem = source.x_size * sizeof(T);

        for (size_t i = 0; i < x_size; ++i)
        {
            new (&x_data[i]) T(source.x_data[i]);
        }

        return *this;
    }

    // []��������غ���������ȡ��vectorʵ��������ĵ�index��Ԫ�أ�
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

    // ==��������غ����������ж�����vector�����Ƿ���ȣ�
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

    // ��vectorʾ������������Ԫ��
    void push_back(const T& element)
    {
        if (x_size == x_mem)
        {
            // ��չ�ڴ�
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

    // ɾ��vectorʵ�����������һ��Ԫ��
    void pop_back()
    {
        assert(x_size > 0);
        x_data[--x_size].~T();
    }

    // ��vectorʵ���������itλ�ò���һ��Ԫ��
    iterator insert(iterator it, const T& element)
    {
        size_t index = it - begin();
        assert(index <= x_size);

        if (x_size == x_mem)
        {
            // ��չ�ڴ�
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

    // ɾ��vectorʵ��������itλ�õ�Ԫ��
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

    // ȡ��vector�����еĵ�һ��Ԫ��
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

    // ȡ��vector�����е����һ��Ԫ��
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

    // ��ȡvectorʵ���������Ԫ���׵�ַ
    T* data()
    {
        return x_data;
    }

    const T* data() const
    {
        return x_data;
    }

    // ��ȡvectorʵ������������һ��Ԫ�ص���һ����ַ
    iterator end()
    {
        return iterator(x_data + x_size);
    }

    // ��ȡvectorʵ���������Ԫ�ظ���
    size_t size() const
    {
        return x_size;
    }

    // ��ȡvectorʵ�������������
    size_t capacity() const
    {
        return x_mem / sizeof(T);
    }

    // �ж�vectorʵ���������Ƿ�Ϊ��
    bool empty() const
    {
        return x_size == 0;
    }

    // ���vectorʵ��������
    void clear()
    {
        for (size_t i = 0; i < x_size; ++i)
        {
            x_data[i].~T();
        }

        x_size = 0;
    }

    // ��������vectorʵ��������
    void swap(Vector& other)
    {
        std::swap(x_data, other.x_data);
        std::swap(x_size, other.x_size);
        std::swap(x_mem, other.x_mem);
    }

    // ��ȡvectorʵ�����������ʼ������
    iterator begin()
    {
        return iterator(x_data);
    }
};