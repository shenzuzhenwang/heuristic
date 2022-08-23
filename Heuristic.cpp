#include "Heuristic.h"
#include "Node.h"
#include "ui_Heuristic.h"
#include <QMap>
#include <QQueue>
#include <QMessageBox>

Heuristic::Heuristic(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Heuristic)
{
    ui->setupUi(this);
}

Heuristic::~Heuristic()
{
    delete ui;
}

void Heuristic::Display(int num)  // 显示八数码的某个状态
{
    int c[3][3];
    int tt = num;
    for (int i = 2; i >= 0; i--)
        for (int j = 2; j >= 0; j--)
        {
            c[i][j] = tt % 10;
            tt /= 10;
        }  // 将数字转换为数组
    ui->num1->setText(QString("%1").arg(c[0][0]));  // 将每一位数字，显示在对应的框中
    ui->num2->setText(QString("%1").arg(c[0][1]));
    ui->num3->setText(QString("%1").arg(c[0][2]));
    ui->num4->setText(QString("%1").arg(c[1][0]));
    ui->num5->setText(QString("%1").arg(c[1][1]));
    ui->num6->setText(QString("%1").arg(c[1][2]));
    ui->num7->setText(QString("%1").arg(c[2][0]));
    ui->num8->setText(QString("%1").arg(c[2][1]));
    ui->num9->setText(QString("%1").arg(c[2][2]));
}

bool Heuristic::CheckInput()  // 检测输入是否符合规范
{
    QRegExp rx("[0-8]{9}");
    if (!rx.exactMatch(ui->edit_start->text().trimmed()))
    {
        QMessageBox::critical(this, "警告", "初始状态数字长度不符！！");
        return false;
    }
    int a[9] = {0};
    int t = ui->edit_start->text().toInt();
    for (int i = 0; i < 9; i++)
    {
        if (a[t % 10] > 0)
        {
            QMessageBox::critical(this, "警告", "初始状态数字重复！！");
            return false;
        }
        a[t % 10]++;
        t /= 10;
    }
    if (!rx.exactMatch(ui->edit_end->text().trimmed()))
    {
        QMessageBox::critical(this, "警告", "目标状态数字长度不符！！");
        return false;
    }
    int b[9] = {0};
    t = ui->edit_end->text().toInt();
    for (int i = 0; i < 9; i++)
    {
        if (b[t % 10] > 0)
        {
            QMessageBox::critical(this, "警告", "目标状态数字重复！！");
            return false;
        }
        b[t % 10]++;
        t /= 10;
    }
    return true;
}

QPair<int, int> Heuristic::BFS (int start, int end)  // 分支限界法
{
    int kx[4] = { 1, 0, 0, -1 };
    int ky[4] = { 0, 1, -1, 0 };

    QMap<int, int> m;  // 记录是否重复
    QQueue<Node*> open;
    QList<Node*> close;
    int sum_Node = 0;  // 记录搜索过的结点个数

    Node* p = new Node(start, nullptr, 0);
    open.push_back(p);
    m[start] = 1;  // 标记已用过
    while (!open.empty ())
    {
        Node* temp = open.front();
        open.pop_front();
        close.push_back(temp);

        if (temp->now == end)  // 如果到达目标结点
        {
            QVector<int> tt;
            Node* t = temp;
            do
            {
                tt.append(t->now);
                t = t->pre;
            }
            while (t);
            for (int i = tt.length() - 1; i >= 0; i--)
                path.append(tt[i]);  // 将沿着亲节点依次加入路径

            for (Node* t : open)
                delete t;
            for (Node* t : close)
                delete t;   // 将所有new的结点delete
            return QPair<int, int>(temp->gx, sum_Node);
        }

        int c[3][3];  //  数组表示法
        int tt = temp->now;  //  数字表示法
        int x = 0, y = 0;  //  0的位置
        for (int i = 2; i >= 0; i--)
            for (int j = 2; j >= 0; j--)
            {
                c[i][j] = tt % 10;
                tt /= 10;
                if (c[i][j] == 0)
                {
                    x = i;
                    y = j;
                }
            }  // 将数字变为数组
        sum_Node++;  // 搜索过的结点个数+1
        for (int i = 0; i < 4; i++)
        {
            if (x + kx[i] >= 0 && x + kx[i] < 3 && y + ky[i] >= 0 && y + ky[i] < 3)
            {
                qSwap (c[x][y], c[x + kx[i]][y + ky[i]]);
                int t = 0;
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                        t = t * 10 + c[i][j];  // 将数组变为数字
                if (m.count (t) == 0)  // 未重复
                {
                    m[t] = 1;
                    open.push_back(new Node(t, temp, temp->gx + 1));  // 将此结点加入open表
                }
                qSwap (c[x][y], c[x + kx[i]][y + ky[i]]);
            }
        }
    }
    for (auto t : close)
        delete t;
    return QPair<int, int>(-1, sum_Node);  // 未找到则返回-1
}

