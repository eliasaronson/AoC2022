using BenchmarkTools

function count_trees_dir(trees, coord, step)
    height = trees[coord...]
    x, y = coord
    dx, dy = step
    xi, yi = x + dx, y + dy
    n = 0
    while checkbounds(Bool, trees, xi, yi)
        n += 1
        if trees[xi, yi] >= height
            break
        end
        xi += dx
        yi += dy
    end
    return n
end

function score(trees, coord)
    score = 1
    for step in [(1, 0), (-1, 0), (0, 1), (0, -1)]
        score *= count_trees_dir(trees, coord, step)
    end
    score
end


file = open("$(@__DIR__)/inp_1.txt")
lines = readlines(file)

function part2(lines)
    trees = zeros((length(lines[1]), length(lines)))

    for (idx_y, l) in enumerate(lines)
        trees[idx_y, :] = parse.(Int, split(l, ""))
    end

    highest = -1
    for I in CartesianIndices(trees)
        score = score(trees, Tuple(I))
        if score > highest
            global highest = score
        end
    end
    highest
end

@btime part2(lines)
# println(highest)
