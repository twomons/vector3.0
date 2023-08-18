#include <iostream>
#include "Vector.h"

//说实话这是我第一次手搓容器，磕磕碰碰凑了几天，这个主函数是我仿照vector调用模板改动的
//里面还是有很大问题，模板根本就是画虎成猫，特别是编vector时候要崩溃了“( { ”看的眼花缭乱、
//我怀疑是我cmake配置有问题，一直拆不对，淦！！！

int main()
{
    // 创建一个空的Vector对象
    Vector<int> vec1;

    // 添加元素到Vector对象
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);

    // 使用下标运算符访问元素
    std::cout << "vec1[0]: " << vec1[0] << std::endl;
    std::cout << "vec1[1]: " << vec1[1] << std::endl;
    std::cout << "vec1[2]: " << vec1[2] << std::endl;

    // 使用迭代器遍历Vector对象
    std::cout << "vec1: ";
    for (Vector<int>::iterator it = vec1.begin(); it != vec1.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 创建一个带有初始值的Vector对象
    Vector<int> vec2(5, 10);
    std::cout << "vec2: ";
    for (Vector<int>::iterator it = vec2.begin(); it != vec2.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 复制Vector对象
    Vector<int> vec3 = vec1;
    std::cout << "vec3: ";
    for (Vector<int>::iterator it = vec3.begin(); it != vec3.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 判断两个Vector对象是否相等
    if (vec1 == vec3)
    {
        std::cout << "vec1 and vec3 are equal." << std::endl;
    }
    else
    {
        std::cout << "vec1 and vec3 are not equal." << std::endl;
    }

    // 清空Vector对象
    vec1.clear();
    std::cout << "vec1 size after clear: " << vec1.size() << std::endl;

    return 0;
}