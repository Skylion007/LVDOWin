# -*- coding: utf-8 -*-
"""
This is a sample GUI for LVDOWin written by Aaron Gokaslan.
The GUI just feeds a very simplistic set of parameters to and from video.
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
 
        self.label = QtGui.QLabel("Welcome to LVDOWin!")
        self.label.setAlignment(Qt.AlignCenter| Qt.AlignCenter)
 
        # create the buttons
        encodeFileBtn = QtGui.QPushButton("Convert file to video")
        decodeFileBtn = QtGui.QPushButton("Convert a video to a file")
 
        # connect the buttons to the functions (signals to slots)
        encodeFileBtn.clicked.connect(self.encodeFile)
        decodeFileBtn.clicked.connect(self.decodeFile)
 
        encodeFileBtn.setToolTip("Saves the file as output.mkv")
        decodeFileBtn.setToolTip("Decodes the file at the specified")
        
        source_component = QtGui.QVBoxLayout()
        
        sourceLabel = QtGui.QLabel("Input File:")
        sourceLabel.setToolTip("Specifies what file you want to convert")
        sourceLabel.setAlignment(Qt.AlignCenter| Qt.AlignCenter)
                
        sourceLayout = QtGui.QHBoxLayout()
        #TODO Add a wrapper the preserves the original filename
        openBtn = QtGui.QPushButton("...")
        width = openBtn.fontMetrics().boundingRect(openBtn.text()).width() + 14
        openBtn.setMaximumWidth(width)
        self.sourcePath = QtGui.QLineEdit(".." + os.path.sep + "message.txt")
        self.sourcePath.setEnabled(False)        
        openBtn.clicked.connect(self.setSourcePath)
        sourceLayout.addWidget(sourceLabel)        
        sourceLayout.addWidget(self.sourcePath)
        sourceLayout.addWidget(openBtn)
        
        #source_component.addWidget(sourceLabel)
        source_component.addLayout(sourceLayout)
        
        #source_box = self.BorderBox(source_component)
 
        destination_component = QtGui.QVBoxLayout()

        destinationLabel = QtGui.QLabel("Output File")
        destinationLabel.setToolTip("Specifies where you want to save the file")
        destinationLabel.setAlignment(Qt.AlignCenter| Qt.AlignCenter)
        
        destinationLayout = QtGui.QHBoxLayout()
        #TODO Add a wrapper the preserves the original filename
        saveBtn = QtGui.QPushButton('...')
        width = saveBtn.fontMetrics().boundingRect(saveBtn.text()).width() + 14
        saveBtn.setMaximumWidth(width)
        self.destinationPath = QtGui.QLineEdit(".." + os.path.sep + "output.mkv")
        self.destinationPath.setEnabled(False)
        saveBtn.clicked.connect(self.setDestinationPath)
        destinationLayout.addWidget(destinationLabel)        
        destinationLayout.addWidget(self.destinationPath)
        destinationLayout.addWidget(saveBtn)

        #destination_component.addWidget(destinationLabel)
        destination_component.addLayout(destinationLayout)

        #destination_box = self.BorderBox(destination_component)
 
        self.te = QtGui.QTextEdit()
        self.te.setEnabled(False)
 
        buttonLayout = QtGui.QHBoxLayout()
        buttonLayout.addWidget(encodeFileBtn)
        buttonLayout.addWidget(decodeFileBtn)       
        
        io_component = QtGui.QVBoxLayout()
        io_component.addLayout(source_component)
        io_component.addLayout(destination_component)
        io_box = self.BorderBox(io_component)
    
 
        # layout widgets
        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.label)
        #layout.addWidget(self.HLine())
        #layout.addLayout(source_component)
        #layout.addWidget(source_box)        
        #layout.addWidget(self.HLine())
        #layout.addLayout(destination_component)
        #layout.addWidget(destination_box)
        layout.addWidget(io_box)        
        layout.addWidget(self.te)
        
        layout.addLayout(buttonLayout)
        self.setLayout(layout)
 
        self.adjustSize() 
 
        self.move(QtGui.QDesktopWidget().availableGeometry().center() 
                    - self.frameGeometry().center())

        self.setWindowTitle("LVDOWin - GUI")
            # ...


 

    def setSourcePath(self):
        import re
        path = self.exec_file_open_dialog()
        if path is None:
            return
        path = re.sub('(.)', r'\1', path)
        self.sourcePath.setText(str(path))
        
    def setDestinationPath(self):
        import re
        path = self.exec_file_save_dialog()
        if path is None:
            return
        path = re.sub('(.)', r'\1', path)
        self.destinationPath.setText(str(path))

    def BorderBox(self, layout):
        from PySide.QtGui import QFrame
        toto = QFrame()
        toto.setLayout(layout)
        toto.setFrameShape(QFrame.Box)
        toto.setFrameShadow(QFrame.Sunken)
        return toto        
        
    def popupMessage(self, message):
        from PySide.QtGui import QMessageBox        
        msgBox = QMessageBox()
        msgBox.setWindowTitle('Warning!!!')
        msgBox.setText(message)
        msgBox.exec_()

    def HLine(self):        
        from PySide.QtGui import QFrame
        toto = QFrame()
        toto.setFrameShape(QFrame.HLine)
        toto.setFrameShadow(QFrame.Sunken)
        return toto        
        
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
        
        path = self.sourcePath.text()
        #This is necessary since we are using the shell.      
        
        if path is None:
            return
        
        destination_text = self.destinationPath.text()
        
        #TODO Reimpliment with shell off when you find an alternative for type
        cmd = 'type "%s"' % path
        cmd += ' | lvdoenc -s 640x480 -q 6 --qmin 1 --qmax 4 | '
        cmd += 'x264 --input-res 640x480 --fps 1 --profile high --level 5.1 --tune stillimage '
        cmd += '--crf 22 --colormatrix bt709 --me dia '
        cmd += '--merange 0 -o "%s" -' % destination_text

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
        
        path = self.sourcePath.text()
        #This is necessary since we are using the shell.      
        
        if path is None:
            return
                
        if not isVideo(path):
            self.popupMessage('File: The input file does not seem to be a video file')
            return 

        #TODO Reimpliment with shell off when you find an alternative for type
        cmd = 'ffmpeg -i "%s"' % path
        cmd += ' -r 1 -f rawvideo - | lvdodec -s 640x480 -q 6 --qmin 1 --qmax 4 > '
        #cmd = cmd.split()
        cmd = cmd + '"' + self.destinationPath.text() + '"'
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
                
                
    def exec_file_open_dialog(self):
        path, _ = QtGui.QFileDialog.getOpenFileName(self, "Open File", os.getcwd())
        if path:    
            path = path = path.replace("/", os.path.sep) 
            return path
        else:
            return None

    def exec_file_save_dialog(self):
        path, _ = QtGui.QFileDialog.getSaveFileName(self, "Save file", "")
        print(path)        
        if path:             
            path = path = path.replace("/", os.path.sep) 
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
 
def isVideo(filename):
    filename, file_extension = os.path.splitext(filename)        
    valid_filetypes = ['.mp4', '.flv', '.avi', '.wmv', '.mkv']
    return file_extension is valid_filetypes
        
 
#----------------------------------------------------------------------
if __name__ == "__main__":
    app = QtGui.QApplication([])
    form = OsirisGUI()
    form.show()
    app.exec_()
    