#include "logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

static QFile *logFile = nullptr;
static QStringList logLines;   // хранит все строки в порядке: сначала новые, потом старые

static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!logFile || !logFile->isOpen())
        return;

    // Форматируем строку лога
    QString level;
    switch (type) {
        case QtDebugMsg:    level = "DEBUG"; break;
        case QtInfoMsg:     level = "INFO";  break;
        case QtWarningMsg:  level = "WARN";  break;
        case QtCriticalMsg: level = "CRIT";  break;
        case QtFatalMsg:    level = "FATAL"; break;
    }
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString logLine = QString("[%1] [%2] %3").arg(timestamp, level, msg);

    // Вставляем новую строку в начало списка
    logLines.prepend(logLine);

    // Перезаписываем файл целиком
    if (logFile->isOpen()) {
        logFile->resize(0);                     // очищаем файл
        QTextStream out(logFile);
        for (const QString &line : logLines) {
            out << line << Qt::endl;
        }
        out.flush();
    }

    if (type == QtFatalMsg)
        abort();
}

void initLogging()
{
    // Папка для логов – рядом с исполняемым файлом (или currentPath)
    QString logDir = QDir::currentPath() + "/log";
    QDir dir;
    if (!dir.exists(logDir)) {
        if (!dir.mkpath(logDir)) {
            qWarning() << "Не удалось создать папку для логов:" << logDir;
            return;
        }
    }

    QString dateStr = QDate::currentDate().toString("yyyy-MM-dd");
    QString logPath = logDir + "/" + dateStr + "_loader.log";

    logFile = new QFile(logPath);
    if (!logFile->open(QIODevice::ReadWrite | QIODevice::Text)) {
        // Пробуем создать файл заново
        logFile->open(QIODevice::WriteOnly | QIODevice::Text);
        if (!logFile->isOpen()) {
            qWarning() << "Не удалось открыть файл лога:" << logPath;
            delete logFile;
            logFile = nullptr;
            return;
        }
    }

    // Читаем существующие строки (если файл не пуст)
    logLines.clear();
    QTextStream in(logFile);
    in.seek(0);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.isEmpty())
            logLines.append(line);
    }

    // Устанавливаем обработчик (после чтения, чтобы избежать рекурсии)
    qInstallMessageHandler(messageHandler);

    // Первое сообщение – о запуске, оно добавится в начало при вызове qInfo()
    qInfo() << "========== Приложение запущено ==========";
    qInfo() << "Лог-файл:" << logPath;
}
