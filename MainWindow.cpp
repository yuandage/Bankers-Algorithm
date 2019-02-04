#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model_init=new QStandardItemModel(this);
    table_init=ui->tableView_init;
    model_resourcesAllocation=new QStandardItemModel(this);
    table_resourcesAllocation=ui->tableView_resourcesAllocation;
    model_safeCheck=new QStandardItemModel(this);
    table_safeCheck=ui->tableView_safeCheck;
    model_request=new QStandardItemModel(this);
    table_request=ui->tableView_request;
    resourceCount=3;
    ProcessCount=5;
    //初始化
    init();
    //列宽自动适应
    table_init->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_resourcesAllocation->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_safeCheck->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    //资源分配初始化;
    ui->stackedWidget->setCurrentWidget(ui->page_init);
    model_init->setRowCount(ProcessCount+3);//8行
    model_init->setColumnCount(resourceCount*2+1);//7列

    //初始值
//    model_init->setItem(2,1,new QStandardItem(QString::number(10)));
//    model_init->setItem(2,2,new QStandardItem(QString::number(5)));
//    model_init->setItem(2,3,new QStandardItem(QString::number(7)));
//    //1
//    model_init->setItem(3,1,new QStandardItem(QString::number(7)));
//    model_init->setItem(3,2,new QStandardItem(QString::number(5)));
//    model_init->setItem(3,3,new QStandardItem(QString::number(3)));
//    model_init->setItem(3,4,new QStandardItem(QString::number(0)));
//    model_init->setItem(3,5,new QStandardItem(QString::number(1)));
//    model_init->setItem(3,6,new QStandardItem(QString::number(0)));
//    //2
//    model_init->setItem(4,1,new QStandardItem(QString::number(3)));
//    model_init->setItem(4,2,new QStandardItem(QString::number(2)));
//    model_init->setItem(4,3,new QStandardItem(QString::number(2)));
//    model_init->setItem(4,4,new QStandardItem(QString::number(2)));
//    model_init->setItem(4,5,new QStandardItem(QString::number(0)));
//    model_init->setItem(4,6,new QStandardItem(QString::number(0)));
//    //3
//    model_init->setItem(5,1,new QStandardItem(QString::number(9)));
//    model_init->setItem(5,2,new QStandardItem(QString::number(0)));
//    model_init->setItem(5,3,new QStandardItem(QString::number(2)));
//    model_init->setItem(5,4,new QStandardItem(QString::number(3)));
//    model_init->setItem(5,5,new QStandardItem(QString::number(0)));
//    model_init->setItem(5,6,new QStandardItem(QString::number(2)));
//    //4
//    model_init->setItem(6,1,new QStandardItem(QString::number(2)));
//    model_init->setItem(6,2,new QStandardItem(QString::number(2)));
//    model_init->setItem(6,3,new QStandardItem(QString::number(2)));
//    model_init->setItem(6,4,new QStandardItem(QString::number(2)));
//    model_init->setItem(6,5,new QStandardItem(QString::number(1)));
//    model_init->setItem(6,6,new QStandardItem(QString::number(1)));
//    //5
//    model_init->setItem(7,1,new QStandardItem(QString::number(4)));
//    model_init->setItem(7,2,new QStandardItem(QString::number(3)));
//    model_init->setItem(7,3,new QStandardItem(QString::number(3)));
//    model_init->setItem(7,4,new QStandardItem(QString::number(0)));
//    model_init->setItem(7,5,new QStandardItem(QString::number(0)));
//    model_init->setItem(7,6,new QStandardItem(QString::number(2)));

    model_init->setItem(0,0,new QStandardItem("分配情况"));
    model_init->setItem(1,0,new QStandardItem("资源名称"));
    model_init->setItem(2,0,new QStandardItem("资源数量"));
    for(int i=0;i<ProcessCount;i++)
        model_init->setItem(i+3,0,new QStandardItem(QString("P%1").arg(i)));
    for(int i=0;i<resourceCount;i++){
        model_init->setItem(0,i+1,new QStandardItem("Max"));
        model_init->setItem(0,i+1+resourceCount,new QStandardItem("Allocation"));
        model_init->setItem(1,i+1,new QStandardItem(QString('A'+i)));//A B C
        model_init->setItem(1,i+1+resourceCount,new QStandardItem(QString('A'+i)));
    }
    //第0列不能编辑
    for(int i = 0; i<model_init->rowCount();i++)
        model_init->item(i,0)->setEditable(false);
    //第0,1行不能编辑
    for(int i=0;i<2;i++)
        for(int j=0;j<model_init->columnCount();j++)
            model_init->item(i,j)->setEditable(false);

    table_init->setModel(model_init);

}

