# coding: UTF-8
from __future__ import absolute_import, division, print_function, unicode_literals
import argparse
import numpy
import matplotlib.pyplot as plt


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('fname', type=file)
    args = parser.parse_args()
    img = numpy.loadtxt(args.fname)
    plt.imshow(img, interpolation='nearest')
    plt.gray()
    plt.show()


if __name__ == '__main__':
    main()
