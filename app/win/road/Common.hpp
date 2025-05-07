#pragma once

#include <QString>

constexpr long long NOT_COMPLETE = -1;

// 道路部署时的数据
struct PoleRoadData
{
    long long completeDateTime; // 完成时间
    QString slot; // 编号
    float peg; // 桩号
    bool isLeft; // 是否面向左侧行车
};