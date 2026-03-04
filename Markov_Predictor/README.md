# Markov Predictor

**Language(s) used:** C++

**Short description:**
- A bigram (trigram WIP) text prediction system that uses Markov chains to analyze word transitions.
- Reads a text file, builds a probabilistic model of word sequences, and generates predictions.
- Uses inverse transform sampling to select the next word based on learned probabilities.

**Key features:**
- Preprocesses text (removes punctuation, converts to lowercase).
- Builds transition maps tracking word frequency patterns.
- Generates 10-word predictions from any starting word.
- Efficient probabilistic sampling using Mersenne Twister RNG.

**How to use:**
1. Place a `test.txt` file in the project directory
2. Modify the starting word in `main()` as needed
3. Compile and run the program

For more on Markov chains, see the [Wikipedia article](https://en.wikipedia.org/wiki/Markov_chain).