QPair<int, int> Heuristic::MHDistance(int start, int end)  // 曼哈顿距离A*
{
    int kx[4] = { 1, 0, 0, -1 };
    int ky[4] = { 0, 1, -1, 0 };

    QList<NodeM*> open;
    QList<NodeM*> close;
    int sum_Node = 0;

    NodeM* p = new NodeM(start, nullptr, 0, end);
    open.push_back(p);
    while (!open.empty ())
    {
        int min = 10000;
        int index = 0;
        for (int i = 0; i < open.size(); i++)
            if (open[i]->gx + open[i]->hx < min)
            {
                min = open[i]->gx + open[i]->hx;
                index = i;
            }  // 遍历open表，找到hx+gx最小的结点

        NodeM* temp = open[index];
        open.removeOne(temp);  // 将此结点从open表删除
        close.push_back(temp);  // 将此结点加入close表

        if (temp->now == end)  // 如果到达目标结点
        {
            QVector<int> tt;
            Node* t = temp;
            do
            {
                tt.append(t->now);
                t = t->pre;
            }
            while (t);
            for (int i = tt.length() - 1; i >= 0; i--)
                path.append(tt[i]);  // 将沿着亲节点依次加入路径
            int sum_step = temp->gx;  // 总步数

            for (NodeM* t : open)
                delete t;
            for (NodeM* t : close)
                delete t;  // 将所有new的结点delete
            return QPair<int, int>(sum_step, sum_Node);
        }

        int c[3][3];
        int tt = temp->now;
        int x = 0, y = 0;  //  0的位置
        for (int i = 2; i >= 0; i--)
            for (int j = 2; j >= 0; j--)
            {
                c[i][j] = tt % 10;
                tt /= 10;
                if (c[i][j] == 0)
                {
                    x = i;
                    y = j;
                }
            }  // 将数字变为数组
        sum_Node++;
        for (int i = 0; i < 4; i++)
        {
            if (x + kx[i] >= 0 && x + kx[i] < 3 && y + ky[i] >= 0 && y + ky[i] < 3)
            {
                qSwap (c[x][y], c[x + kx[i]][y + ky[i]]);
                int t = 0;
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                        t = t * 10 + c[i][j];  // 将数字变为数组

                bool in_close = false;  // 查看结点是否在close表中
                for (NodeM* j : close)
                    if (j->now == t)
                    {
                        in_close = true;
                        break;
                    }
                if (in_close == true)  // 如果在close表中，则丢弃此结点
                {
                    qSwap (c[x][y], c[x + kx[i]][y + ky[i]]);
                    continue;
                }

                NodeM* son = new NodeM(t, temp, temp->gx + 1, end);  // 扩展结点

                bool in_open = false;  // 查看扩展的结点是否在open表中
                NodeM* same = nullptr;
                for (NodeM* j : open)
                    if (j->now == t)
                    {
                        in_open = true;
                        same = j;
                        break;
                    }
                if (in_open == true)  // 如果在open表中
                {
                    if (same->gx > son->gx)  // 如果代价小，则更换结点
                    {
                        open.removeOne(same);
                        open.push_back(son);
                        delete same;
                    }
                    qSwap (c[x][y], c[x + kx[i]][y + ky[i]]);
                    continue;
                }

                open.push_back(son);
                qSwap (c[x][y], c[x + kx[i]][y + ky[i]]);
            }
        }
    }
    for (auto t : close)
        delete t;
    return QPair<int, int>(-1, sum_Node);  // 搜索失败，则返回-1
}

