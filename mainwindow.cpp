#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QtMath>
#include <QMenu>
#include <QMenuBar>
#include <QTextStream>
#include <QMessageBox>

QPainter painter;
QPen pen_1;
QPen pen_axes(QPen(QColor(0, 0, 0), 2, Qt::SolidLine));
QPen pen_background(QPen(QColor(144, 189, 209), 1, Qt::DashLine));
QPen pen_2(QPen(QColor(255, 0 ,0),2,Qt::SolidLine));
double h = 50;
QString Function;
double left_board;
double right_board;
double step = 1;
const double Pi = acos(-1);
QPoint OO(640,480);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setFixedSize(1280, 960);
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DrawCoorinateAxes()
{
    int W = width(); // Ширина и высота окна
    int H = height();

    painter.translate(OO); // новое начало кординат
    painter.setPen(pen_axes);
    painter.drawLine(0, -H, 0,H);
    painter.drawLine(-W,0,W,0);

}

void MainWindow::DrawValueScale()
{
    int W = width(); // Ширина и высота окна
    int H = height();

    painter.setFont(QFont("Arial",-1,-1, false));
    painter.drawText(10, 20,  QString::number(0));
    double counter = step;
    for (int i = h*step; i < W; i+= h*step)
    {
        painter.setPen(pen_background);
        painter.drawLine(i, -H , i, H);
        painter.setPen(pen_axes);
        painter.drawLine(i, 3, i, -3);
        painter.drawText(i-4, 20 ,  QString::number(counter));
        counter += step;
    }
    counter = -step;
    for (int i = -h*step; i > -W; i-= h*step)
    {
        painter.setPen(pen_background);
        painter.drawLine(i, -H , i, H);
        painter.setPen(pen_axes);
        painter.drawLine(i, 3, i, -3);
        painter.drawText(i-6, 20 ,  QString::number(counter));
        counter -= step;
    }
    counter = -step;
    for (int i = h*step; i < H; i+= h*step)
    {
        painter.setPen(pen_background);
        painter.drawLine(W, i , -W, i);
        painter.setPen(pen_axes);
        painter.drawLine(3, i, -3, i);
        painter.drawText(7, i + 3 ,  QString::number(counter));
        counter -= step;
    }
    counter = step;
    for (int i = -h*step; i > -H; i-= h*step)
    {
        painter.setPen(pen_background);
        painter.drawLine(W, i , -W, i);
        painter.setPen(pen_axes);
        painter.drawLine(3, i, -3, i);
        painter.drawText(7, i - 7 , 20, 20, Qt::AlignCenter, QString::number(counter));
        counter += step;
    }
}
double MainWindow::Execute(double x)
{
    while (true) {
        QString str = ui->func->text();
        std::string stri = str.toStdString();
        std::stringstream sstr;
        for (int i = 0; i < stri.length(); i++)
        {
            if (stri[i] == 'x')
            {sstr << x;} else {sstr << stri[i];}
        }

        char Ch; //Переменная, в которую будет записываться текущий обрабатываемый символ
        double value;
        bool flag = true; //Нужен для того, чтобы программа смогла отличить унарный минус (-5) от вычитания (2-5)
        stack<MainWindow::Leksema> Stack_n; //Стек с числами
        stack<MainWindow::Leksema> Stack_o; //Стек с операциями
        MainWindow::Leksema item; //Объект типа Leksema
        while (true) {
            Ch = sstr.peek(); //Смотрим на первый символ
            if (Ch == '\377')break; //Если достигнут конец строки, выходим из цикла
            if (Ch == ' ') { //Игнорирование пробелов
                sstr.ignore();
                continue;
            }
            if (Ch == 's' || Ch == 'c' || Ch == 't' || Ch == 'E') { //Если прочитана функция
                char foo[3]; //массив на 3 символа для определения типа прочитанной функции
                for (int i = 0; i < 3; i++) {
                    Ch = sstr.peek();
                    foo[i] = Ch;
                    sstr.ignore();
                }
                if (foo[0] == 's' && foo[1] == 'i' && foo[2] == 'n') { //Если прочитанная функция - синус
                    item.type = 's';
                    item.value = 0;
                    Stack_o.push(item); //Операция кладется в стек с операциями
                    continue;
                }
                if (foo[0] == 'c' && foo[1] == 'o' && foo[2] == 's') { //Если прочитанная функция - косинус
                    item.type = 'c';
                    item.value = 0;
                    Stack_o.push(item); //Операция кладется в стек с операциями
                    continue;
                }
                if (foo[0] == 't' && foo[1] == 'a' && foo[2] == 'n') { //Если прочитанная функция - тангенс
                    item.type = 't';
                    item.value = 0;
                    Stack_o.push(item); //Операция кладется в стек с операциями
                    continue;
                }
                if (foo[0] == 'c' && foo[1] == 't' && foo[2] == 'g') { //Если прочитанная функция - котангенс
                    item.type = 'g';
                    item.value = 0;
                    Stack_o.push(item); //Операция кладется в стек с операциями
                    continue;
                }
                if (foo[0] == 'E' && foo[1] == 'X' && foo[2] == 'P') { //Если прочитанная функция - экспонента
                    item.type = 'E';
                    item.value = 0;
                    Stack_o.push(item); //Операция кладется в стек с операциями
                    continue;
                }
            }
            if (Ch == 'p') { //Если прочитано число Пи
                item.type = '0';
                item.value = Pi;
                Stack_n.push(item); //Число кладется в стек с числами
                flag = 0;
                sstr.ignore();
                continue;
            }
            if ((Ch >= '0' && Ch <= '9') || (Ch == '-' && flag == 1)) { //Если прочитано число
                sstr >> value;
                item.type = '0';
                item.value = value;
                Stack_n.push(item); //Число кладется в стек с числами
                flag = 0;
                continue;
            }
            if (Ch == '+' || (Ch == '-' && flag == 0) || Ch == '*' || Ch == '/' || Ch == '^') { //Если прочитана операция
                if (Stack_o.size() == 0) { //Если стек с операциями пуст
                    item.type = Ch;
                    item.value = 0;
                    Stack_o.push(item); //Операция кладется в стек с операциями
                    sstr.ignore();
                    continue;
                }
                if (Stack_o.size() != 0 && getRang(Ch) >
                    getRang(Stack_o.top().type)) { //Если стек с операциями НЕ пуст, но приоритет текущей операции выше верхней в стеке с операциями
                    item.type = Ch;
                    item.value = 0;
                    Stack_o.push(item); //Операция кладется в стек с операциями
                    sstr.ignore();
                    continue;
                }
                if (Stack_o.size() != 0 && getRang(Ch) <=
                    getRang(Stack_o.top().type)) {//Если стек с операциями НЕ пуст, но приоритет текущей операции ниже либо равен верхней в стеке с операциями
                    if (Maths(Stack_n, Stack_o, item) == false) { //Если функция вернет "false", то прекращаем работу
                        system("pause");
                        return 0;
                    }
                    continue;
                }
            }
            if (Ch == '(') { //Если прочитана открывающаяся скобка
                item.type = Ch;
                item.value = 0;
                Stack_o.push(item); //Операция кладется в стек с операциями
                sstr.ignore();
                continue;
            }
            if (Ch == ')') { //Если прочитана закрывающаяся скобка
                while (Stack_o.top().type != '(') {
                    if (Maths(Stack_n, Stack_o, item) == false) { //Если функция вернет "false", то прекращаем работу
                        system("pause");
                        return 0;
                    }
                    else continue; //Если все хорошо
                }
                Stack_o.pop();
                sstr.ignore();
                continue;
            }
            else { //Если прочитан какой-то странный символ
                ui->lineEdit->setText("\nНеверно введено выражение!");
                return 0;
            }
        }
        while (Stack_o.size() !=
            0) { //Вызываем матем. функцию до тех пор, пока в стеке с операциями не будет 0 элементов
            if (Maths(Stack_n, Stack_o, item) == false) { //Если функция вернет "false", то прекращаем работу
                return 0;
            }
            else continue; //Если все хорошо
        }
        return Stack_n.top().value; //Выводим ответ

    }
}

