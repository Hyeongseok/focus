from EMAN2 import *
from sparx import *

import os
import sys


def is_number(s):
    try:
        n=str(float(s))
        if n == "nan" or n=="inf" or n=="-inf" : return False
    except ValueError:
        try:
            complex(s) # for complex
        except ValueError:
            return False
    return True



if __name__ == "__main__":
	
	if len(sys.argv) != 2:
		sys.exit("Missuse detected")

	dirlist = sys.argv[1]
	print ":Evaluating directories listed in ", dirlist
	
        directories = open(dirlist, 'r')

        init = -1e10

        ratio_sum = 0.0
        ratio_num = 0

        for dirfolder in directories:
                configfile = "../" + dirfolder.strip() + "/2dx_image.cfg"
                if os.path.isfile(configfile):
                        f = open(configfile, 'r')
                        TANGL = init
                        DEFOCUS_TLTANG = init
                        LATTICE_TLTANG = init
                        ratio = 0.0
                        for l in f:
                                if l.startswith("set TANGL ="):
                                        TANGL_line = l.split('"')[1]
                                        if is_number(TANGL_line):
                                                TANGL = abs(float(TANGL_line))
                                if l.startswith("set DEFOCUS_TLTANG ="):
                                        DEFOCUS_TLTANG_line = l.split('"')[1]
                                        if is_number(DEFOCUS_TLTANG_line):
                                                DEFOCUS_TLTANG = abs(float(DEFOCUS_TLTANG_line))
                                if l.startswith("set LATTICE_TLTANG ="):
                                        LATTICE_TLTANG_line = l.split('"')[1]
                                        if is_number(LATTICE_TLTANG_line):
                                                LATTICE_TLTANG = abs(float(LATTICE_TLTANG_line))
                        if TANGL > 10.0 and LATTICE_TLTANG > 10.0:
                                ratio = LATTICE_TLTANG / TANGL 
                                ratio_sum += ratio
                                ratio_num += 1
                        print "File ", configfile, " has tilt angle of ", TANGL, ", ", DEFOCUS_TLTANG, ", ", LATTICE_TLTANG, " => ", ratio
                        f.close()
        
        directories.close()

        if ratio_num > 0:
                ratio_ave = ratio_sum / ratio_num
        else:
                ratio_ave = 1.0
        
        print ":: Tilt angles are on average increased by a factor of ", ratio_ave, "  (n = ",ratio_num,")"
                
        directories = open(dirlist, 'r')
        for dirfolder in directories:
                configfile = "../" + dirfolder.strip() + "/2dx_image.cfg"
                if os.path.isfile(configfile):
                        f = open(configfile, 'r')
                        lines = []
                        for l in f:
                                if l.startswith("set TANGL ="):
                                        TANGL_line = l.split('"')[1]
                                        if is_number(TANGL_line):
                                                TANGL = float(TANGL_line)
                                        TANGL_new = TANGL * ratio_ave
                                        lines.append('set TANGL = "' + str(TANGL_new) + '"\n')        
                                        print "Image ", configfile, ": old TANGL = ", TANGL, ", new TANGL = ", TANGL_new
                                else:
                                        lines.append(l)
                        f.close()
                        
                        fout = open(configfile, 'w')
                        fout.writelines(lines)
                        fout.close()

