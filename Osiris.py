# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
from PySide import QtGui
from PySide.QtCore import Qt
from subprocess import Popen, PIPE, STDOUT
import os

########################################################################
class OsirisGUI(QtGui.QWidget):
    """"""
 
    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""
        # super(DialogDemo, self).__init__()
        QtGui.QWidget.__init__(self)
 
        self.label = QtGui.QLabel("Welcome to Osiris Ninja Backup!")
        self.label.setAlignment(Qt.AlignCenter| Qt.AlignCenter)
 
        # create the buttons
        encodeFileBtn = QtGui.QPushButton("Convert file to video")
        decodeFileBtn = QtGui.QPushButton("Convert a video to a file")
 
        # connect the buttons to the functions (signals to slots)
        encodeFileBtn.clicked.connect(self.encodeFile)
        decodeFileBtn.clicked.connect(self.decodeFile)
 
        encodeFileBtn.setToolTip("Saves the file as output.mkv")
        decodeFileBtn.setToolTip("Decodes the file at the specified")
 
        uploadLayout = QtGui.QHBoxLayout()
        #TODO Add a wrapper the preserves the original filename
        uploadLabel = QtGui.QLabel("Decoded Output File")
        uploadLabel.setToolTip("Specifies where you want to save the file")
        uploadLabel.setAlignment(Qt.AlignCenter| Qt.AlignCenter)
        saveBtn = QtGui.QPushButton("...")
        self.saveText = QtGui.QLineEdit(".." + os.path.sep + "message.txt")
        saveBtn.clicked.connect(self.setSaveText)
        uploadLayout.addWidget(self.saveText)
        uploadLayout.addWidget(saveBtn)
 
        self.te = QtGui.QTextEdit()
        self.te.setEnabled(False)
 
        # layout widgets
        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.label)
        layout.addWidget(encodeFileBtn)
        layout.addWidget(decodeFileBtn)
        layout.addWidget(uploadLabel)        
        layout.addLayout(uploadLayout)
        layout.addWidget(self.te)
        self.setLayout(layout)
 
 
        self.move(QtGui.QDesktopWidget().availableGeometry().center() 
                    - self.frameGeometry().center())


        self.setWindowTitle("LVDOWin - Osiris Backup")
            # ...
    def setSaveText(self):
        path = self.exec_file_save_dialog()
        self.saveText.setText(str(path))
        
    def normalOutputWritten(self, text):
        """Append text to the QTextEdit."""
        # Maybe QTextEdit.append() works as well, but this is how I do it:
        cursor = self.te.textCursor()
        cursor.movePosition(QtGui.QTextCursor.End)
        cursor.insertText(text)
        self.te.setTextCursor(cursor)
        self.te.ensureCursorVisible() 
 
    #------O----------------------------------------------------------------
    def encodeFile(self):
        """
        Opens a file dialog and encodes the file as output.mkv
        """
        
        path = self.exec_file_open_dialog()
        path = path.replace("/", os.path.sep) 
        #This is necessary since we are using the shell.      
        
        if path is None:
            return
        
        #TODO Reimpliment with shell off when you find an alternative for type
        cmd = 'type "%s"' % path
        cmd += ' | lvdoenc -s 640x480 -q 6 --qmin 1 --qmax 4 | '
        cmd += 'x264 --input-res 640x480 --fps 1 --profile high --level 5.1 --tune stillimage '
        cmd += '--crf 22 --colormatrix bt709 --me dia --merange 0 -o ..//output.mkv -'
       
        bin_directory = os.getcwd() + os.path.sep +'bin'
       
        p = Popen(cmd, stdout = PIPE, 
                stderr = STDOUT, shell = True, cwd=bin_directory, 
                bufsize = 0, universal_newlines=True)
        while True:
            line = p.stdout.readline()
            print(line.rstrip())
            self.normalOutputWritten(str(line))               
            self.te.repaint()
            if not line: break
    
    def decodeFile(self):
        """
        Opens a file dialog and decodes the file as message.txt
        """
        
        path = self.exec_file_open_dialog()
        path = path.replace("/", os.path.sep) 
        #This is necessary since we are using the shell.      
        
        if path is None:
            return
        
        #TODO Reimpliment with shell off when you find an alternative for type
        cmd = 'ffmpeg -i "%s"' % path
        cmd += ' -r 1 -f rawvideo - | lvdodec -s 640x480 -q 6 --qmin 1 --qmax 4 > '
        #cmd = cmd.split()
        cmd = cmd + '"' + self.saveText.text() + '"'
        bin_directory = os.getcwd() + os.path.sep +'bin'
        print(cmd)
       
        p = Popen(cmd, stdout = PIPE, 
                stderr = STDOUT, shell = True, cwd=bin_directory, 
                bufsize = 0, universal_newlines=True)
        while True:
            line = p.stdout.readline()
            print(line.rstrip())
            self.normalOutputWritten(str(line))               
            self.te.repaint()
            if not line: break
                
                
    def exec_file_open_dialog(self):
        path, _ = QtGui.QFileDialog.getOpenFileName(self, "Open File", os.getcwd())
        if path:    
            return path
        else:
            return None

    def exec_file_save_dialog(self):
        path, _ = QtGui.QFileDialog.getSaveFileName(self, "Save file", "")
        print(path)        
        print(dir(path))
        if path:    
            return path
        else:
            return None


    #----------------------------------------------------------------------
    def openDirectoryDialog(self):
        """
        Opens a dialog to allow user to choose a directory
        """
        flags = QtGui.QFileDialog.DontResolveSymlinks | QtGui.QFileDialog.ShowDirsOnly
        d = directory = QtGui.QFileDialog.getExistingDirectory(self,
                                                               "Open Directory",
                                                               os.getcwd(),
                                                               flags)
        self.label.setText(d)
 
#----------------------------------------------------------------------
if __name__ == "__main__":
    app = QtGui.QApplication([])
    form = OsirisGUI()
    form.show()
    app.exec_()
    