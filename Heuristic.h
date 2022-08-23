#ifndef HEURISTIC_H
#define HEURISTIC_H

#include <QMainWindow>
#include <QPair>

QT_BEGIN_NAMESPACE
namespace Ui
{
class Heuristic;
}
QT_END_NAMESPACE

class Heuristic : public QMainWindow
{
    Q_OBJECT

public:
    Heuristic(QWidget *parent = nullptr);
    ~Heuristic();

private slots:
    void on_btn_search_clicked();  // 搜索按钮

    void on_radioButton_clicked();  // 更改搜索方法--分支限界法

    void on_radioButton_2_clicked();  // 更改搜索方法--曼哈顿距离的A*

    void on_radioButton_3_clicked();  // 更改搜索方法--不同点个数的A*

    void on_btn_pre_clicked();  // 上一步按钮

    void on_btn_next_clicked();  // 下一步按钮

private:
    Ui::Heuristic *ui;

    int mode = 1;  // 搜索策略  1分支限界法  2曼哈顿距离的A*  3不同点个数的A*
    int step = 0;  // 正在演示的步数
    int sum_step = 0;  // 起点到终点的步数

    QVector<int> path;  // 从起点到目标路径

    bool CheckInput();  // 检测输入是否符合规范
    void ButtonState();  // 改变按键状态

    int start = 0;  // 初始状态
    int end = 0;  // 目标状态

    QPair<int, int> BFS(int start, int end);  // 分支限界法
    QPair<int, int> MHDistance(int start, int end);  // 曼哈顿距离A*
    QPair<int, int> DiffPoint(int start, int end);  // 不同点个数A*

    void Display(int num);  // 显示八数码的某个状态

};
#endif // HEURISTIC_H