QPair<int, int> Heuristic::DiffPoint(int start, int end)  // 不同点个数A*
{
    int kx[4] = { 1, 0, 0, -1 };
    int ky[4] = { 0, 1, -1, 0 };

    QList<NodeD*> open;
    QList<NodeD*> close;
    int sum_Node = 0;

    NodeD* p = new NodeD(start, nullptr, 0, end);
    open.push_back(p);
    while (!open.empty ())
    {
        int min = 10000;
        int index = 0;
        for (int i = 0; i < open.size(); i++)
            if (open[i]->gx + open[i]->hx < min)
            {
                min = open[i]->gx + open[i]->hx;
                index = i;
            }  // 遍历open表，找到hx+gx最小的结点

        NodeD* temp = open[index];
        open.removeOne(temp);
        close.push_back(temp);

        if (temp->now == end)  // 如果到达目标结点
        {
            QVector<int> tt;
            Node* t = temp;
            do
            {
                tt.append(t->now);
                t = t->pre;
            }
            while (t);
            for (int i = tt.length() - 1; i >= 0; i--)
                path.append(tt[i]);  // 将沿着亲节点依次加入路径
            int sum_step = temp->gx;

            for (NodeD* t : open)
                delete t;
            for (NodeD* t : close)
                delete t;  // 将所有new的结点delete
            return QPair<int, int>(sum_step, sum_Node);
        }

        int c[3][3];
        int tt = temp->now;
        int x = 0, y = 0;  //  0的位置
        for (int i = 2; i >= 0; i--)
            for (int j = 2; j >= 0; j--)
            {
                c[i][j] = tt % 10;
                tt /= 10;
                if (c[i][j] == 0)
                {
                    x = i;
                    y = j;
                }
            }  // 将数字变为数组
        sum_Node++;
        for (int i = 0; i < 4; i++)
        {
            if (x + kx[i] >= 0 && x + kx[i] < 3 && y + ky[i] >= 0 && y + ky[i] < 3)
            {
                qSwap (c[x][y], c[x + kx[i]][y + ky[i]]);
                int t = 0;
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                        t = t * 10 + c[i][j];  // 将数字变为数组

                bool in_close = false;  // 查看结点是否在close表中
                for (NodeD* j : close)
                    if (j->now == t)
                    {
                        in_close = true;
                        break;
                    }
                if (in_close == true)  // 如果在close表中，则丢弃此结点
                {
                    qSwap (c[x][y], c[x + kx[i]][y + ky[i]]);
                    continue;
                }

                NodeD* son = new NodeD(t, temp, temp->gx + 1, end);  // 扩展结点

                bool in_open = false;  // 查看是否在open表中
                NodeD* same = nullptr;
                for (NodeD* j : open)
                    if (j->now == t)
                    {
                        in_open = true;
                        same = j;
                        break;
                    }
                if (in_open == true)  // 如果在open表中
                {
                    if (same->gx > son->gx)  // 如果代价小，则更换结点
                    {
                        open.removeOne(same);
                        open.push_back(son);
                        delete same;
                    }
                    qSwap (c[x][y], c[x + kx[i]][y + ky[i]]);
                    continue;
                }

                open.push_back(son);
                qSwap (c[x][y], c[x + kx[i]][y + ky[i]]);
            }
        }
    }
    for (auto t : close)
        delete t;
    return QPair<int, int>(-1, sum_Node);  // 搜索失败，则返回-1
}

bool CheckEnable(int start, int end)  // 查看是否可达
{
    int start1[8], end1[8];
    int rev1 = 0, rev2 = 0;
    for (int i = 0; i < 8; i++)  // 计算初始结点的逆序值
    {
        if (start % 10 == 0)
            start /= 10;
        start1[i] = start % 10;
        start /= 10;
        for (int j = 0; j < i; j++)
            if (start1[j] > start1[i])
                rev1++;
    }

    for (int i = 0; i < 8; i++)  // 计算目标结点的逆序值
    {
        if (end % 10 == 0)
            end /= 10;
        end1[i] = end % 10;
        end /= 10;
        for (int j = 0; j < i; j++)
            if (end1[j] > end1[i])
                rev2++;
    }
    if (rev1 % 2 != rev2 % 2)  // 如果逆序值不相等，则不可达
        return false;
    return true;  // 否则可达
}

void Heuristic::on_btn_search_clicked()
{
    if (CheckInput())  // 检测输入是否规范
    {
        QPair<int, int> ans;

        start = ui->edit_start->text().toInt();
        end = ui->edit_end->text().toInt();
        path.clear();

        if (!CheckEnable(start, end))  // 检测为不可达
        {
            sum_step = -1;
            ui->lab_sum_step->setText("不可达");
            ui->lab_sum_node->setNum(-1);
        }
        else
        {
            if (mode == 1)
            {
                ans = BFS(start, end);  // 分支限界法
            }
            if (mode == 2)
            {
                ans = MHDistance(start, end);  // 曼哈顿距离的A*
            }
            if (mode == 3)
            {
                ans = DiffPoint(start, end);  // 不同点个数的A*
            }
            sum_step = ans.first;
            ui->lab_sum_step->setText(QString("共%1步").arg(ans.first));
            ui->lab_sum_node->setNum(ans.second);
        }

        step = 0;
        if (sum_step != -1)
        {
            Display(path[step]);
            ui->lab_step->setText(QString("第%1步").arg(step));
            ButtonState();
        }
        else
        {
            Display(start);
            ui->lab_step->setText(QString("第%1步").arg(step));
            ui->btn_next->setEnabled(false);
            ui->btn_pre->setEnabled(false);
        }
    }
}

void Heuristic::on_radioButton_clicked()  // 更改搜索方法
{
    mode = 1;  // 分支限界法
}

void Heuristic::on_radioButton_2_clicked()
{
    mode = 2;  // 曼哈顿距离的A*
}

void Heuristic::on_radioButton_3_clicked()
{
    mode = 3;  // 不同点个数的A*
}

void Heuristic::ButtonState()  // 改变按键状态
{
    if (step == 0)
        ui->btn_pre->setEnabled(false);
    else
        ui->btn_pre->setEnabled(true);
    if (step == sum_step)
        ui->btn_next->setEnabled(false);
    else
        ui->btn_next->setEnabled(true);
}

void Heuristic::on_btn_pre_clicked()  // 上一步按钮
{
    step--;
    Display(path[step]);
    ui->lab_step->setText(QString("第%1步").arg(step));
    ButtonState();
}

void Heuristic::on_btn_next_clicked()  // 下一步按钮
{
    step++;
    Display(path[step]);
    ui->lab_step->setText(QString("第%1步").arg(step));
    ButtonState();
}
