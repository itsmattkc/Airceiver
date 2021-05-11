#include "qtutils.h"

int QtUtils::MessageBox(QWidget *parent, const QString &title, const QString &message, QMessageBox::StandardButtons buttons)
{
  QMessageBox m(parent);
  m.setWindowTitle(title);
  m.setText(message);
  m.setWindowModality(Qt::WindowModal);

  for (uint b = QMessageBox::FirstButton; b <= QMessageBox::LastButton; b<<=1) {
    if (b & buttons) {
      m.addButton(static_cast<QMessageBox::StandardButton>(b));
    }
  }

  return m.exec();
}
