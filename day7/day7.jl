file = open("$(@__DIR__)/inp_1.txt")
lines = readlines(file)

thresh = 100000
thresh2 = 30000000 - (70000000-43636666)

q = Int[]
smaller = Int[]
larger = Int[]

append!(q, 0)
for l in lines
    if length(l) < 4
        continue
    end
    if l[1:4] == "\$ cd"
        if l == "\$ cd .."
            del = pop!(q)
            if del <= thresh
                append!(smaller, del)
            elseif del >= thresh2
                append!(larger, del)
            end
        else
            append!(q, 0)
        end
    elseif l[1] != '\$'
        ll = split(l, " ")
        if tryparse(Float64, ll[1]) !== nothing
            q .+= parse(Int, ll[1])
        end
    end
end

q1 = filter(x->x<=thresh, q)
q2 = filter(x->x>=thresh2, q)
println(larger)
println(q2)
append!(larger, q2)

println("Part1: ", sum(q1) + sum(smaller))
println("Part2: ", minimum(larger))