void MainWindow::DrawFunc()
{

    double x;
    for (double i = left_board*h; i < right_board*h; i += 0.01)
    {
     QPointF c;
     c.rx() = i/h;
     x = c.rx();
     c.ry() = -Execute(x);
     painter.drawPoint(QPointF(c.rx()*h, c.ry()*h));
    }
}

void MainWindow::paintEvent(QPaintEvent *) {
    painter.begin(this);
    DrawCoorinateAxes();
    DrawValueScale();
    painter.setPen(pen_2);
    if(ui->func->text() != "none") {DrawFunc();}
    painter.end();
}










void MainWindow::on_actionSet_Values_triggered()
{
    double max;
    double x=ui->x_min->text().toDouble();
    while(x<=ui->x_max->text().toDouble()){
        max = Execute(x) < max ? max : Execute(x);
        x += 0.0001;
    }
    QMessageBox::information(this, "Max value", QString::number(max));

}







void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Minus)
    {
        h-=3;
        update();
    }
    if(event->key()==Qt::Key_Plus)
    {
        h+=3;
        update();
    }
    if(event->key()==Qt::Key_W)
    {
        OO.ry() += 25;
        update();
    }
    if(event->key()==Qt::Key_S)
    {
        OO.ry() -= 25;
        update();
    }
    if(event->key()==Qt::Key_D)
    {
        OO.rx() -= 25;
        update();
    }
    if(event->key()==Qt::Key_A)
    {
        OO.rx() += 25;
        update();
    }
}


void MainWindow::on_pushButton_clicked()
{
    left_board = ui->x_min->text().toDouble();
    right_board = ui->x_max->text().toDouble();
    step = ui->step->text().toDouble();
    QString func = ui->lineEdit->text();
    if(ui->lineEdit->text() != "") {ui->func->setText(func);}
    update();
}


void MainWindow::on_actionRed_triggered()
{
    pen_2 = (QPen(QColor(255, 0 ,0),2,Qt::SolidLine));
    update();
}


void MainWindow::on_actionGreen_triggered()
{
    pen_2 = (QPen(QColor(0, 255 ,0),2,Qt::SolidLine));
    update();

}


void MainWindow::on_actionYellow_triggered()
{
    pen_2 = (QPen(QColor(255, 238 ,0),2,Qt::SolidLine));
    update();
}


void MainWindow::on_actionDark_BLue_triggered()
{
    pen_2 = (QPen(QColor(21, 0 , 143),2,Qt::SolidLine));
    update();
}


void MainWindow::on_actionPurple_triggered()
{
    pen_2 = (QPen(QColor(141, 1 , 255),2,Qt::SolidLine));
    update();
}

