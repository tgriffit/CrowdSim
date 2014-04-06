__author__ = "adneufel@ualberta.ca"

from bvhutils import writeBvhFile, readInBvh, Skeleton, Joint, Motion, getFrameStr
from modifiers import funcs, unevenrandrange
from copy import deepcopy
import os

outputdir = ""


# Based upon the skel moderate a number of altered bvh files
def modSomeBvh(defaultSkel, skelfilename, num):
    for i in range(0, num):
        skelfilename = skelfilename.split(".")
        skelfilename[0] += str(i+1)
        modskel = modBvh(defaultSkel)
        skelFilepath = os.path.join(outputdir, ".".join(skelfilename))
        writeBvhFile(skelFilepath, modskel)


def modBvh(skel):
    newSkel = deepcopy(skel)

    # call each of the modifiers.funcs on our skeleton
    for func in funcs:
        print("modBvh func call")
        func(newSkel)

    return newSkel


def main():
    global outputdir
    '''
    parser = argparse.ArgumentParser(
        description='Create some modified BVH files from a single BVH file')
    parser.add_argument("-f", help="Path to the original BVH file",
                        dest="bvhfile", type=str, required=True)
    parser.add_argument("-o", help="Directory for output files",
                        dest="outdir", type=str, required=True)
    parser.add_argument("-n", help="The number of altered BVH files to create",
                        dest="num", type=int, required=True)

    args = parser.parse_args(sys.argv[1:])

    # bvhfile to read from
    bvhfilepath = args.bvhfile
    numfiles = args.num  # number of modified bvh files to create
    outputdir = args.outdir
    '''
    # temporary hard-coded example bvh file
    bvhfilepath = "../AnimFiles/BVH/walking-497frames.bvh"
    numfile = 1
    outputdir = "TestOutput"
    
    # parse bvhfilepath to get just the filename
    bvhfile = os.path.split(bvhfilepath)[1]
    
    skel = readInBvh(bvhfilepath)
    
    for i in range(0, 100):
        print(unevenrandrange(1, 100))
    
    if skel is None:
        print("Error reading bvh file")
    else:
        modSomeBvh(skel, bvhfile, numfile)
    print("Completed")


if __name__ == '__main__':
    main()