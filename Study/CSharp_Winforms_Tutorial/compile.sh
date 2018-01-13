#!/bin/sh
CC="gmcs"
Assemblies="-r:System.Windows.Forms.dll -r:System.Drawing.dll -r:System.dll"

$CC $Assemblies $1



