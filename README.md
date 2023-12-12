# Single Image Tree Modeling
![ezgif com-video-to-gif-converted](https://github.com/Akash21009/Computer_Graphics_-CSE-333-IIID-/assets/108830659/08b2b403-e0b4-49aa-a42c-9d3205efe5af)

![ezgif com-video-to-gif-converted (1)](https://github.com/Akash21009/Computer_Graphics_-CSE-333-IIID-/assets/108830659/6ee8d309-ec28-4f4f-ae86-f07351ce17a6)
## Introduction

In the realm of computer graphics, the representation of trees is crucial for various applications. Traditional tree modeling methods such as Rule-based modeling, Image-based modeling, and Sketch-based modeling each have their strengths and weaknesses. This project introduces a novel approach - Single Image Tree Modeling - enabling users to create 3D tree models effortlessly from a single image.

### Key Advantages:
- **User-Friendly for Amateurs:** Eliminates complexity for novice users.
- **Efficiency Over Sketch Generation:** Accelerates tree generation compared to sketch-based methods.
- **Minimal Data Requirement:** Only requires a single image, reducing data acquisition burden.

## Literature Review

The proposed approach integrates rule-based, image-based, and sketch-based techniques, minimizing user intervention while creating visually convincing 3D tree models. Fang et al. present a method where users draw strokes to define the tree's structure, and automation enhances realism by tracing visible branches.

### Methodology:
1. **Image Plane Sketching:**
   - Users draw strokes to mark crown and trunk.
   - Foliage extracted using GrabCut.
   - Tracing algorithm automates visible branch creation.

2. **Tree Growing:**
   - Visible branches converted from 2D to 3D.
   - Library of elementary subtrees created.
   - Leaves generated based on branch structure and image information.

3. **Growth Engine:**
   - Non-parametric synthesis approach with attractors guiding growth.
   - Alternates between image-driven and 3D point-driven growth.

4. **Completing the Tree:**
   - Leaves synthesized automatically and textured with input image.

## Milestones
- **Sketching visible branches and foliage region.**

  In the first screen, users draw strokes on the picture of the tree through the cursor to mark visible branches and the trunk of the tree. A C++ program will generate a text file named branches.txt, which stores pairs of 3D coordinates of each stroke so these can be used later in visible branch generation. We also require the location of the root of the tree so we can apply any upcoming algorithm on the tree recursively. As the trunk is obviously visible in the image of trees, the user must click first on the root while sketching visible branches so we can store the location of the root. On-screen, the program renders only an odd number of pairs to avoid continuous tracing of lines and allow the user to generate separate strokes. The renderer will use normalized coordinates as per image dimension.
  In the second screen, users draw anchor points through the cursor on the foliage region. Here, the user tries to cover the boundaries of the foliage region. A C++ program will generate a text file named foliage.txt, which stores 3D coordinates of each anchor point. These coordinates will be used later to limit leaves and branch generation within the foliage region. In this step, our goal is to strictly determine boundary points. Inner points will not contribute to further algorithms. However, the user can increase the area boundary at any time in this step by adding more anchors. On the screen, green dots are visible on each anchor point.

- **Generate visible branches and hidden branches within the foliage region.**

  We have stored the coordinates of each branch. We can represent them as cylinders in 3D. For that, we need a `DrawCylinder` function that can generate cylinders in 3D space.
  Mainly, `DrawCylinder` takes three parameters `p1`, `p2`, and `radius`. `p1` and `p2` are 3D coordinates where `p1` determines the origin of the branch and `p2` determines the end of the branch. The cylinder rotates itself in 3D space according to `p1` and `p2`. The `radius` variable determines the radius of branches. Currently, we have two issues with the current data. First, all branches are separated from each other; we need to join them properly. Second, the format of coordinate data is unordered and it’s difficult to apply any recursive algorithm to them.
  To solve the first problem, we can iterate over branches and find its closer branch. Then split that closer branch such that there should be a common point that joins those branches.

        branches = storedData
        connectedBranches
        for branch in branches:
            for branchToCheck in branches[branch.index to branches.size]:
                if branch is close to branchToCheck:
                    connectedBranches.add(three different branches where one point is common in each branch)


  For the second problem, we need a different perspective. Viewing tree as a directed acyclic graph with at
  max outdegree 2. Now instead of storing the coordinates of each branch in pairs, we can keep track of
  nodes in key-value pairs where each node is pointing to it’s outgoing node. With the root information
  that we stored earlier, it will be easy to apply generation and modification algorithms to tree.
  
        Array connectedbranches
        Map nodes
            for branch in connectedbranches:
                if nodes have any of branch coordinate as key:
                if nodes[branch coordinate] have out degree less than 2:
                    nodes[branch coordinate].add(hanging coordinate)
                if nodes[branch coordinate] have out degree equal to 2:
                    caland store in subBranches culate midpoints of both branches form from branch coordinate
                    join hanging coordinate with closest midpoint obtained
  
  Now we want to generate some hidden branches which can act as base for subbranches and subsubbranches
  generation within convex hull. These branches can be generated by spreading visible tree, which we
  stored in nodes, to boundaries of foliage region.
  To identify boundary regions we can use Graham’s scan algorithm on stored foliage coordinates to
  generate a set of boundary points and identify whether the point is inside, outside or on the boundary of
  the convex hull formed by the set of points. These will be also helpful in further branch generation.
  Now we will add boundary coordinates to visible branches via same algorithm used for populating nodes
  map. We simply iterate in nodes and generate 3d branches via DrawCylinder algorithm.

- **Branch rotation in 3D space.**

  In nature, if we consider two branches connected at one node and view is on xy plane, we will observe
  some angle let’s say 𝛼. Now viewing them from yz plane we will observe some angle 𝛽 . In nature, these
  angles are not exactly equal but proportional i.e. 𝛼 ∝ 𝛽. Extent of proportionality varies in different
  species of trees. For the program, we can use any suitable extent of proportionality that makes tree as
  realistic as possible.

  Calculate the angle between branches from sketch plane then apply suitable proportionality so that
  branches of the base tree will rotate in 3D. Both branches which are connected to a single node should
  rotate at the opposite angle. We can use the axis of rotation for the current branch to it’s corresponding
  branch which will avoid the absurd rotation of the tree due to the choice of the wrong plane. Also, only
  z coordinate should be modified to apply rotation because we sketched tree on xy plane, hence x and y
  coordinates of branches can’t be modified else tree structure deviate from image provided.
  Start from root node and apply rotations recursively. All above branches should be rotated(i.e. there z
  coordinate should modify) by the influence of the current branch rotation.

         rotation(root) {
            if root not in nodes key:
                return
            else:
                if nodes[root] out degree is 2
                    applyrotate(nodes[root][0])
                    applyrotate(nodes[root][1])
                    rotation(nodes[root][0])
                    rotation(nodes[root][1])
                if nodes[root] out degree is 1
                    rotation(nodes[root][0])
          }

  
- **Leaves, subbranches, and subsubbranches generation on the base tree.**

  Trees in nature often show recursive branch structure. L sytem is a popular method for tree generation
  using a recursive growth funtion so we can use this idea to generate subbranches as a recursive base
  tree which is translated, scaled down, and rotated at some angle. However, we have to ensure that
  these subbranches do not exceed the boundary of the convex hull. And in many cases, it will cross the
  boundary. In that case we can use some subbranch stored structures. For branches that are at end of
  boundary, there is high probability that subbranches will exceed the convex hull boundary, so we will
  try to generate more subbranches at end branches.
  Here we will try to generate two subbranches at each branch and for end branches, we will generate six
  subbranches. Rotation is random of subbranches hence it is possible to get slightly different tree each
  time subbranches are generated.
  Recursive base tree structure is priority for subbranches but if it exceeds from boundary of convex hull
  we iterate it in our available library to get a suitable subbranch structure. This library stored root and
  node data of different trees.
  A transform function is required which will scale and rotate subtree from its root and translate it to a
  specific point.
  Similarly, we can generate subsubbranches i.e. smaller subbranches over subbranches with the same
  conditions. This can go further like subsubsubbranches but subsubbranches are enough to get a decent
  tree structure without applying significant load on the machine.

          Array of Maps subBranches
          generateSubBranches(nodes) {
              for node in nodes:
                  for n in nodes[node]:
                      if branch is not end branch:
                          generate 2 subbranches and store in subBranches amp
                      if any of the subbranch exceeds the boundary of convex hull:
                          replace that branch with suitable subbranch from library
                      if branch is end branch:
                          generate 6 subbranches and store in subBranches map
                      if any of the subbranches exceeds the boundary of convex hull:
                          replace that branch with suitable subbranch from library
          }

  For leaves, we can avoid leaves on base branch however, we can generate some leaves on end branches
  for a better look and cover foliage region. Density of leaves will be decrease as we move subbranches to
  subsubbranches. We can also use darker color to show high density of leaves.
  
## Final Notes

This project provides a practical and user-friendly solution for creating 3D trees from a single image. The integration of rule-based, image-based, and sketch-based techniques offers a versatile tool for various applications in virtual reality, gaming, and design.

*Author: Akash Maurya, Contact: akash21009@iiitd.ac.in.*
