#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Script to test performance of SamplableSet vs random choice from numpy

Author: Guillaume St-Onge <guillaume.st-onge.4@ulaval.ca>
"""

import numpy as np
import matplotlib.pyplot as plt
from time import time
from SamplableSet import SamplableSet

#Sampling test
Nlist = [10,30,100,300,1000,3000,10**4]
ss_time_list = []
choice_time_list = []

for N in Nlist:
    weights = np.arange(1,N+1,dtype=np.float64)
    weights /= np.sum(weights)
    elements = list(range(1,N+1))
    #test random choice --- each time it is as if the distribution was new
    t1 = time()
    for i in range(5*10**4):
        np.random.choice(elements,p=weights)
    t2 = time()
    choice_time_list.append(t2-t1)

    #test SamplableSet
    t1 = time()
    s = SamplableSet(min(weights),max(weights), zip(elements,weights))
    for i in range(5*10**4):
        e,w = s.sample()
        s[e] = w*1.
    t2 = time()
    ss_time_list.append(t2-t1)

#Plot results
font_size=8
plt.style.use('seaborn-paper')
plt.rc('text', usetex=True)
plt.rc('font',family='serif',serif='Computer Modern')
plt.rc('xtick', labelsize=font_size)
plt.rc('ytick', labelsize=font_size)
plt.rc('axes', labelsize=font_size)
plt.rc('legend', fontsize=font_size-1)

#color list
color_list = ["#41b6c4","#0c2c84"]


#define figure
width = 7.057/2
height = width/1.5
fig = plt.figure(figsize=(width, height))
ax = fig.add_subplot(1,1,1)
plt.subplots_adjust(left=0.19, bottom=0.17, right=0.95,top=0.95)
plt.loglog(Nlist, choice_time_list, color=color_list[0], label="NumPy RC")
plt.loglog(Nlist, ss_time_list, color=color_list[1], label="SamplableSet")
plt.xlabel("Vector size")
plt.ylabel(r"Time [s] for $5 \times 10^4$ samples")
plt.legend(frameon=False)
plt.show()
