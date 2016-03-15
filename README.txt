I changed the minimax algorithm to a recursive function
so that it can easily change what depth it searches to, as well as more
conveniently searches higher depths. I also combined the edge weighting
with the recursive function in a way that the minimax algorithm adds the edge
weighting score to the total score for every step. In this way, the algorithm
takes into account not only how many pieces, ultimately, each side has, but also
how many side and edge pieces each side has.