#!/usr/bin/python

import sys
import sip
from PyQt4 import QtGui
from PyQt4 import QtCore

#wrap raw pointer to specified Qt-class
def wrap(type, ptr):
	return sip.wrapinstance(ptr, type)

#def main():

	# ts_raw - loaded from C++ environment

print("raw value:", ts_raw);
ts_wrapped = wrap(QtCore.QObject, ts_raw);
ts_wrapped.trigger_str("hello from python!")

app = QtCore.QCoreApplication.instance()
if not (app): app = QtGui.QApplication(sys.argv)


print("raw www:", www);
www = QtGui.QWidget()
print("raw2 www:", www);
www.resize(250, 150)
www.move(300, 300)
www.setWindowTitle('Simple')

btn = QtGui.QPushButton('Button', www)
btn.setToolTip('This is a <b>QPushButton</b> widget')
btn.resize(btn.sizeHint())
btn.move(50, 50)
btn.clicked.connect(ts_wrapped.trigger)

www.show()


#sys.exit(app.exec_())


#if __name__ == '__main__':
	#main()