void MainWindow::on_pushButton_add_clicked()
{
    if(resourceCount>25)
        QMessageBox::warning(this,"提示","本系统最大资源种类为26!","确定");
    else{
        resourceCount++;
        init();
    }
}

void MainWindow::on_pushButton_addProcess_clicked()
{
    ProcessCount++;
    init();
}

void MainWindow::on_pushButton_delete_clicked()
{
    if(resourceCount<3)
        QMessageBox::warning(this,"提示","本系统最少资源种类为2!","确定");
    else{
        resourceCount--;
        init();
    }
}

void MainWindow::on_pushButton_deleteProcess_clicked()
{
    if(ProcessCount<3)
        QMessageBox::warning(this,"提示","本系统最少进程种类为2!","确定");
    else{
        ProcessCount--;
        init();
    }
}


void MainWindow::on_pushButton_start_clicked()
{
    bool maxFlag=true;
    bool AllFlag=true;
    bool avail1Flag=true;
    bool availFlag=true;
    Available.clear();
    Max.clear();
    Allocation.clear();
    Need.clear();

    //可用的初始资源数量
    for(int i=0;i<resourceCount;i++)
        Available<<model_init->data(model_init->index(2,i+1)).toInt();
    for(int i=0;i<resourceCount;i++)
        if(Available.at(i)<=0)
            avail1Flag=false;
    for(int i=0;i<ProcessCount;i++){
        QList<int> list;
        for(int j=0;j<resourceCount;j++){
            list<<model_init->data(model_init->index(i+3,j+1)).toInt();
        }
        Max<<list;
    }
    for(int i=0;i<ProcessCount;i++){
        for(int j=0;j<resourceCount;j++){
            if(Max.at(i).at(j)>Available.at(j)){
                maxFlag=false;
                break;
            }
        }
    }
    if(avail1Flag==false)
        QMessageBox::warning(this,"提示","资源数量必须大于零!请重新输入!","确定");
    if(maxFlag==false)
        QMessageBox::warning(this,"提示","进程最大需求数量大于资源数量!请重新输入!","确定");
    for(int i=0;i<ProcessCount;i++){
        QList<int> list;
        for(int j=0;j<resourceCount;j++){
            list<<model_init->data(model_init->index(i+3,j+1+resourceCount)).toInt();
        }
        Allocation<<list;
    }
    for(int i=0;i<ProcessCount;i++){
        for(int j=0;j<resourceCount;j++){
            if(Allocation.at(i).at(j)>Max.at(i).at(j)){
                AllFlag=false;
                break;
            }
        }
    }
    for(int i=0;i<ProcessCount;i++){
        for(int j=0;j<resourceCount;j++){
            Available[j]=Available.at(j)-Allocation.at(i).at(j);
            if(Available.at(j)<0)
                availFlag=false;
        }
    }
    if(availFlag==false)
        QMessageBox::warning(this,"提示","进程已分配数量大于资源数量!请重新输入!","确定");
    if(AllFlag==false)
        QMessageBox::warning(this,"提示","进程已分配数量大于最大需求数量!请重新输入!","确定");
    //Need[][]
    for(int i=0;i<ProcessCount;i++){
        QList<int> list;
        for(int j=0;j<resourceCount;j++){
            list<<Max.at(i).at(j)-Allocation.at(i).at(j);
        }
        Need<<list;
    }
    if(maxFlag&&AllFlag&&availFlag&&avail1Flag)
        show_init();
}

