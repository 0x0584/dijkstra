Update your Homework 2 graph class to add two more methods. Add a constructor that can read in a graph from a file. The file format will be an initial integer that is the node size of the graph and the further values will be integer triples: (i, j, cost). See Sample Test Data here.

Implement either the Prim or Kruskal Minimum Spanning Tree Algorithm. These algorithms are similar in flavor to Dijkstra’s shortest path algorithm. Links to these algorithms are provided besides being explained in the videos.

http://en.wikipedia.org/wiki/Kruskal's_algorithm

http://en.wikipedia.org/wiki/Prim_algorithm

http://en.wikipedia.org/wiki/Minimum_spanning_tree

The output for MST should be the cost and the tree (the edges).  The input will be provided on the website.

More challenging is an extension to this problem discussed in Module 4 as extra credit, however, for the purpose of this Homework, no extra credit will be given. This is to give each edge one of three colors. Consider that a road may be a local street or a highway or a turnpike.  Many route finding algorithms find a least costly path constrained by what type of road to take. We mimic this with colors and ask that you compute MST where the edge colors are a constraint, eg. all edges must be green. You are welcome to try this problem but the peer grading should be based on the simpler problem where there are no colors.
