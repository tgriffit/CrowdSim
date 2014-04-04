__author__ = "adneufel@ualberta.ca"

from bvhutils import writeBvhFile, readInBvh, Skeleton, Joint, Motion, getFrameStr
from modifiers import funcs
import os

outputdir = ""


# Based upon the skel generate a number of altered bvh files
def genSomeBvh(defaultSkel, skelfilename, num):
    for i in range(0, num):
        skelfilename = skelfilename.split(".")
        skelfilename[0] += str(i+1)
        skel = genBvh(defaultSkel)
        skelFilepath = os.path.join(outputdir, ".".join(skelfilename))
        print(outputdir)
        print(skelFilepath)
        writeBvhFile(skelFilepath, skel)


def genBvh(skel):
    newSkel = Skeleton(skel.jointsRoot)

    # call each of the modifiers.funcs on our skeleton
    for func in funcs:
        print("genBvh func call")
        func(newSkel.jointsRoot)

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

    if skel is None:
        print("Error reading bvh file")
    else:
        genSomeBvh(skel, bvhfile, numfile)
    print("Completed")


if __name__ == '__main__':
    main()