void MainWindow::show_init()
{
    model_resourcesAllocation->setRowCount(ProcessCount+2);//7行
    model_resourcesAllocation->setColumnCount(resourceCount*4+1);//13列
    model_resourcesAllocation->setItem(0,0,new QStandardItem("分配情况"));
    model_resourcesAllocation->setItem(1,0,new QStandardItem("资源名称"));
    for(int i=0;i<ProcessCount;i++)
        model_resourcesAllocation->setItem(i+2,0,new QStandardItem(QString("P%1").arg(i)));
    for(int i=0;i<resourceCount;i++){
        model_resourcesAllocation->setItem(0,i+1,new QStandardItem("Max"));
        model_resourcesAllocation->setItem(0,i+1+resourceCount,new QStandardItem("Allocation"));
        model_resourcesAllocation->setItem(0,i+1+resourceCount*2,new QStandardItem("Need"));
        model_resourcesAllocation->setItem(0,i+1+resourceCount*3,new QStandardItem("Available"));
        model_resourcesAllocation->setItem(1,i+1,new QStandardItem(QString('A'+i)));//A B C
        model_resourcesAllocation->setItem(1,i+1+resourceCount,new QStandardItem(QString('A'+i)));
        model_resourcesAllocation->setItem(1,i+1+resourceCount*2,new QStandardItem(QString('A'+i)));
        model_resourcesAllocation->setItem(1,i+1+resourceCount*3,new QStandardItem(QString('A'+i)));
    }
    for(int i=0;i<ProcessCount;i++){
        for(int j=0;j<resourceCount*4;j++){
            if(j<resourceCount)
                model_resourcesAllocation->setItem(i+2,j+1,new QStandardItem(QString::number(Max.at(i).at(j))));
            if(j>=resourceCount&&j<resourceCount*2)
                model_resourcesAllocation->setItem(i+2,j+1,new QStandardItem(QString::number(Allocation.at(i).at(j-resourceCount))));
            if(j>=resourceCount*2&&j<resourceCount*3)
                model_resourcesAllocation->setItem(i+2,j+1,new QStandardItem(QString::number(Need.at(i).at(j-resourceCount*2))));
            if(j>=resourceCount*3&&i==0)
                model_resourcesAllocation->setItem(i+2,j+1,new QStandardItem(QString::number(Available.at(j-resourceCount*3))));
        }
    }
    table_resourcesAllocation->setModel(model_resourcesAllocation);
    table_resourcesAllocation->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->stackedWidget->setCurrentWidget(ui->page_resourcesAllocation);
}

void MainWindow::on_pushButton_check_clicked()
{
    check_init();
    safe();
}

void MainWindow::check_init()
{
    model_safeCheck->setRowCount(ProcessCount+2);//7行
    model_safeCheck->setColumnCount(resourceCount*4+2);//14列
    model_safeCheck->setItem(0,0,new QStandardItem("分配情况"));
    model_safeCheck->setItem(1,0,new QStandardItem("资源名称"));
    //最后一列
    model_safeCheck->setItem(0,1+resourceCount*4,new QStandardItem("Finish"));
    for(int i=0;i<ProcessCount;i++)
        model_safeCheck->setItem(i+2,0,new QStandardItem(QString("P%1").arg(i)));
    for(int i=0;i<resourceCount;i++){
        model_safeCheck->setItem(0,i+1,new QStandardItem("Work"));
        model_safeCheck->setItem(0,i+1+resourceCount,new QStandardItem("Allocation"));
        model_safeCheck->setItem(0,i+1+resourceCount*2,new QStandardItem("Need"));
        model_safeCheck->setItem(0,i+1+resourceCount*3,new QStandardItem("Work+Allocation"));
        model_safeCheck->setItem(1,i+1,new QStandardItem(QString('A'+i)));//A B C
        model_safeCheck->setItem(1,i+1+resourceCount,new QStandardItem(QString('A'+i)));
        model_safeCheck->setItem(1,i+1+resourceCount*2,new QStandardItem(QString('A'+i)));
        model_safeCheck->setItem(1,i+1+resourceCount*3,new QStandardItem(QString('A'+i)));
    }
    //不能编辑
    table_safeCheck->setModel(model_safeCheck);
}

