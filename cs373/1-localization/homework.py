colors = [['red', 'green', 'green',   'red', 'red'],
          ['red',   'red', 'green',   'red', 'red'],
          ['red',   'red', 'green', 'green', 'red'],
          ['red',   'red',   'red',   'red', 'red']]

measurements = ['green', 'green', 'green' ,'green', 'green']

motions = [[0,0],[0,1],[1,0],[1,0],[0,1]]

sensor_right = 0.7

p_move = 0.8

p = []
prob = 1.0 / (len(colors) * len(colors[0]))
for r in range(len(colors)):
    p.append([])
    for c in range(len(colors[r])):
        p[r].append(prob)


def show(p):
    for i in range(len(p)):
        print (p[i])


#Do not delete this comment!
#Do not use any import statements.
#Adding or changing any code above may
#cause the assignment to be graded incorrectly.

#Enter your code here:

def sense(p, Z):
    q = []
    s = 0.0
    for r in range(len(p)):
        q.append([])
        for c in range(len(p[r])):
            hit = (Z == colors[r][c])
            prob = sensor_right if hit else (1 - sensor_right)
            #print r,c,hit,Z,colors[r][c], p[r][c], hit * sensor_right, p[r][c] * (hit * sensor_right)
            q[r].append(p[r][c] * prob)
        s += sum(q[r])

    for r in range(len(q)):
        for c in range(len(q[r])):
            q[r][c] = q[r][c] / s

    return q


def move(p, U):
    q = []
    for r in range(len(p)):
        q.append([])
        for c in range(len(p[r])):
            mr = U[0]
            mc = U[1]
            v_dontmove = p[r][c] * (1.0 - p_move)
            v_move = p[(r - mr) % len(p)][(c - mc) % len(p[r])] * p_move
            s = v_dontmove + v_move
            # s = pExact * p[(i-U) % len(p)]
            # s = s + pOvershoot * p[(i-U-1) % len(p)]
            # s = s + pUndershoot * p[(i-U+1) % len(p)]
            q[r].append(s)
    return q


#show(colors)
#print 'Init:'
#show(p)
for k in range(len(motions)):
    #print
    #print k
    p = move(p, motions[k])
    #print 'moved:', motions[k]
    #show(p)
    p = sense(p, measurements[k])
    #print 'sensed:', measurements[k]
    #show(p)


#Your probability array must be printed
#with the following code.

#print
#print 'done'
show(p)
