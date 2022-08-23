#ifndef NODE_H
#define NODE_H

#include <QObject>

class Node
{
public:
    int now;  // 现在的状态
    Node* pre;  // 指向亲节点
    int gx;  // 已付出代价  已走步数
    //        int hx;  // 估计还要付出代价

    Node(int n, Node* p, int step)
    {
        now = n;
        pre = p;
        gx = step;
        //            hx = value();
    }
    virtual ~Node() {};
};

class NodeM: public Node  // 曼哈顿距离A*
{
    int value(int goal)  // 计算每个点与正确点曼哈顿距离计算
    {
        int temp = 0;
        int a1[9], a2[9];
        int n = now;
        for (int i = 8; i >= 0; i--)
        {
            a1[n % 10] = i;
            a2[goal % 10] = i;
            goal /= 10;
            n /= 10;
        }
        for (int i = 0; i < 9; i++)
            temp += abs(a1[i] / 3 - a2[i] / 3) + abs(a1[i] % 3 - a2[i] % 3);
        return temp;
    }

public:
    int hx;  // 估计还要付出代价

    NodeM(int n, Node* p, int step, int g)
        : Node(n, p, step)
    {
        hx = value(g);
    }
    ~NodeM() {};
};

class NodeD: public Node  // 不同点个数A*
{
    int value(int goal)  // 计算不同点个数计算
    {
        int temp = 0;
        int n = now;
        for (int i = 0; i < 9; i++)
        {
            if (goal % 10 != n % 10)
                temp++;
            goal /= 10;
            n /= 10;
        }
        return temp;
    }

public:
    int hx;  // 估计还要付出代价

    NodeD(int n, Node* p, int step, int g)
        : Node(n, p, step)
    {
        hx = value(g);
    }
    ~NodeD() {};
};

#endif // NODE_H
