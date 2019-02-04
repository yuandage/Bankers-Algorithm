#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTableView>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    //初始化
    void init();
    void on_pushButton_add_clicked();
    void on_pushButton_addProcess_clicked();
    void on_pushButton_delete_clicked();
    void on_pushButton_deleteProcess_clicked();
    void on_pushButton_start_clicked();
    void check_init();
    void show_init();
    int safe();
    int changdata(int processNum);
    void on_pushButton_check_clicked();
    void on_pushButton_request_clicked();
    void on_pushButton_startAllocation_clicked();
    //退出系统
    void on_pushButton_exit_clicked();
    void on_pushButton_exit1_clicked();
    void on_pushButton_exit3_clicked();
    void on_pushButton_exit2_clicked();
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    int resourceCount; //资源数量
    int ProcessCount; //进程数量
    QList<int> Available; //可用资源
    QList<int> temp;
    QList<QList<int>> Max;//最大需求
    QList<QList<int>> Allocation;//已分配资源
    QList<QList<int>> Need;//需求资源
    QList<int> Request;//请求资源
    QStandardItemModel *model_init; //model_init
    QTableView *table_init;
    QStandardItemModel *model_resourcesAllocation;   //model_resourcesAllocation
    QTableView *table_resourcesAllocation;
    QStandardItemModel *model_safeCheck; //model_safeCheck
    QTableView *table_safeCheck;
    QStandardItemModel *model_request;  //model_request
    QTableView *table_request;
};

#endif // MAINWINDOW_H
