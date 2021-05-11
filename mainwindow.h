#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
  virtual void showEvent(QShowEvent *event) override;

private:
  bool m_first_show;

};

#endif // MAINWINDOW_H
