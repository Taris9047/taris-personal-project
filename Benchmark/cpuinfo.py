#!/usr/bin/env python

# CPU information script adopted from 
# http://stackoverflow.com/questions/4842448/getting-processor-information-in-python
#

import os, platform, subprocess, re

def cpu_type(OS_type):
    if OS_type == 'Windows':
        return platform.processor()
    elif OS_type == 'Darwin':
        import os
        os.environ['PATH'] = os.environ['PATH'] + os.pathsep + '/usr/sbin'
        command ="sysctl -n machdep.cpu.brand_string"
        return subprocess.check_output(command, shell=True).strip()
    elif OS_type == 'Linux':
        command = "cat /proc/cpuinfo"
        all_info = subprocess.check_output(command, shell=True).strip()
        for line in allInfo.split("\n"):
            if "model name" in line:
                return re.sub( ".*model name.*:", "", line,1)
    else:
    	return 'Unknown Processor'
    return ""