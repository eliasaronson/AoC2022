file = open("$(@__DIR__)/inp_t1.txt")
lines = readlines(file)

trees = zeros((length(lines[1]), length(lines)))

for (idx_y, l) in enumerate(lines)
    trees[idx_y, :] = parse.(Int, split(l, ""))
end

score = zeros(size(trees))

x_sz, y_sz = size(trees)
idx_x, idx_y = 0, 0

for ii in 1:x_sz
    for jj in 1:x_sz
        # up
        prev = trees[ii, jj]
        up = 0
        if ii > 1
            for u in ii-1:1
                up += 1
                if trees[u, jj] >= prev
                    break
                end
            end
        end
        prev = trees[ii, jj]
        down = 0
        if ii < x_sz
            for u in ii+1:x_sz
                down += 1
                if trees[u, jj] >= prev
                    break
                end
            end
        end
        prev = trees[ii, jj]
        left = 0
        if jj > 1
            for u in jj-1:1
                left += 1
                if trees[ii, u] >= prev
                    break
                end
            end
        end
        prev = trees[ii, jj]
        right = 0
        if jj < x_sz
            for u in jj+1:x_sz
                right += 1
                if trees[ii, u] >= prev
                    break
                end
            end
        end
        score[ii, jj] = left*right*up*down
    end
end

println("Part2: ", maximum(score))
