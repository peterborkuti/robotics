import re # for overlapping occurences
import random

def count_repeats(s, checkLen, actual_min):
    allLen = len(s)
    checked = []
    szum = 0
    for l in xrange(1, checkLen):
        #print 'new checklen:', l
        
        for num in xrange(0, 2**l):
            
            for ll in xrange(l, allLen):
                #print 'new appendlen:', ll
                formatStr = '{0:0' + str(ll) + 'b}'
                a = formatStr.format(num)
                if (a in checked):
                    continue
                checked.append(a)
                #print 'check: l:', l, 'll:', ll, 'num:', num, 'a: ',a
                ss = s + s[:(len(a)-1)]
                found = re.findall('(?=' + a + ')', ss)
                c = len(found)
                if (c == 0):
                    break
                if (c > 1):
                    #print c, 'repetition found of ', a
                    szum += c
                if szum > actual_min + 2:
                    return 10000

    return szum

N = 16
min=10000
fmt = '{0:0' + str(N) +'b}'
print fmt
s = fmt.format(random.getrandbits(N))
print 'setting min to a reasonable value with random binary:', s
min = count_repeats(s, len(s), min)
print 'done. starting min is ', min

for i in xrange(0, 2**N):
    
    fmt = '{0:0' + str(N) + 'b}'
    s = fmt.format(i)

    c = count_repeats(s, len(s), min)
    if (c < min):
        min = c
    if (c == min):
        print 'total repetitions in ', s, ' is ', c

print 'min:', min
