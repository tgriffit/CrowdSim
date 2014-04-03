import operator

__author__ = "adneufel@ualberta.ca"
chanNum = 1

### CLASSES ###
class Skeleton:
    jointsList = []
    jointsRoot = None
    
    def __init__(self, jointsRoot):
        self.jointsRoot = jointsRoot

    # create an ordering of the joints based on channNum
    def initJointsList(self):
        self.addJointToJointsList(self.jointsRoot)
        # sort them by channel number
        self.jointsList.sort(key=operator.attrgetter('chanNum'))
    
    def addJointToJointsList(self, joint):
        self.jointsList.append(joint)
        print "num children: " + str(len(joint.childJoints))
        for j in joint.childJoints:
            print str(j.chanNum)
            #self.addJointToJointsList(j)
    
    def getJointByChanNum(self, num):
        # if tfirst call, call initJointsList
        if self.jointsList is None:
            self.initJointsList()
        
        return self.jointsList[num-1]
    
    def addMotionToJoints(self, motion):
        # a list of joint motion data organized by joint chanNum
        jointFrameData = []
        for frame in motion.samples:
            pass

class Joint:
    name = ""
    type = ""
    offset = (0, 0, 0)
    numChannels = 0
    chanNum = 0;
    childJoints = []
    motionFrames = []
    
    def add_child(joint):
        self.childJoints.append(joint)
    
    def set_offset(x, y, z):
        self.offset = (x, y, z)

class Motion:
    numFrames = 0
    frameTime = 0.0
    jointNumChannels = []
    samples = [] # raw lines of motion data as float

## Start of NON-CLASS functions

def readInBvh(bvhfile):
    # read in all the lines of the file and strip leading/trailing whitespace
    file = open(bvhfile, "r")
    
    # if first line is not HIERARCHY then the file is erroneous
    firstline = file.readline()
    if not firstline.startswith("HIERARCHY"):
        print "File format does not conform to BVH standards"
        print "-> First line is not 'HIERARCHY'"
        exit(0)
    
    # Parse the HIERARCHY section of the bvh file
    rootJoint = readInJoint(file)
    
    # Parse the MOTIN section of the bvh file
    motion = readInMotion(file)
    
    skeleton = Skeleton(rootJoint)
    skeleton.initJointsList()
    
    file.close()
    return Skeleton

def readInJoint(file):
    global chanNum
    line = []
    newJoint = Joint()
    
    ## SET JOINT NAME AND TYPE ##
    line = getNextLine(file)
    # if not valid, return None as fail case
    if not isValidJoint(line):
        return None
    
    newJoint.type = line[0]
    newJoint.name = line[1]
    
    # account for the special case of end site joints
    if newJoint.name == "End":
        newJoint.name += " Site"
    
    getNextLine(file)   # skip the {
    
    ## SET JOINT OFFSET ##
    line = getNextLine(file)
    
    if line[0] != "OFFSET" or len(line) != 4:
        print "Error: OFFSET malformed or not where expected in " + newJoint.name
        exit(0)
    
    x = float(line[1])
    y = float(line[2])
    z = float(line[3])
    newJoint.offset = (x, y, z)
    
    ## SET JOINT CHANNELS INFO ##
    line = getNextLine(file)
    
    if line[0] == "CHANNELS":
        newJoint.numChannels = int(line[1])
        newJoint.chanNum = chanNum
        
        continueParse = True
        while(continueParse):
            childJoint = readInJoint(file)
            if childJoint is None:
                continueParse = False
            else:
                newJoint.childJoints.append(childJoint)
        chanNum += 1
    else:
        newJoint.numChannels = 0
        newJoint.chanNum = -1
    
    return newJoint

def isValidJoint(line):
    # if the joint is a ROOT, JOINT or End Site return true
    if line[0] == "ROOT" or line[0] == "JOINT" or line[0] == "End":
        return True
    return False

# get and return the next line from file as a list and strip lead/trail whitespace 
def getNextLine(file):
    return file.readline().strip().split(" ")

def readInMotion(file):
    motion = Motion()
    line = []
    # move the file cursor up to the MOTION section
    complete = False
    while not complete:
        line = getNextLine(file)
        complete = line[0] == "MOTION"
    
    ## SET MOTION numFrames ##
    line = getNextLine(file)
    if line[0] == "Frames:":
        motion.numFrames = int(line[1])
    else:
        print "Error cannot find Frames value"
        exit(0)
    
    ## SET MOTION frameTime ##
    line = getNextLine(file)
    if line[0] == "Frame" and line[1] == "Time:":
        motion.frameTime = float(line[2])
    else:
        print "Error cannot find FrameTime value"
    
    ## PARSE MOTION CHANNEL FRAMES ##
    while True:
        line = getNextLine(file)
        # if end of file has been reached, exit the loop
        if line[0] == '':
            break
        else:
            # convert from strs of numbers to floats
            floatline = [float(x) for x in line]
            motion.samples.append(floatline)
    
    return motion

def writeBvhFile(filepath, skeleton):
    pass