//资源分配情况检查
int MainWindow::safe()
{
    table_safeCheck->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->stackedWidget->setCurrentWidget(ui->page_safeCheck);
    QList<int> Work;
    Work.clear();
    temp.clear();
    int i, k = 0, m, apply;
    int j;
    int flag = 0;
    QList<int> Finish;
    for (i = 0; i<resourceCount; i++)
        Work<<Available.at(i);

    for (i = 0; i<ProcessCount; i++){
        Finish<<0;
        apply = 0;
        for (j = 0; j<resourceCount; j++){
            if (Finish[i] == 0&&Need[i][j] <= Work[j]){
                apply++;
                if (apply == resourceCount){
                    for(int x=0;x<resourceCount*4;x++){
                        if(x<resourceCount)
                            model_safeCheck->setItem(flag+2,x+1,new QStandardItem(QString::number(Work.at(x))));
                        if(x>=resourceCount&&x<resourceCount*2)
                            model_safeCheck->setItem(flag+2,x+1,new QStandardItem(QString::number(Allocation.at(i).at(x-resourceCount))));
                        if(x>=resourceCount*2&&x<resourceCount*3)
                            model_safeCheck->setItem(flag+2,x+1,new QStandardItem(QString::number(Need.at(i).at(x-resourceCount*2))));
                    }
                    //work+allocation
                    for (m = 0; m<resourceCount; m++){
                        Work[m] = Work.at(m) + Allocation.at(i).at(m);
                        model_safeCheck->setItem(flag+2,m+1+resourceCount*3,new QStandardItem(QString::number(Work.at(m))));
                    }
                    //变分配数
                    Finish[i] = 1;
                    temp<<i;
                    model_safeCheck->setItem(flag+2,1+resourceCount*4,new QStandardItem("True"));
                    //qDebug()<< "True" ;
                    i = -1;
                    flag++;
                }
            }
        }
    }

    for(int x=0;x<ProcessCount;x++){
        //qDebug()<<"Finish:"<<Finish[x];
        if(Finish[x]==0){
            k=1;
        }
    }

    if(k==1){
        model_safeCheck->clear();
        model_safeCheck->setRowCount(ProcessCount+2);//7行
        model_safeCheck->setColumnCount(resourceCount*3+1);//10列
        model_safeCheck->setItem(0,0,new QStandardItem("分配情况"));
        model_safeCheck->setItem(1,0,new QStandardItem("资源名称"));
        for(int i=0;i<ProcessCount;i++)
            model_safeCheck->setItem(i+2,0,new QStandardItem(QString("P%1").arg(i)));
        for(int i=0;i<resourceCount;i++){
            model_safeCheck->setItem(0,i+1,new QStandardItem("Allocation"));
            model_safeCheck->setItem(0,i+1+resourceCount,new QStandardItem("Need"));
            model_safeCheck->setItem(0,i+1+resourceCount*2,new QStandardItem("Available"));
            model_safeCheck->setItem(1,i+1,new QStandardItem(QString('A'+i)));
            model_safeCheck->setItem(1,i+1+resourceCount,new QStandardItem(QString('A'+i)));
            model_safeCheck->setItem(1,i+1+resourceCount*2,new QStandardItem(QString('A'+i)));
        }
        for(int i=0;i<ProcessCount;i++)
            for(int j=0;j<resourceCount*3+1;j++){
                if(j<resourceCount)
                    model_safeCheck->setItem(i+2,j+1,new QStandardItem(QString::number(Allocation.at(i).at(j))));
                if(j>=resourceCount&&j<resourceCount*2)
                    model_safeCheck->setItem(i+2,j+1,new QStandardItem(QString::number(Need.at(i).at(j-resourceCount))));
                if(j>=resourceCount*2&&j<resourceCount*3)
                    model_safeCheck->setItem(2,j+1,new QStandardItem(QString::number(Available.at(j-resourceCount*2))));
            }
        ui->label_safe->setText("系统进入不安全状态！此时系统不分配资源！");
        //qDebug() << endl << "系统进入不安全状态！此时系统不分配资源！"<<endl;//不成功系统不安全
        return 0;
    }

    //如果安全，输出成功
    QString str;
    for (i = 0; i<ProcessCount; i++){//输出运行进程数组
        str+= QString("P%1->").arg(temp.at(i));
        model_safeCheck->setItem(i+2,0,new QStandardItem(QString("P%1").arg(temp.at(i))));
    }
    ui->label_safe->setText("此时系统是安全的!安全序列为:\n"+str);
    return 0;
}

