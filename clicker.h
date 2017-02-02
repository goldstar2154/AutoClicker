#ifndef CLICKER_H
#define CLICKER_H

#include <QObject>
#include <QMutex>

class Clicker : public QObject
{
    Q_OBJECT

public:
    explicit Clicker();
    ~Clicker();

    void setKeepGoing(const bool& _keepGoing);
    void setup(const long& _count, const long& _delay, const int& _clickBtn, const int& _clickMode, const bool& _freezePointer);

public slots:
    void start();

signals:
    void progress();
    void finished();

private:
    long click();

    long count;
    long delay;
    int clickBtn;
    int clickMode;
    bool freezePointer;
    bool keepGoing;
    QMutex mutex;

};

#endif // CLICKER_H
