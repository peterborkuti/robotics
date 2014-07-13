colors = [['red', 'green', 'green',   'red', 'red'],
          ['red',   'red', 'green',   'red', 'red'],
          ['red',   'red', 'green', 'green', 'red'],
          ['red',   'red',   'red',   'red', 'red']]

measurements = ['green', 'green', 'green' ,'green', 'green']

p = [[1./20, 1./20, 1./20, 1./20, 1./20],
     [1./20, 1./20, 1./20, 1./20, 1./20],
     [1./20, 1./20, 1./20, 1./20, 1./20],
     [1./20, 1./20, 1./20, 1./20, 1./20]]

# motions = [[0,0],[0,1],[1,0],[1,0],[0,1]]

motions = [[0,0],[0,1],[1,0],[1,0],[0,1]]

sensor_right = 1.0

p_move = 1.0

def show(p):
    for i in range(len(p)):
        print p[i]

#Do not delete this comment!
#Do not use any import statements.
#Adding or changing any code above may
#cause the assignment to be graded incorrectly.

#Enter your code here:

def sense(p, Z):
    q=[]
    s=0
    for r in range(len(p)):
        q.append([])
        for c in range(len(p[r])):
            hit = (Z == colors[r][c])
            q[r].append(p[r][c] * (hit * sensor_right))
        s += sum(q[r])

    for r in range(len(q)):
        for c in range(len(q[r])):
            q[r][c] = q[r][c] / s

    return q

def move(p, U):
    q = []
    for r in range(len(p)):
        q.append([]);
        for c in range(len(p[r])):
            mr = U[0]
            mc = U[1]
            s = p[ (r - mr) % len(p) ][ (c - mc) % len(p[r]) ]    
            # s = pExact * p[(i-U) % len(p)]
            # s = s + pOvershoot * p[(i-U-1) % len(p)]
            # s = s + pUndershoot * p[(i-U+1) % len(p)]
            q[r].append(s)
    return q

for k in range(len(motions)):
    p = sense(p, measurements[k])
    p = move(p, motions[k])
    



#Your probability array must be printed 
#with the following code.

show(p)
