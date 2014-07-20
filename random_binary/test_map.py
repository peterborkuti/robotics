import random

def init(map):
    p = []
    prob = 1.0 / len(map)
    for r in range(len(map)):
        p.append(prob)
    return p


def sense(p, place,  map):
    q = []
    s = 0.0
    Z = map[place]
    for c in range(len(p)):
        hit = (Z == map[c])
        prob = 1 if hit else 0

        q.append(p[c] * prob)

    s = sum(q)

    for r in range(len(q)):
        q[r] = q[r] / s

    return q


def move(p, U):
    q = []
    for c in range(len(p)):
        v_move = p[(c - U) % len(p)]
        q.append(v_move)
    return q


def check_map(map):
    steps_needed = []
    dir = 1

    for k in range(len(map)):
        p = init(map)
        steps = 0

        while ((max(p) < 1.0)):
            p = sense(p, (k + steps)%len(map),  map)
            p = move(p, dir)
            steps += dir
            if (steps > (len(map) + 1)):
                return [10000,  10000]

        steps_needed.append(steps)
 
    s  = steps_needed
    return [s,  sum(s)/len(s)]


def check_all(N):
    fmt = '{0:0' + str(N) + 'b}'
    min = 10000
    for i in range(1,  2**N):
        map =  fmt.format(i)
        [steps,  avg] = check_map(map)
        if (avg <= min):
            print ([i,  map,  steps,  avg])
            min = avg    


def check_one(map):
    [steps,  avg] = check_map(map)
    print ([0,  map,  steps,  avg])


def check_one_random(N):
    fmt = '{0:0' + str(N) + 'b}'
    map = fmt.format(random.getrandbits(N))
    check_one(map)


#check_one_random(120)
check_all(24)
