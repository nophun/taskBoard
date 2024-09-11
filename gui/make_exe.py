# -*- coding: utf-8 -*-
"""Make single-file executable for CSOLVER2.

The executable is created by running this Python script. Before running it, one
verify and edit the general information, especially the program file location,
below. The program file CSOLVER2.pyw must be in a local folder (not a network
drive folder) together with the other required folder CSOLVER2_files. The
executable is created from these using PyInstaller and at least version 3.3.1
is required.

This script was modified from a similar script for making executables for the
DD-23 Updater and DD-23 Timeout Configurator (by Liangyu Chen, Janesko Oy,
20.2.2019).

Created on Aug 24 2021
Last edit on Aug 25 2021
@author: ANJOK2
"""

import os
from PyInstaller.utils.win32.versioninfo import (
    StringStruct, VSVersionInfo, FixedFileInfo, StringFileInfo, StringTable,
    VarFileInfo, VarStruct)
import PyInstaller.__main__

# General parameters (EDIT THESE ONLY):
SCRIPT_NAME = 'TaskBoard.py'
SCRIPT_LOCATION = (os.getcwd())  # folder of the program files
APP_NAME = 'TASKBOARD'
EXE_NAME = 'TASKBOARD'
VERSION = '1.0'
VERSIONa = (1, 0, 0, 0)
ICON = 'Yuster.ico'
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

