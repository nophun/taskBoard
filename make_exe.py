import os
from PyInstaller.utils.win32.versioninfo import (
    StringStruct, VSVersionInfo, FixedFileInfo, StringFileInfo, StringTable,
    VarFileInfo, VarStruct)
import PyInstaller.__main__
import shutil

SCRIPT_NAME = 'taskBoard-programmer/src/TaskBoard.py'
SCRIPT_LOCATION = (os.getcwd())
APP_NAME = 'TASKBOARD'
EXE_NAME = 'TASKBOARD'
VERSION = '1.3'
VERSIONa = (1, 3, 0, 0)
ICON = 'taskBoard-programmer/logo/Yuster.ico'
COMPANY = 'Yuster electronics'
COPYRIGHT = 'Yuster electronics'

# Construct VSVersionInfo for writing version info to a temporary file:
vi = VSVersionInfo(
    ffi=FixedFileInfo(filevers=VERSIONa, prodvers=VERSIONa),
    kids=[
       StringFileInfo([StringTable("040904B0", [
           StringStruct("CompanyName", COMPANY),
           StringStruct("FileDescription", APP_NAME),
           StringStruct("FileVersion", VERSION),
           StringStruct("LegalCopyright", COPYRIGHT),
           StringStruct("OriginalFilename", EXE_NAME),
           StringStruct("ProductName", APP_NAME),
           StringStruct("ProductVersion", VERSION)])]),
       VarFileInfo([VarStruct('Translation', [1033, 1200])])])
print(SCRIPT_LOCATION)
# Collect required files not covered by import calls in CSOLVER:
add_icon = f'--icon={ICON}'

# Set the directory, make the exe and remove temporary version file:

os.chdir(SCRIPT_LOCATION)
file = open('version.temp', 'w')
file.write(str(vi))
file.close()

PyInstaller.__main__.run([f'{SCRIPT_NAME}',
                          f'--distpath={SCRIPT_LOCATION}\\dist',
                          '--onefile',
                          f'{add_icon}',
                          '--version-file=version.temp',
                          '--noconsole'])
os.unlink('version.temp')
shutil.copyfile(ICON, 'dist/Yuster.ico')
