function part1()
    file = open("$(@__DIR__)/inp_1.txt")
    lines = readlines(file)

    trees = zeros((length(lines[1]), length(lines)))

    for (idx_y, l) in enumerate(lines)
        trees[idx_y, :] = parse.(Int, split(l, ""))
    end

    visable = Set{Pair{Int, Int}}()

    x_sz, y_sz = size(trees)
    idx_x, idx_y = 0, 0

    for case in 1:4
        for i in 1:x_sz
            prev = -1
            for j in 1:y_sz
                if case == 1
                    global idx_x, idx_y = i, j;
                elseif case == 2
                    global idx_x, idx_y = j, i;
                elseif case == 3
                    global idx_x, idx_y = i, 1 + x_sz - j;
                elseif case == 4
                    global idx_x, idx_y = 1 + y_sz - j, i;
                end
                # println(idx_x, " ", idx_y, ", prev: ", prev, ", val: ", trees[idx_x, idx_y])
                if trees[idx_x, idx_y] > prev
                    push!(visable, Pair(idx_x, idx_y))
                    prev = trees[idx_x, idx_y]
                end
            end
        end
    end

    # println("Part1: ", length(visable))
end

@btime part1()
