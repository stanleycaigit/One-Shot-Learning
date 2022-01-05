# One-Shot-Learning

My implementation of a simple machine learning algorithm that uses one round of historical data (hence, "One-Shot Learning") to predict housing prices based 
on specific weighted attributes.

The goal of this project is to find specific values for the relevant "weights" using a set of training data. Once those value have been found, they are used to
further estimate prices for additional houses.

This project heavily utilizes the functionality of 2-D arrays as well as the Gauss-Jordan elimination algorithm for finding inverses.

This repository contains my C program that performs everything described above, training data "train.txt" that contains data for X number of houses and Y attributes
and a price for each house, test data "data.txt" that contains X number of houses and Y attributes for each house, and reference data "ref.txt" that gives the 
correct calculations performed by the program of each training data with its corresponding test data on houses.
