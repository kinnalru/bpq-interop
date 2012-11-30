#ifndef BPQ_INTEROP_H
#define BPQ_INTEROP_H


#include <iostream>

#include <QObject>


class TestSignal : public QObject {
    Q_OBJECT
public:
    TestSignal(QObject* parent = 0) : QObject(parent) {}
    virtual ~TestSignal() {}
    
    
Q_SIGNALS:
    void activate(const QString& str);
    
public Q_SLOTS:
    void print(const QString& str) {
        std::cerr << "Print: " << str.toStdString() << std::endl;
    }

    void trigger() {
        emit activate("default trigger");
    }
    
    void trigger_str(const QString& str) {
        emit activate(str);
    }
    

};


#endif

