#!/usr/bin/python

import sys
from PyQt4 import QtGui
from PyQt4 import QtCore


def main():
    
    app = QtCore.QCoreApplication.instance()
    if not (app): app = QtGui.QApplication(sys.argv)

    
    w = QtGui.QWidget()
    w.resize(250, 150)
    w.move(300, 300)
    w.setWindowTitle('Simple')
    
    
    
    btn = QtGui.QPushButton('Button', w)
    btn.setToolTip('This is a <b>QPushButton</b> widget')
    btn.resize(btn.sizeHint())
    btn.move(50, 50)      
    btn.clicked.connect(app.quit)
    
    
    mo = btn.metaObject()
    
    #print(dir(mo))
    
    for i in range(0, mo.methodCount()):
        method = mo.method(i)
        if (method.methodType() == QtCore.QMetaMethod.Signal):
            print(method.signature())

    
    w.show()
    
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()