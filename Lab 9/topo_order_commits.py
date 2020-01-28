#!/usr/bin/env python3
import os, sys, zlib
    
class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()
            
def dfs_visit(u, color, order, allHash):
    color[u] = 1
    for v in sorted(list(allHash[u].children)):
        if color[v] == 0:
            dfs_visit(v, color, order, allHash)
    color[u] = 2
    order.append(u)
        
# Step 1
cwd = os.getcwd() 
gitDir = ""
while True:
    allFile = os.listdir(cwd)
    parDir = os.path.dirname(cwd)
    if ".git" in allFile and os.path.isdir(cwd + '/.git'):
        gitDir = cwd
        break
    else:
        if cwd is parDir: 
            sys.stderr.write("Not inside a Git repository\n")
            exit(1)
        else:
            cwd = parDir

# Step 2 and 3
branches = dict()
branchDir = os.fsencode(gitDir + "/.git/refs/heads")
allHash = dict()
leaf = set()
for file in os.listdir(branchDir):
    filename = os.fsdecode(file)
    os.chdir(branchDir)
    f = open(filename)
    firstLine = f.readline()
    f.close()
    firstLine = firstLine[:40]
    if firstLine not in branches:
        branches[firstLine] = []
    branches[firstLine].append(filename)
    leaf.add(firstLine)
    if firstLine not in allHash:
        allHash[firstLine] = CommitNode(firstLine)
visited = set()
for a in sorted(list(leaf)):
    stack = []
    if a not in visited:
        stack.append(a)
    if a not in allHash:
        allHash[a] = CommitNode(a)
    while stack:
        v = stack.pop()
        if v in visited:
            continue
        visited.add(v)
        folder = v[:2]
        obj = v[2:]
        objDir = os.fsencode(gitDir + "/.git/objects/" + folder)
        os.chdir(objDir)
        f = open(obj, 'rb')
        compressed_contents = f.read()
        f.close()
        decompressed_contents = zlib.decompress(compressed_contents).decode().splitlines()
        parents = []
        for line in decompressed_contents:
            if line.startswith("parent "):
                parentHash = line[7:]
                parents.append(parentHash)
                continue
            else:
                continue
        for parent in parents:
            if parent not in visited:
                stack.append(parent)
            if parent not in allHash:
                allHash[parent] = CommitNode(parent)
            allHash[parent].children.add(v)
            allHash[v].parents.add(parent)
        
allCommits = set()
root_commits = set()
for a in allHash:
    allCommits.add(a)
    if not allHash[a].parents:
        root_commits.add(a)
        
# Step 4
visited = set()
processed = set()
order = []
for s in sorted(list(root_commits)):
    if s in visited:
        continue
    stack = [s]
    visited = set()
    while stack:
        v = stack.pop()
        if v in processed:
            if v not in order:
                order.append(v)
            continue
        visited.add(v)
        processed.add(v)
        stack.append(v)
        for child in sorted(list(allHash[v].children)):
            if child not in visited:
                stack.append(child)

# Step 5
for a in order[:-1]:
    print(a, end = "")
    if a in branches:
        for br in sorted(branches[a]):
            print(" " + br, end = "")
    print("")
    b = order[order.index(a) + 1]
    if b not in allHash[a].parents:
        pOfA = sorted(list(allHash[a].parents))
        for p in pOfA:
            print(p, end = "")
            if pOfA.index(p) != len(pOfA) - 1:
                print(' ', end = "")
        print('=\n')
        cOfB = sorted(list(allHash[b].children))
        print('=', end = "")
        for c in cOfB:
            print(c, end = "")
            if cOfB.index(c) != len(cOfB) - 1:
                print(' ', end = "")
        print('\n', end = "")
if order != []:
    print(order[-1], end = "")
    if order[-1] in branches:
        for br in sorted(branches[order[-1]]):
            print(" " + br, end = "")
    print("")
