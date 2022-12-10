using BenchmarkTools
using Plots

function parse_inp()
    file = open("$(@__DIR__)/inp_1.txt")
    lines = readlines(file)
    mat = reduce(hcat, split.(lines, " "))

    dist = parse.(Int, mat[2, :])

    dict = Dict("R" => CartesianIndex(0, 1), "L" => CartesianIndex(0, -1), "U" => CartesianIndex(-1, 0), "D" => CartesianIndex(1, 0))
    dir = getindex.(Ref(dict), mat[1, :])

    return dir, dist
end

function part1_old()
    dir, dist = parse_inp()

    tail = CartesianIndex(1, 0)
    start = CartesianIndex(0, 0)

    visited = Set{CartesianIndex{2}}()
    push!(visited, start)

    for i ∈ 1:length(dir)
        to = start + dir[i] * dist[i]

        # map(push!, Iterators.cycle(visited), (start:to)[2:end-1])
        diag = start - tail
        diag_abs = abs.(diag.I)
        axs = findfirst(x->x != 0, dir[i].I)
        tight_turn = (diag_abs[1] != diag_abs[2]) ? 0 : (diag[axs] == dir[i][axs]) ? -1 : 1
        for i ∈ (min(start, to):max(start, to))[2+tight_turn:end-1]
            print(i, ", ")
            push!(visited, i)
        end

        if dist[i] > 1
            tail = to - dir[i]
        end
        start = to
    end

    return visited
end

function part1()
    dir, dist = parse_inp()

    tail = CartesianIndex(0, 0)
    head = CartesianIndex(0, 0)

    visited = Set{CartesianIndex{2}}()
    push!(visited, head)

    for i ∈ 1:length(dir)
        for j ∈ 1:dist[i]
            if maximum(abs.((head + dir[i] - tail).I)) > 1
                tail = head
            end
            head += dir[i]
            push!(visited, tail)
        end
    end
    return visited
end

function part2()
    dir, dist = parse_inp()

    rope_length = 10
    rope = Array{CartesianIndex{2}}(undef, rope_length)
    fill!(rope, CartesianIndex(0, 0))

    visited = Set{CartesianIndex{2}}()
    push!(visited, CartesianIndex(0, 0))

    for i ∈ 1:length(dir)
        for j ∈ 1:dist[i]
            rope[1] += dir[i]

            for part in 2:rope_length
                diff = rope[part - 1] - rope[part]
                diff_abs = abs.((diff).I)
                if maximum(abs.((diff).I)) > 1
                    rope[part] += CartesianIndex((diff[1] > 0) - (diff[1] < 0), (diff[2] > 0) - (diff[2] < 0))
                end
            end
            push!(visited, rope[end])
        end
    end
    return visited
end

function draw(visited)
    visited = (visited .- Ref(minimum(visited))) .+ Ref(CartesianIndex(1, 1))
    vis = zeros(maximum(visited)[1], maximum(visited)[2])
    for I ∈ visited
        vis[I] = 1
    end

    p = heatmap(vis)
    show(p)
    # show(stdout, "text/plain", vis)
    return vis, p
end

println("Part 1")
@btime part1()
println("Part 2")
@btime part2()
visted = part2()
vis, p = draw(visted)
println("Part2: ", length(visted))
