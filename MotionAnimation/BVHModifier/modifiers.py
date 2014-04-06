__author__ = "adneufel@ualberta.ca"

from random import randrange, randint


# return a random number from a range where it is biased towards lower numbers
def unevenrandrange(start, end):
    randnum = randrange(10)
    diff = end-start
    # low is 10%
    lowstart = start
    lowend = start + int(diff/10)
    # mid is 30%
    midstart = lowend+1
    midend = int(midstart + diff/3)
    # top end is 60%
    topstart = midend+1
    topend = end
    
    if randnum == 0:
        return randint(topstart, end)
    elif randnum <= 2:
        return randint(midstart, midend)
    # else 
    return randint(lowstart, lowend)


def backposture(skel):
    power = unevenrandrange(0, 100)  # the severity of the modification
    hipsjoint = skel.getJointByName("Hips")
    jointnames = ["LowerBack", "Spine", "Spine1", "Neck", "Neck1", "Head"]
    jointsinorder = [skel.getJointByName(name) for name in jointnames]
    maxjointmod = []
    
    return skel


def stridelength(skel):
    power = randrange(100)
    return skel


# A list with references to all functions above that take in a skeleton and return
#       a modified version of the skeleton
funcs = [backposture, stridelength]