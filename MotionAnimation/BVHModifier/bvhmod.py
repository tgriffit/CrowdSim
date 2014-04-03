__author__ = "adneufel@ualberta.ca"

from bvhutils import writeBvhFile, readInBvh
from bvhutils import Joint, Skeleton, Motion

def main():
    '''
    parser = argparse.ArgumentParser(description='Create a series of modified BVH files from a single BVH file')
    parser.add_argument("-f", help="Path to the original BVH file",
                        dest="bvhfile", type=str, required=True)
    parser.add_argument("-n", help="The number of altered BVH files to create",
                        dest="num", type=int, required=True)
    
    args = parser.parse_args(sys.argv[1:])
    
    # bvhfile to read from
    bvhfile = args.bvhfile
    # number of alternative bvh files to create
    numfiles = args.num
    '''
    # temporary hard-coded example bvh file
    bvhfile = "../AnimFiles/BVH/walking-497frames.bvh"
    numfile = 1
    
    skel = readInBvh(bvhfile)
    
    print(str(len(skel.jointsList)))
    for j in skel.jointsList:
        print(j.chanNum)

    if skel is None:
        print("Error reading bvh file")
    #else:
    #    genSomeBvh(skel, numfile)


# Based upon the skel generate a number of altered bvh files
def genSomeBvh(defaultSkel, num):
    for i in range(0, num):
        skel = genBvh(defaultSkel)
        skelFilepath = defaultSkel.file + str(i)
        writeBvhFile(skelFilepath, skel)


def genBvh(skel):
    newSkel = Skeleton(skel.jointsRoot)

    return newSkel

if __name__ == '__main__':
    main()