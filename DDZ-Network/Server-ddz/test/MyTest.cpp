//
// Created by mushan on 2025/12/4.
//

#include "MyTest.h"
#include "Person.pb.h"
#include <iostream>

void MyTest::test()
{
    // 序列化
    Person p;
    p.set_id(10);
    p.set_age(32);
    p.set_sex("man");
    p.set_name("lucy");

    // 序列化对象p, 最终得到一个字符串
    std::string output;
    p.SerializeToString(&output);

    // 反序列化数据
    Person pp;
    pp.ParseFromString(output);
    std::cout << pp.id() << ", " << pp.name() << ", " << pp.sex() << ", " << pp.age() << std::endl;
}