//资源请求
void MainWindow::on_pushButton_request_clicked()
{
    ui->comboBox->clear();
    model_request->setRowCount(3);//3行
    model_request->setColumnCount(resourceCount+1);//4列
    model_request->setItem(0,0,new QStandardItem("请求资源"));
    model_request->setItem(1,0,new QStandardItem("资源名称"));
    model_request->setItem(2,0,new QStandardItem(QString("P0")));
    for(int i=0;i<resourceCount;i++){
        model_request->setItem(0,i+1,new QStandardItem("Request"));
        model_request->setItem(1,i+1,new QStandardItem(QString('A'+i)));//A B C
    }
    for(int i=0;i<ProcessCount;i++)
        ui->comboBox->addItem(QString("P%1").arg(i));
    //第0,1行不能编辑
    for(int i=0;i<2;i++)
        for(int j=0;j<model_request->columnCount();j++)
            model_request->item(i,j)->setEditable(false);
    table_request->setModel(model_request);
    ui->stackedWidget->setCurrentWidget(ui->page_request);
}


void MainWindow::on_pushButton_startAllocation_clicked()
{
    bool requestFlag=true;
    Request.clear();
    //请求资源的下标
    int processNum=ui->comboBox->currentIndex();
    for(int i=0;i<resourceCount;i++)
        Request<<model_request->data(model_request->index(2,i+1)).toInt();
    for(int i=0;i<resourceCount;i++){
        if(Request.at(i)<0){
            requestFlag=false;
            break;
        }
    }

    if(requestFlag==false)
        QMessageBox::warning(this,"提示","请求资源不能为负数!请重新输入!","确定");
    for (int j = 0; j<resourceCount; j++){
        //判断申请是否大于需求，若大于则出错
        if (Request[j]>Need[processNum][j]){
            requestFlag=false;
            QMessageBox::warning(this,"提示","申请的资源数量大于它需要的资源数量!分配不合理!\n请重新输入!","确定");
            break;
        } else {//判断申请是否大于当前资源，若大于则出错
            if (Request[j]>Available[j]){
                QMessageBox::warning(this,"提示","申请的资源数量大于系统现在可利用的资源数量!分配出错!\n请重新输入!","确定");
                requestFlag=false;
                break;
            }
        }
    }
    if (requestFlag) {
        changdata(processNum);//根据进程需求量变换资源
        safe();//根据进程需求量进行银行家算法判断
    }
    //qDebug()<<"processNum:"<<processNum<<"Request:"<<Request;

}

int MainWindow::changdata(int processNum)//进行资源分配
{
    for (int i = 0; i<resourceCount; i++) {
        Available[i] = Available[i] - Request[i];
        Allocation[processNum][i] = Allocation[processNum][i] + Request[i];
        Need[processNum][i] = Need[processNum][i] - Request[i];
    }
    return 1;
}

void MainWindow::on_pushButton_exit_clicked()
{
    int ret=QMessageBox::question(this,"退出?","你真的想退出本系统吗?","确定","我在想想");
    if(ret==0)
        close();
}

void MainWindow::on_pushButton_exit1_clicked()
{
    on_pushButton_exit_clicked();
}

void MainWindow::on_pushButton_exit3_clicked()
{
    on_pushButton_exit_clicked();
}

void MainWindow::on_pushButton_exit2_clicked()
{
    on_pushButton_exit_clicked();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    model_request->setItem(2,0,new QStandardItem(QString("P%1").arg(index)));
}
