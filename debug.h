#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>

void DebugHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  QByteArray localMsg = msg.toLocal8Bit();
  const char *file = context.file ? context.file : "";
  const char *function = context.function ? context.function : "";

  const char *msg_type = "UNKNOWN";

  switch (type) {
  case QtDebugMsg:
    msg_type = "DEBUG";
    break;
  case QtInfoMsg:
    msg_type = "INFO";
    break;
  case QtWarningMsg:
    msg_type = "WARNING";
    break;
  case QtCriticalMsg:
    msg_type = "CRITICAL";
    break;
  case QtFatalMsg:
    msg_type = "FATAL";
    break;
  }

  fprintf(stderr, "[%s] %s\n", msg_type, localMsg.constData());

#ifdef Q_OS_WINDOWS
  fflush(stderr);
#endif
}

#endif // DEBUG_H
