#!/usr/bin/python

import subprocess
import time
import os
import sys
import getpass

"""
Python script to check for updates iin gentoo linux so it can be displayed in polybar

This code was written when I was still learning python, and was written for py2.7
"""

# To prevent uneccessary update when changing polybar config
def check_last():

    if not os.path.exists('/home/{}/.config/polybar/last_check'.format(getpass.getuser())):
        return None
    else:
        cur_time = time.time()
        st = os.stat('/home/{}/.config/polybar/last_check'.format(getpass.getuser()))
        if cur_time - st.st_mtime > 3600:
            return True
        else:
            return False

def check_updates():
    try:
        num = 0
        out = subprocess.check_output(['emerge', '-pvuDn', '@world'])
        for line in out.split('\n'):
            if 'Total' in line:
                num = line.split(' ')[1] 
        print("Pending updates: {}".format(num))
        return num
    except subprocess.CalledProcessError:
        print("Update error")
        return False
    except:
        pass
        return False

def read_from_file():

    with open('/home/{}/.config/polybar/last_check'.format(getpass.getuser()), 'r') as f:
        num = f.readline()

    print("Pending updates: {}".format(num))

def write_to_file(num):
    with open('/home/{}/.config/polybar/last_check'.format(getpass.getuser()), 'w+') as fw:
        fw.truncate()
        fw.write(num)

def main():

    status = check_last()

    if status is True:
        output = check_updates()
    elif status is False:
        read_from_file()
        sys.exit(0)
    else:
        output = check_updates()

    if output is not False:
        write_to_file(output)

if __name__ == '__main__':
    main()
