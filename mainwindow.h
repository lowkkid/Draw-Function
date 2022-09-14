#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "setter.h"
#include <QMainWindow>
#include<QKeyEvent>
#include <QMessageBox>
#include <cmath>
#include <stack>
#include <sstream>
using std::stack;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *event);
    double Sin(double x) { //Функция для округления значение синуса
        return (round(sin(x) * 10000000) / 10000000);
    }

    double Cos(double x) { //Функция для округления значение косинуса
        return (round(cos(x) * 10000000) / 10000000);
    }

    double Ctg(double x) { //Функция для расчета котангенса
        double a = cos(x);
        double b = Sin(x);
        return (a / b);
    }

    struct Leksema //Структура, описывающая любое число или операцию
    {
        char type; // 0 для чисел, "+" для операции сложения и т.д.
        double value; //Значение (только для чисел). У операций значение всегда "0"
    };

    bool Maths(stack <Leksema>& Stack_n,stack <Leksema>& Stack_o, Leksema& item) { //Математическая функция, которая производит расчеты
        //Функция имеет тип bool, чтобы при возникновении какой-либо ошибки возвращать "false"
        double a, b, c;
        a = Stack_n.top().value; //Берется верхнее число из стека с числами
        Stack_n.pop(); //Удаляется верхнее число из стека с числами
        switch (Stack_o.top().type) {  //Проверяется тип верхней операции из стека с операциями
        case '+': //Если тип верхней операции из стека с операциями сложение
            b = Stack_n.top().value;
            Stack_n.pop();
            c = a + b;
            item.type = '0';
            item.value = c;
            Stack_n.push(item); //Результат операции кладется обратно в стек с числами
            Stack_o.pop();
            break;

        case '-':
            b = Stack_n.top().value;
            Stack_n.pop();
            c = b - a;
            item.type = '0';
            item.value = c;
            Stack_n.push(item);
            Stack_o.pop();
            break;

        case '^':
            b = Stack_n.top().value;
            Stack_n.pop();
            c = pow(b, a);
            item.type = '0';
            item.value = c;
            Stack_n.push(item);
            Stack_o.pop();
            break;

        case '*':
            b = Stack_n.top().value;
            Stack_n.pop();
            c = a * b;
            item.type = '0';
            item.value = c;
            Stack_n.push(item);
            Stack_o.pop();
            break;

        case '/':
            b = Stack_n.top().value;
            if (a == 0) {
                return false;
            }
            else {
                Stack_n.pop();
                c = (b / a);
                item.type = '0';
                item.value = c;
                Stack_n.push(item);
                Stack_o.pop();
                break;
            }

        case 's':
            c = Sin(a);
            item.type = '0';
            item.value = c;
            Stack_n.push(item);
            Stack_o.pop();
            break;

        case 'c':
            c = Cos(a);
            item.type = '0';
            item.value = c;
            Stack_n.push(item);
            Stack_o.pop();
            break;

        case 't':
            if (Cos(a) == 0) {

            }
            else {
                c = tan(a);
                item.type = '0';
                item.value = c;
                Stack_n.push(item);
                Stack_o.pop();
                break;
            }

        case 'g':
            if (Sin(a) == 0) {

                return false;
            }
            else {
                c = Ctg(a);
                item.type = '0';
                item.value = c;
                Stack_n.push(item);
                Stack_o.pop();
                break;
            }

        case 'E':
            c = exp(a);
            item.type = '0';
            item.value = c;
            Stack_n.push(item);
            Stack_o.pop();
            break;

        default:

            return false;
            break;
        }
        return true;
    }
    int getRang(char Ch) { //Функция возвращает приоритет операции: "1" для сложения и вычитания, "2" для умножения и деления и т.д.
        if (Ch == 's' || Ch == 'c' || Ch == 't' || Ch == 'g' || Ch == 'E')return 4;
        if (Ch == '^')return 3;
        if (Ch == '+' || Ch == '-')return 1;
        if (Ch == '*' || Ch == '/')return 2;
        else return 0;
    }
private slots:
    void on_actiony_x_2_triggered();

    void on_actiony_sin_x_triggered();

    void on_actionSet_Values_triggered();

    void on_actiony_x_2_sin_x_triggered();

    void on_spinBox_3_valueChanged(int arg1);

    void on_spinBox_4_valueChanged(int arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_actiony_x_triggered();

    void on_pushButton_clicked();

    void on_actionRed_triggered();

    void on_actionGreen_triggered();

    void on_actionYellow_triggered();

    void on_actionDark_BLue_triggered();

    void on_actionPurple_triggered();

private:
    Ui::MainWindow *ui;
    double Execute(double x);
    void DrawModule();
    void DrawCoorinateAxes();
    void DrawValueScale();
    void DrawHyperbola();
    void DrawSinus();
    void DrawSinusPlusHyperbola();
    void MessageBorders();
    void DrawFunc();
};
#endif // MAINWINDOW_H
