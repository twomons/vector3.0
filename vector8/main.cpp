#include <iostream>
#include "Vector.h"

//˵ʵ�������ҵ�һ���ִ��������Ŀ��������˼��죬������������ҷ���vector����ģ��Ķ���
//���滹���кܴ����⣬ģ��������ǻ�����è���ر��Ǳ�vectorʱ��Ҫ�����ˡ�( { �������ۻ����ҡ�
//�һ�������cmake���������⣬һֱ�𲻶ԣ��ƣ�����

int main()
{
    // ����һ���յ�Vector����
    Vector<int> vec1;

    // ���Ԫ�ص�Vector����
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);

    // ʹ���±����������Ԫ��
    std::cout << "vec1[0]: " << vec1[0] << std::endl;
    std::cout << "vec1[1]: " << vec1[1] << std::endl;
    std::cout << "vec1[2]: " << vec1[2] << std::endl;

    // ʹ�õ���������Vector����
    std::cout << "vec1: ";
    for (Vector<int>::iterator it = vec1.begin(); it != vec1.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // ����һ�����г�ʼֵ��Vector����
    Vector<int> vec2(5, 10);
    std::cout << "vec2: ";
    for (Vector<int>::iterator it = vec2.begin(); it != vec2.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // ����Vector����
    Vector<int> vec3 = vec1;
    std::cout << "vec3: ";
    for (Vector<int>::iterator it = vec3.begin(); it != vec3.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // �ж�����Vector�����Ƿ����
    if (vec1 == vec3)
    {
        std::cout << "vec1 and vec3 are equal." << std::endl;
    }
    else
    {
        std::cout << "vec1 and vec3 are not equal." << std::endl;
    }

    // ���Vector����
    vec1.clear();
    std::cout << "vec1 size after clear: " << vec1.size() << std::endl;

    return 0;
}