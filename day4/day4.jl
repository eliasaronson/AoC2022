
inp = open("$(@__DIR__)/inp_1.txt")
input = readlines(inp)

## Part 1
N = 0
for l in input
    p1, p2 = split(l, ",")
    min1, max1 = parse.(Int32, split(p1, "-"))
    min2, max2 = parse.(Int32, split(p2, "-"))
    if min1 <= min2 && max1 >= max2
        global N += 1
    elseif min2 <= min1 && max2 >= max1
        global N += 1
    end

end

## Part 2
No = 0
for l in input
    p1, p2 = split(l, ",")
    min1, max1 = parse.(Int32, split(p1, "-"))
    min2, max2 = parse.(Int32, split(p2, "-"))
    if !(max1 < min2 || min1 > max2)
        global No += 1
    end
end


println("N: $N. No: $No");
println("Done.");
