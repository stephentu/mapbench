import numpy as np
import matplotlib.pylab as plt
import sys

def parse(fname):
    with open(fname, 'r') as fp:
        lines = fp.readlines()
    lines = [np.array(map(float, l.strip().split())) for l in lines]
    return np.array(lines)

if __name__ == '__main__':
    ns = [1000, 10000, 100000, 1000000, 10000000]
    data = parse(sys.argv[1])

    map_insert = data[np.arange(0, len(ns)),:]
    map_lookup = data[np.arange(len(ns), 2*len(ns)),:]
    umap_insert = data[np.arange(2*len(ns), 3*len(ns)),:]
    umap_lookup = data[np.arange(3*len(ns), 4*len(ns)),:]

    plt.semilogx(ns, map_insert[:,0], 'o-')
    plt.semilogx(ns, umap_insert[:,0], 'x-')
    plt.legend(['map', 'unordered_map'], loc='upper left')
    plt.xlabel('keys')
    plt.ylabel('time/op (ms)')
    plt.savefig('insert.pdf')
    plt.close()

    plt.semilogx(ns, map_lookup[:,0], 'o-')
    plt.semilogx(ns, umap_lookup[:,0], 'x-')
    plt.legend(['map', 'unordered_map'], loc='upper left')
    plt.xlabel('keys')
    plt.ylabel('time/op (ms)')
    plt.savefig('lookup.pdf')
    plt.close()
