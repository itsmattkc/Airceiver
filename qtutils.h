#ifndef QTUTILS_H
#define QTUTILS_H

#include <QMessageBox>

class QtUtils
{
public:
  QtUtils() = default;

  static int MessageBox(QWidget *parent, const QString &title, const QString &message, QMessageBox::StandardButtons buttons = QMessageBox::Ok);

};

#endif // QTUTILS_H
