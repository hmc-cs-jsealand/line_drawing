import random

def meow():
    print '{',
    for row in range(16):
        for col in range(16):
            print random.choice([0,1]),
            print ',',
        print
    print '}'


def hawm():
    for row in range(16):
        for col in range(16):
            if random.choice([0,1]) == 0:
                print 'X',
            else:
                print ' ',
        print
